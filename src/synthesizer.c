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
 * Implements OLA and PSOLA vocoders for concatenative speech synthesis. Handles
 * pitch tracking, waveform manipulation, and WAV file generation.
 */
#include "synthesizer.h"
#include "diphone.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <math.h>

#define MAX_SAMPLES 1000000
#define SAMPLE_RATE 16000

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

static PSOLA_DiphoneEntry mock_entry;
static int16_t* mock_waveform_data = NULL;
static int* mock_epoch_indices = NULL;

extern DiphoneEntry* find_diphone_entry(const char* diphone_str);

/**
 * @brief Writes a WAV file header with PCM format parameters.
 *
 * Configures the RIFF/WAVE header for mono 16-bit audio at 16 kHz sample rate.
 *
 * @param file Output file handle
 * @param data_size Size of audio data in bytes
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
    int sampleRate = SAMPLE_RATE;
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
 * @brief Refines epoch marker using center-of-gravity method with safety bounds
 *
 * Analyzes the energy distribution around a candidate marker and finds
 * the point of maximum amplitude within constrained search windows.
 *
 * @param data Input waveform
 * @param initial_marker Starting position for analysis (in samples)
 * @param min_bound Minimum allowed position in waveform
 * @param max_bound Maximum allowed position in waveform
 * @param search_window Size of window to analyze (samples)
 * @return Optimized epoch position, clamped within bounds
 */
int refine_with_cog(const int16_t* data, int initial_marker,
                    int min_bound, int max_bound,
                    int min_search, int max_search) {
    if (!data || max_bound <= min_bound || initial_marker < min_bound || initial_marker > max_bound) {
        return initial_marker;
    }

    int start = initial_marker - min_search;
    if (start < min_bound) start = min_bound;
    
    int end = initial_marker + max_search;
    if (end > max_bound) end = max_bound;
    
    // STRICT BOUNDS CHECK: Ensure windows have a valid tracking length
    if (start >= end || start >= max_bound) {
        return initial_marker; 
    }

    float max_energy = -1.0f; 
    int cog_point = initial_marker;

    for (int i = start; i <= end; i++) {
        // Prevent array overruns near the upper boundary limits
        if (i > max_bound) break;
        
        float current_energy = fabsf((float)data[i]);
        if (current_energy > max_energy) {
            max_energy = current_energy;
            cog_point = i;
        }
    }
    return cog_point;
}

/**
 * @brief Applies a triangular window to a frame of audio samples.
 *
 * Scales each sample by a linear ramp up/down for smooth overlap.
 *
 * @param buffer input/output waveform buffer
 * @param frame_size Number of samples in the frame
 * @param half_frame Midpoint of the frame (for window calculation)
 */
void apply_triangle_window(int16_t* buffer, int frame_size, int half_frame) {
    for (int i = 0; i < frame_size; i++) {
        float window = (i < half_frame) ? (float)i / half_frame : (float)(frame_size - i) / half_frame;
        buffer[i] = (int16_t)((int32_t)buffer[i] * window);
    }
}

/**
 * @brief Implements the AMDF pitch tracking algorithm with Center-of-Gravity refinement.
 *
 * Detects pitch periods by finding minima in the Average Magnitude Difference
 * Function across overlapping frames, then refines each marker using energy-based
 * center-of-gravity analysis for improved phase alignment.
 *
 * @param data Input waveform (int16)
 * @param size Number of samples
 * @param min_period Minimum expected period (samples)
 * @param max_period Maximum expected period (samples)
 * @param marks Output array of detected epochs (allocated by function)
 * @return Number of detected epochs, or -1 on error.
 */
