/* synthesizer.h
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
 * @file synthesizer.h
 * @brief Header file for audio synthesis functionality in Dippo.
 * 
 * This header file contains declarations and prototypes for the core audio synthesis
 * functions used by Dippo, including WAV file generation and diphone processing.
 */

#ifndef SYNTHESIZER_H
#define SYNTHESIZER_H

#include <stdio.h>
#include <stdint.h>

// Forward declaration of diphone entry structure 

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
void write_wav_header(FILE* file, int data_size);

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
void apply_triangle_window(int16_t* buffer, int frame_size, int half_frame);

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
void vocoder(char* diphone_string, FILE* output_file, float tempo, float av);

#endif // WAVE_GENERATOR_H
