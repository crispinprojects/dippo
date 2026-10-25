/* synthesizer.c
 *
 * Copyright 2026 Alan Crispin <crispinalan@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

/**
 * @file synthesizer.c
 * @brief Voice coder with overlap-add logic for diphone synthesis.
 * 
 * This file implements the core audio synthesis functionality of Dippo,
 * including WAV header writing, triangle windowing, and overlap-add vocoding.
 */
#include "synthesizer.h"
#include "diphone.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

// Forward declaration of diphone lookup function - assumes this is in diphone.c
extern DiphoneEntry* find_diphone_entry(const char* diphone_str);


/**
 * @brief Write WAV header to file
 * 
 * This function writes the standard WAV file header structure to a file.
 * It sets up proper format information including sample rate, channels,
 * and data size for PCM audio.
 * 
 * @param file Pointer to FILE structure where header will be written
 * @param data_size Size of the audio data in bytes
 */
void write_wav_header(FILE* file, int data_size) {
    fwrite("RIFF", 1, 4, file);
    int fileSize = 36 + data_size;
    fwrite(&fileSize, sizeof(int), 1, file);

    fwrite("WAVEfmt ", 1, 8, file);
    int fmtSize = 16;
    fwrite(&fmtSize, sizeof(int), 1, file);

    short format = 1; // PCM
    fwrite(&format, sizeof(short), 1, file);

    short channels = 1; // Mono
    fwrite(&channels, sizeof(short), 1, file);

    int sampleRate = 16000;
    fwrite(&sampleRate, sizeof(int), 1, file);

    int byteRate = sampleRate * 2;
    fwrite(&byteRate, sizeof(int), 1, file);

    short blockAlign = 2;
    fwrite(&blockAlign, sizeof(short), 1, file);

    short bps = 16;
    fwrite(&bps, sizeof(short), 1, file);

    fwrite("data", 1, 4, file);
    fwrite(&data_size, sizeof(int), 1, file);
}

/**
 * @brief Apply triangle windowing for smooth transitions
 * 
 * This function applies a triangular window to audio samples to reduce
 * discontinuities when overlapping frames during synthesis.
 * 
 * @param buffer Pointer to array of int16_t audio samples
 * @param frame_size Size of the audio frame in samples
 * @param half_frame Half the frame size for window calculation
 */
void apply_triangle_window(int16_t* buffer, int frame_size, int half_frame) {
    for (int i = 0; i < frame_size; i++) {
        float window;
        if (i < half_frame)
            window = (float)i / half_frame;
        else
            window = (float)(frame_size - i) / half_frame;
        
        // Apply window to the sample (multiply by window factor)
        int32_t sample = buffer[i];
        int32_t windowed_sample = (int32_t)(sample * window);
        buffer[i] = (int16_t)windowed_sample;
    }
}

/**
 * @brief Overlap-add vocoder that takes a string of space-separated diphones
 * 
 * This function performs the core audio synthesis by playing back diphone segments,
 * using overlap-add technique to smoothly transition between them.
 * It handles both first and subsequent diphone playback with proper timing adjustments.
 * 
 * @param diphone_string Pointer to null-terminated string containing space-separated diphones
 * @param output_file Pointer to FILE structure for writing WAV audio data
 * @param tempo Tempo factor controlling speech speed (default = 1.0)
 * @param av Audio volume amplification factor (default = 1.0)
 */