int amdf_pitch_tracker(const int16_t* data, size_t size, int min_period, int max_period, int** marks) {
    if (!data || !marks || min_period <= 0 || max_period <= min_period || size < (size_t)max_period * 2) {
        return -1;
    }
    float* amdf = calloc(max_period + 1, sizeof(float));
    if (!amdf) return -1;
    int num_marks = 0;
    *marks = NULL;
    const size_t frame_size = max_period * 2;
    
    for (size_t start = 0; start < size - frame_size; start += frame_size / 4) {
        memset(amdf, 0, (max_period + 1) * sizeof(float));
        for (int lag = min_period; lag <= max_period; lag++) {
            float sum_diff = 0.0f;
            for (size_t i = start; i < start + frame_size - lag && i + lag < size; i++) {
                sum_diff += fabsf((float)(data[i] - data[i + lag]));
            }
            amdf[lag] = sum_diff / (frame_size - lag);
        }
        
        int best_lag = min_period;
        float min_val = amdf[min_period];
        for (int lag = min_period + 1; lag <= max_period; lag++) {
            if (amdf[lag] < min_val) {
                min_val = amdf[lag];
                best_lag = lag;
            }
        }
        
        // Safety bounds check for the end of the file descriptor arrays
        int target_initial = start + best_lag;
        if (target_initial < (int)size) {
            int cog_marker = refine_with_cog(data, target_initial,
                                             (int)start, (int)(size - 1),
                                             min_period / 2, max_period * 2);
            num_marks++;
            int* temp_marks = realloc(*marks, num_marks * sizeof(int));
            if (!temp_marks) {
                free(amdf);
                return -1;
            }
            *marks = temp_marks;
            (*marks)[num_marks - 1] = cog_marker;
        }
    }
    free(amdf);
    return num_marks;
}


/**
 * @brief Loads and processes a diphone for PSOLA synthesis.
 *
 * Extracts waveform data from DiphoneEntry, converts to int16, and computes
 * pitch periods using AMDF. Returns a PSOLA_DiphoneEntry with processed data.
 *
 * @param name Name of the diphone (e.g., "aa-aa")
 * @return Pointer to PSOLA_DiphoneEntry or NULL on failure.
 */
PSOLA_DiphoneEntry* find_psola_diphone(const char* name) {
    // Clean up older lifespan allocations cleanly
    if (mock_waveform_data) { free(mock_waveform_data); mock_waveform_data = NULL; }
    if (mock_epoch_indices) { free(mock_epoch_indices); mock_epoch_indices = NULL; }

    // Use your absolute production lookup tracking loop directly
    DiphoneEntry* entry = find_diphone_entry(name);
    if (!entry || !entry->data || entry->size < 44) {
        // Return a hard NULL and do not alter our structural mock container maps
        return NULL;
    }

    mock_entry.name = name;
    uint32_t offset = 44; 
    uint32_t data_chunk_size = entry->size - 44;
    unsigned int num_samples = data_chunk_size / sizeof(int16_t);

    mock_waveform_data = malloc(num_samples * sizeof(int16_t));
    if (!mock_waveform_data) return NULL;

    for (unsigned int i = 0; i < num_samples; i++) {
        const unsigned char* ptr = entry->data + offset + (i * sizeof(int16_t));
        mock_waveform_data[i] = (int16_t)(ptr[0] | (ptr[1] << 8));
    }

    mock_entry.size = num_samples * sizeof(int16_t);
    mock_entry.data = mock_waveform_data;

    int min_period = 50; 
    int max_period = 220; 
    
    int num_marks = amdf_pitch_tracker(mock_waveform_data, num_samples, min_period, max_period, &mock_epoch_indices);
    if (num_marks <= 0) {
        free(mock_waveform_data);
        mock_waveform_data = NULL;
        return NULL;
    }

    mock_entry.epochs.sample_indices = mock_epoch_indices;
    mock_entry.epochs.num_marks = num_marks;
    return &mock_entry;
}

/**
 * @brief PSOLA vocoder with dynamic pitch and tempo control.
 *
 * Synthesizes speech by adjusting epoch spacing based on a target pitch array
 * and applying a triangular window for overlap-add. Supports phrase-level
 * intonation modulation.
 *
 * @param diphone_string Space-separated list of diphones (e.g., "pau_d d_ih ih_p p_pau pau_ow ow_ih")
 * @param output_file Output WAV file handle
 * @param tempo Time-stretch factor (1.0 = normal speed)
 * @param av Amplitude scaling factor (1.0 = no change)
 * @param pitch_array Optional array of target pitches (Hz) per diphone
 * @param pitch_array_len Length of the pitch array
 */
