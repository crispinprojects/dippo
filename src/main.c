/* main.c
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
 * @file main.c
 * @brief Main entry point and processing pipeline for Dippo.
 * 
 * This file contains the main application logic, command-line argument parsing,
 * text preprocessing, and phrase processing functions for the Dippo speech synthesizer.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dictionary.h"
#include "diphone.h"
#include "synthesizer.h"
#include "text_processor.h"
#include "g2p_engine.h"

/**
 * @brief Print usage information and command-line options
 * 
 * This function displays help text showing how to use the Dippo application,
 * including available command-line options.
 */
void print_help(void) {
 printf("Dippo - diphone speech synthesiser\n");
 printf("usage: dippo \"Phrase Text\" [options]\n");
 printf("options:\n");
 printf(" -wav filename Output directly to a WAV audio file\n");
 printf(" -tempo value (default=1)\n");
 printf(" -av factor Audio volume amplification factor (default=1.0)\n");
 printf(" -v Print version information\n");
}


/**
 * @brief Print version information for the DIPPO speech synthesizer
 * 
 * This function prints the application's version number and other identifying information.
 */
void print_version(void) {
 printf("DIPPO: Speech Synthesiser version 0.2.0\n");
}

/**
 * @brief Process a text phrase and convert it to diphone sequences for synthesis
 * 
 * This function takes a text phrase, preprocesses it (converting numbers to words),
 * tokenizes into individual words, looks up each word in the dictionary or uses G2P,
 * and then generates a combined diphone sequence.
 * 
 * @param phrase Pointer to null-terminated string containing the input text
 * @param output_file Pointer to FILE structure for writing output audio data
 * @param tempo Tempo factor for controlling speech speed (default = 1.0)
 * @param av Audio volume amplification factor (default = 1.0)
 * @return Integer indicating success (0) or failure (non-zero)
 */
int process_phrase(const char* phrase, FILE* output_file, float tempo, float av) {
 if (!phrase || !output_file) return 1;
 //printf("Processing phrase: \"%s\"\n", phrase);
 
 // Preprocess the text - convert numbers to words
 char* processed_phrase = preprocess_text(phrase);
 if (!processed_phrase) {
 printf("Warning: Failed to preprocess text\n");
 return 1;
 }
 //printf("After preprocessing: \"%s\"\n", processed_phrase);
 
 // Allocate memory dynamically to prevent stack smashing 
 // when preprocessed strings expand past the original 512-byte ceiling.
 char* phrase_copy = malloc(strlen(processed_phrase) + 1);
 if (!phrase_copy) {
 printf("Error: Failed to allocate memory for phrase processing\n");
 free(processed_phrase);
 return 1;
 }
 strcpy(phrase_copy, processed_phrase);
 
 // Tokenize the phrase into words
 char* word = strtok(phrase_copy, " \t\n\r");
 if (!word) {
 printf("Warning: Empty phrase or no words found.\n");
 free(phrase_copy);
 free(processed_phrase);
 return 1;
 }
 
 // Create a combined diphone string for the entire phrase
 char combined_diphones[8192] = {0}; // Large buffer to handle long phrases
 int first_word = 1;
 
 while (word != NULL) {
 // Skip empty words
 if (strlen(word) == 0) {
 word = strtok(NULL, " \t\n\r");
 continue;
 }
 //printf("Looking up: '%s'\n", word);
 
 // Calculate exactly how much space remains in our main accumulator buffer
 size_t remaining_space = sizeof(combined_diphones) - strlen(combined_diphones) - 1;
 if (remaining_space == 0) {
 printf("Warning: Maximum diphone accumulator buffer threshold reached early.\n");
 break;
 }
 
 // Look up the word in our dictionary
 WordEntry* word_entry = find_word_entry(word);
 if (word_entry != NULL) {
 if (!first_word) {
 strncat(combined_diphones, " ", remaining_space);
 remaining_space = sizeof(combined_diphones) - strlen(combined_diphones) - 1;
 }
 strncat(combined_diphones, word_entry->diphone_str, remaining_space);
 first_word = 0;
 } else {
 // DYNAMIC PLURAL HANDLING: If word fails lookup, check if it ends in 's'
 int w_len = strlen(word);
 int handled_plural = 0;
 
 if (w_len > 1 && word[w_len - 1] == 's') {
 // Create a temporary singular root token safely using memcpy
 char singular_root[64];
 int copy_len = w_len - 1;
 if (copy_len > 63) copy_len = 63; // Buffer overflow safety check
 
 memcpy(singular_root, word, copy_len);
 singular_root[copy_len] = '\0';
 
 WordEntry* singular_entry = find_word_entry(singular_root);
 if (singular_entry != NULL) {
 // Allocate a working layout string to strip the trailing _pau from the root
 char root_diphones[4096];
 strncpy(root_diphones, singular_entry->diphone_str, sizeof(root_diphones) - 1);
 root_diphones[sizeof(root_diphones) - 1] = '\0';
 
 // Find and cut off the closing "_pau" marker of the base word
 char* pau_pos = strstr(root_diphones, "_pau");
 if (pau_pos) {
 *pau_pos = '\0'; // Truncate the silence terminal
 }
 
 if (!first_word) {
 strncat(combined_diphones, " ", remaining_space);
 remaining_space = sizeof(combined_diphones) - strlen(combined_diphones) - 1;
 }
 strncat(combined_diphones, root_diphones, remaining_space);
 remaining_space = sizeof(combined_diphones) - strlen(combined_diphones) - 1;
 
 // Assemble and append the plural suffix sound with explicit bounds checking
 char plural_suffix[32];
 char last_char = singular_root[copy_len - 1];
 snprintf(plural_suffix, sizeof(plural_suffix), "_%c %c_s s_pau", last_char, last_char);
 strncat(combined_diphones, plural_suffix, remaining_space);
 
 first_word = 0;
 handled_plural = 1;
 }
 }
 
 // If it wasn't a plural or singular fallback wasn't found, drop down to G2P
 if (!handled_plural) {
 printf("Warning: No diphone sequence found for word '%s'\n", word);
 char* g2p_result = convert_word_diphone_sequence(word);
 if (g2p_result != NULL) {
 if (!first_word) {
 strncat(combined_diphones, " ", remaining_space);
 remaining_space = sizeof(combined_diphones) - strlen(combined_diphones) - 1;
 }
 strncat(combined_diphones, g2p_result, remaining_space);
 first_word = 0;
 free(g2p_result);
 }
 }
 }
 word = strtok(NULL, " \t\n\r");
 }
 
 free(phrase_copy);
 free(processed_phrase);
 
 if (strlen(combined_diphones) > 0) {
 vocoder(combined_diphones, output_file, tempo, av);
 return 0;
 } else {
 printf("No valid diphone sequences found for the phrase.\n");
 return 1;
 }
}