void vocoder(char* diphone_string, FILE* output_file, float tempo, float av) {
    //printf("Speaking: %s\n", diphone_string);
    
    // Safety clamp: Ensure tempo is valid and above zero to prevent crash
    if (tempo <= 0.1f) {
        tempo = 2.0f; // Default back to normal speed if invalid
    }
 
    // Create a fixed-size output buffer like in the original
    const int MAX_SAMPLES = 1000000;
    int16_t* output_buffer = malloc(MAX_SAMPLES * sizeof(int16_t));
    if (output_buffer == NULL) {
        printf("Error: Failed to allocate output buffer\n");
        return;
    }
 
    // Initialize the entire buffer with zeros - this is CRITICAL
    memset(output_buffer, 0, MAX_SAMPLES * sizeof(int16_t));
 
    int output_size = 0;
    const int FRAME_SIZE = 160;
    const int analysis_step = 80;
    int synthesis_step = (int)(analysis_step / tempo);
    int currentOutStart = 0;
    
    // Parse the diphone string into individual diphones
    char* diphone_copy = malloc(strlen(diphone_string) + 1);
    if (!diphone_copy) {
        free(output_buffer);
        return;
    }
    strcpy(diphone_copy, diphone_string);
    
    char* token = strtok(diphone_copy, " ");
    int first_diphone = 1;
    
    while (token != NULL) {
        // Skip empty tokens
        if (strlen(token) == 0) {
            token = strtok(NULL, " ");
            continue;
        }
        
        //printf(" Processing diphone: %s\n", token);
 
        DiphoneEntry* dentry = find_diphone_entry(token);
        if (dentry) {
            const int16_t* source = (const int16_t*)(dentry->data + 44);
            int sourceSamples = (dentry->size - 44) / sizeof(int16_t);
 
            if (sourceSamples <= 0) {
                printf("Warning: Invalid diphone data size for %s\n", token);
                token = strtok(NULL, " ");
                continue;
            }
            
            int currentIn = 0;
            int currentOut = currentOutStart;
            
            // For first diphone, just copy it directly
            if (first_diphone) {
                int samples_to_copy = sourceSamples;
                if (currentOut + samples_to_copy > MAX_SAMPLES) {
                    printf("Warning: Output buffer overflow for %s\n", token);
                    break;
                }

                for (int k = 0; k < samples_to_copy; k++) {
                    if (currentOut + k < MAX_SAMPLES) {
                        output_buffer[currentOut + k] = source[k];
                    } else {
                        printf("Warning: Buffer overflow at index %d\n", currentOut + k);
                        break;
                    }
                }
                currentOutStart = currentOut + samples_to_copy;
                first_diphone = 0;
            } 
            // For subsequent diphones, use overlap-add
            else {
                while (currentIn + FRAME_SIZE < sourceSamples) {
                    if (currentOut + FRAME_SIZE > output_size) {
                        int new_size = currentOut + FRAME_SIZE + 1000;
                        if (new_size > MAX_SAMPLES) {
                            printf("Warning: Buffer size exceeded for %s\n", token);
                            break;
                        }
                        output_size = new_size;
                    }

                    // Apply overlap-add with triangle windowing
                    for (int k = 0; k < FRAME_SIZE; ++k) {
                        float window;
                        int half_frame = FRAME_SIZE / 2;
                        if (k < half_frame)
                            window = (float)k / half_frame;
                        else
                            window = (float)(FRAME_SIZE - k) / half_frame;

                        int32_t currentSample = output_buffer[currentOut + k];
                        int32_t newSample = (int32_t)(source[currentIn + k] * window);
                        
                        // Apply volume amplification factor
                        if (av != 1.0f) {
                            long amplified_sample = (long)currentSample * av;
                            if (amplified_sample > 32767) amplified_sample = 32767;
                            if (amplified_sample < -32768) amplified_sample = -32768;
                            currentSample = (int16_t)amplified_sample;
                        }
                        
                        int32_t combined = currentSample + newSample;

                        if (combined > 32767) combined = 32767;
                        if (combined < -32768) combined = -32768;

                        output_buffer[currentOut + k] = (int16_t)combined;
                    }

                    currentIn += analysis_step;
                    currentOut += synthesis_step;
                }
                // Update the start position for next diphone
                currentOutStart = currentOut - synthesis_step;
                //currentOutStart = currentOut;
            }
        } else {
            printf(" Warning: Diphone not found: %s\n", token);
        }
        
        token = strtok(NULL, " ");
    }
    
    free(diphone_copy);
 
    // Write the final audio data to file
    int total_samples = currentOutStart;
    if (total_samples > 0) {
        fwrite("RIFF", 1, 4, output_file);
        int fileSize_pos = ftell(output_file);
        int dummy_fileSize = 0;
        fwrite(&dummy_fileSize, sizeof(int), 1, output_file);
 
        fwrite("WAVEfmt ", 1, 8, output_file);
        int fmtSize = 16;
        fwrite(&fmtSize, sizeof(int), 1, output_file);

        short format = 1; // PCM
        fwrite(&format, sizeof(short), 1, output_file);

        short channels = 1; // Mono
        fwrite(&channels, sizeof(short), 1, output_file);

        int sampleRate = 16000;
        fwrite(&sampleRate, sizeof(int), 1, output_file);

        int byteRate = sampleRate * 2;
        fwrite(&byteRate, sizeof(int), 1, output_file);

        short blockAlign = 2;
        fwrite(&blockAlign, sizeof(short), 1, output_file);

        short bps = 16;
        fwrite(&bps, sizeof(short), 1, output_file);

        fwrite("data", 1, 4, output_file);
        int data_size_pos = ftell(output_file);
        int dummy_data_size = 0;
        fwrite(&dummy_data_size, sizeof(int), 1, output_file);
 
        fwrite(output_buffer, sizeof(int16_t), total_samples, output_file);
 
        int current_pos = ftell(output_file);
        fseek(output_file, fileSize_pos, SEEK_SET);
        int final_file_size = current_pos - 8;
        fwrite(&final_file_size, sizeof(int), 1, output_file);
 
        fseek(output_file, data_size_pos, SEEK_SET);
        int final_data_size = total_samples * sizeof(int16_t);
        fwrite(&final_data_size, sizeof(int), 1, output_file);
 
        printf("Audio saved as wav file\n");
    } else {
       printf("No audio data generated\n");
    }
 
    free(output_buffer);
}