void vocoder_psola(char* diphone_string, FILE* output_file, float tempo, float av, const float* pitch_array, int pitch_array_len) {
    int current_buffer_max = 500000;
    int16_t* output_buffer = calloc(current_buffer_max, sizeof(int16_t));
    if (!output_buffer) return;

    size_t diphone_str_len = strlen(diphone_string) + 1;
    char* diphone_copy = malloc(diphone_str_len);
    if (!diphone_copy) { free(output_buffer); return; }
    memcpy(diphone_copy, diphone_string, diphone_str_len);
    
    int total_tokens = 0;
    int in_token = 0;
    for (size_t i = 0; diphone_string[i] != '\0'; i++) {
        if (diphone_string[i] != ' ') {
            if (!in_token) { total_tokens++; in_token = 1; }
        } else { in_token = 0; }
    }

    char* token = strtok(diphone_copy, " ");
    int out_ptr = 160; 
    int current_token_index = 0;

    while (token != NULL) {
        if (strlen(token) == 0) { 
            token = strtok(NULL, " "); 
            continue; 
        }

        PSOLA_DiphoneEntry* dentry = find_psola_diphone(token);
        if (!dentry || !dentry->data || dentry->epochs.num_marks <= 2) {
            // CRITICAL SAFEGUARD: Print a clean warning trace line and move forward safely
            printf("  -> Warning: Skipping missing/invalid acoustic unit resource: '%s'\n", token);
            current_token_index++; // Always step the index to prevent timeline locking
            token = strtok(NULL, " ");
            continue;
        }
        
        int in_idx = 0;
        float virtual_time = 0.0f; 
        int num_samples_total = dentry->size / sizeof(int16_t);

        while (in_idx < dentry->epochs.num_marks - 1) {
            float progress = (float)current_token_index / (total_tokens > 1 ? total_tokens - 1 : 1);
            float targeted_f0 = 140.0f;
            if (pitch_array && pitch_array_len > 0) {
                int p_idx = (int)(progress * (pitch_array_len - 1));
                if (p_idx >= pitch_array_len) p_idx = pitch_array_len - 1;
                if (p_idx < 0) p_idx = 0;
                targeted_f0 = pitch_array[p_idx];
            }

            int synthesis_pitch_period = (int)((float)SAMPLE_RATE / targeted_f0);
            if (synthesis_pitch_period <= 20) synthesis_pitch_period = 114;

            if (out_ptr + (synthesis_pitch_period * 3) >= current_buffer_max) {
                int old_max = current_buffer_max;
                current_buffer_max += 500000;
                int16_t* expanded = realloc(output_buffer, current_buffer_max * sizeof(int16_t));
                if (!expanded) { free(diphone_copy); free(output_buffer); return; }
                output_buffer = expanded;
                memset(output_buffer + old_max, 0, 500000 * sizeof(int16_t));
            }

            while (in_idx < dentry->epochs.num_marks - 1 && dentry->epochs.sample_indices[in_idx] < (int)virtual_time) {
                in_idx++;
            }
            if (in_idx >= dentry->epochs.num_marks - 1) break;

            int cur_epoch = dentry->epochs.sample_indices[in_idx];
            int next_epoch = dentry->epochs.sample_indices[in_idx + 1];
            int analysis_pitch_period = next_epoch - cur_epoch;
            if (analysis_pitch_period <= 0) analysis_pitch_period = synthesis_pitch_period;

            int frame_size = analysis_pitch_period * 2;
            int start_offset = cur_epoch - analysis_pitch_period;

            if (start_offset >= 0 && (start_offset + frame_size) < num_samples_total) {
                for (int k = 0; k < frame_size; k++) {
                    int out_target = out_ptr + k - analysis_pitch_period;
                    if (out_target >= 0 && out_target < current_buffer_max) {
                        float window = 0.5f * (1.0f - cosf((2.0f * M_PI * k) / (frame_size - 1)));
                        int32_t sample = dentry->data[start_offset + k];
                        int32_t windowed = (int32_t)(sample * window * av);
                        
                        int32_t combined = output_buffer[out_target] + windowed;
                        if (combined > 32767) combined = 32767;
                        if (combined < -32768) combined = -32768;
                        output_buffer[out_target] = (int16_t)combined;
                    }
                }
            }
            out_ptr += synthesis_pitch_period;
            virtual_time += synthesis_pitch_period * tempo;
        }
        current_token_index++;
        token = strtok(NULL, " ");
    }
    
    if (out_ptr > 0) {
        write_wav_header(output_file, out_ptr * sizeof(int16_t));
        fwrite(output_buffer, sizeof(int16_t), out_ptr, output_file);
    }
    free(diphone_copy);
    free(output_buffer);
}