/**
 * @brief Main entry point of the Dippo speech synthesizer application
 * 
 * This function handles command-line argument parsing and orchestrates the entire
 * text-to-speech processing pipeline.
 * 
 * @param argc Number of command-line arguments
 * @param argv Array of pointers to command-line arguments
 * @return Integer indicating success (0) or failure (non-zero)
 */
int main(int argc, char **argv) {
 if (argc < 2) {
 print_help();
 return 1;
 } 
 float tempo = 1.0f;
 float av = 1.0f; // Audio volume factor
 char* wavfilename = NULL; 
 char text_raw[512]; 
 memset(text_raw, 0, sizeof(text_raw)); 
 int i = 1;
 while (i < argc) {
 if (argv[i][0] == '-') {
 if (strcmp(argv[i], "-tempo") == 0 && (i + 1) < argc) {
 tempo = (float)atof(argv[i + 1]);
 // Validate tempo
 if (tempo <= 0.0f) {
 fprintf(stderr, "Error: Tempo must be positive\n");
 return 1;
 }
 i += 2;
 continue;
 } else if (strcmp(argv[i], "-wav") == 0 && (i + 1) < argc) {
 wavfilename = argv[i + 1];
 // Validate filename
 if (!wavfilename || strlen(wavfilename) == 0) {
 fprintf(stderr, "Error: Invalid WAV filename\n");
 return 1;
 }
 i += 2;
 continue;
 } else if (strcmp(argv[i], "-av") == 0 && (i + 1) < argc) {
 av = (float)atof(argv[i + 1]);
 // Validate amplification factor
 if (av < 0.0f) {
 fprintf(stderr, "Error: Audio volume factor must be non-negative\n");
 return 1;
 }
 i += 2;
 continue;
 } else if (strcmp(argv[i], "-v") == 0) {
 print_version();
 return 0; 
 } else {
 fprintf(stderr, "Unknown option: %s\n", argv[i]);
 print_help();
 return 1;
 }
 } else {
 // Handle text input safely preventing overruns into local raw container
 if (strlen(text_raw) > 0) {
 strncat(text_raw, " ", sizeof(text_raw) - 1 - strlen(text_raw));
 }
 strncat(text_raw, argv[i], sizeof(text_raw) - 1 - strlen(text_raw));
 i++;
 }
 } 
 if (strlen(text_raw) == 0) {
 fprintf(stderr, "DIPPO Error: Input string empty.\n");
 return 1;
 }
 //printf("Input: %s\n", text_raw);
 //printf("Tempo: %.2f, Volume Factor: %.2f\n", tempo, av); 
 
 FILE* output_file = NULL;
 if (wavfilename == NULL) {
 wavfilename = "out.wav";
 }
 output_file = fopen(wavfilename, "wb");
 if (!output_file) {
 fprintf(stderr, "Error: Cannot open %s for writing\n", wavfilename);
 return 1;
 }
 // Process the phrase and generate audio
 int result = process_phrase(text_raw, output_file, tempo, av);
 fclose(output_file); 
 if (result != 0) {
 printf("Failed to generate speech for: \"%s\"\n", text_raw);
 }
 return result;
}