/**
 * @brief Overlap-and-Add (OLA) vocoder for diphone concatenation.
 *
 * Synthesizes speech by overlapping and adding diphone segments with adjustable
 * tempo and amplitude. Maintains backward compatibility with older versions.
 *
 * @param diphone_string Space-separated list of diphones (e.g., "pau_d d_ih ih_p p_pau pau_ow ow_ih")
 * @param output_file Output WAV file handle
 * @param tempo Time-stretch factor (1.0 = normal speed)
 * @param av Amplitude scaling factor (1.0 = no change)
 */
void vocoder_ola(char* diphone_string, FILE* output_file, float tempo, float av) {  
   if (tempo <= 0.1f) {
        tempo = 2.0f; 
    }
    
    int16_t* output_buffer = malloc(MAX_SAMPLES * sizeof(int16_t));
    if (output_buffer == NULL) {
        printf("Error: Failed to allocate output buffer\n");
        return;
    }
	memset(output_buffer, 0, MAX_SAMPLES * sizeof(int16_t));
	int output_size = 0;
	const int FRAME_SIZE = 160;
	const int analysis_step = 80;
	int synthesis_step = (int)(analysis_step / tempo);
	int currentOutStart = 0;
	char* diphone_copy = malloc(strlen(diphone_string) + 1);
	if (!diphone_copy) {
	free(output_buffer);
	return;
	}
	strcpy(diphone_copy, diphone_string);
	char* token = strtok(diphone_copy, " ");
	int first_diphone = 1;
	while (token != NULL) {
	if (strlen(token) == 0) {
	token = strtok(NULL, " ");
	continue;
	}
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
	if (first_diphone) {
	int samples_to_copy = sourceSamples;
	if (currentOut + samples_to_copy > MAX_SAMPLES) {
	printf("Warning: Output buffer overflow for %s\n", token);
	break;
	}
	for (int k = 0; k < samples_to_copy; k++) {
	if (currentOut + k < MAX_SAMPLES) {
	output_buffer[currentOut + k] = source[k];
	}
	}
	currentOutStart = currentOut + samples_to_copy;
	first_diphone = 0;
	}
	else {
	while (currentIn + FRAME_SIZE < sourceSamples) {
	if (currentOut + FRAME_SIZE > output_size) {
	int new_size = currentOut + FRAME_SIZE + 1000;
	if (new_size > MAX_SAMPLES) break;
	output_size = new_size;
	}
	for (int k = 0; k < FRAME_SIZE; ++k) {
	float window;
	int half_frame = FRAME_SIZE / 2;
	if (k < half_frame)
	window = (float)k / half_frame;
	else
	window = (float)(FRAME_SIZE - k) / half_frame;
	int32_t currentSample = output_buffer[currentOut + k];
	int32_t newSample = (int32_t)(source[currentIn + k] * window);
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
	currentOutStart = currentOut - synthesis_step;
	}
	}
	token = strtok(NULL, " ");
	}
	free(diphone_copy);
	int total_samples = currentOutStart;
	if (total_samples > 0) {
	write_wav_header(output_file, total_samples * sizeof(int16_t));
	fwrite(output_buffer, sizeof(int16_t), total_samples, output_file);
	}
	free(output_buffer);
}
   

