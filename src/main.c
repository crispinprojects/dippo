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
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://gnu.org>.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */
/**
 * @file main.c
 * @brief Main entry point and processing pipeline for Dippo speech synthesizer.
 *
 * This file contains the core application logic including:
 * - Command-line argument parsing
 * - Text preprocessing
 * - Phrase processing
 * - Diphone sequence generation
 * - Audio synthesis configuration
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
 * @brief Prints usage information and available command-line options
 *
 * Displays the program name, basic syntax, and all supported options with their
 * default values. This function is called when invalid arguments are provided or
 * when the user requests help.
 * ./dippo "dippo is not a hippo" -tempo 1.4 -av 1.5  -wav out.wav && aplay out.wav
 * 
 */
void print_help(void) {
    printf("Dippo - diphone speech synthesiser\n");
    printf("usage: dippo \"Phrase Text\" [options]\n");
    printf("options:\n");
    printf("  -mode value     (default=psola)\n");
    printf("  -tempo value    (default=1.0)\n");
    printf("  -av factor      Audio volume amplification factor (default=1.0)\n");
    printf("  -wav filename   Output directly to a WAV audio file\n");
    printf("  -v              Print version information\n");
}

/**
 * @brief Prints version information for the DIPPO speech synthesizer
 *
 * Displays the current version number of the software. This is typically called
 * when the user passes the -v flag.
 */
void print_version(void) {
    printf("DIPPO: Speech Synthesiser version 0.3.0\n");
}


/**
 * @brief Generates context-aware calendar intonation envelopes
 *
 * Creates a dynamic pitch contour array that adapts to different phrase lengths
 * and structures, particularly optimized for calendar-related announcements.
 *
 * @param pitch_points Number of pitch points in the envelope (typically 100)
 * @param diphone_string The combined diphone sequence string being synthesized
 * @return float* Allocated array containing pitch values for each point,
 *                or NULL if allocation fails
 */
float* create_calendar_intonation_envelope(int pitch_points, char* diphone_string) {
    float* envelope = malloc(pitch_points * sizeof(float));
    if (!envelope) return NULL;

    // Count tokens to automatically determine if we are dealing with a long phrase
    int token_count = 0;
    for (int i = 0; diphone_string[i] != '\0'; i++) {
        if (diphone_string[i] == ' ') token_count++;
    }

    // Baseline frequency parameters for a clear, natural male voice register
    float top_pitch = 145.0f;
    float floor_pitch = 110.0f;

    for (int i = 0; i < pitch_points; i++) {
        float progress = (float)i / (pitch_points - 1);

        if (token_count > 8) {
            // MULTI-PHRASE CONTRAST: Perfect for "Date + Time + Appointment" strings
            // Split the long sentence into a two-part declarative arc (60% / 40%)
            if (progress < 0.60f) {
                // First block (e.g., "Tuesday twenty seventh October"): Gentle declination
                float segment_progress = progress / 0.60f;
                envelope[i] = top_pitch - (segment_progress * (top_pitch - (floor_pitch + 10.0f)));
            } else {
                // Second block (e.g., "eleven zero seven amtime dentist appointment"): 
                // Reset pitch slightly upward, then slide down to a definitive full-stop finish
                float segment_progress = (progress - 0.60f) / 0.40f;
                float reset_pitch = top_pitch - 15.0f; // 130 Hz reset
                envelope[i] = reset_pitch - (segment_progress * (reset_pitch - floor_pitch));
            }
        } else {
            // STANDARD PHRASE: For short alert readouts, apply a clean single-clause arc
            // Slopes gently down for 85% of the phrase, then drops comfortably at the very end
            if (progress < 0.85f) {
                float segment_progress = progress / 0.85f;
                envelope[i] = top_pitch - (segment_progress * 20.0f);
            } else {
                float segment_progress = (progress - 0.85f) / 0.15f;
                envelope[i] = (top_pitch - 20.0f) - (segment_progress * 15.0f);
            }
        }
    }

    return envelope;
}

/**
 * @brief Process a text phrase and convert it to diphone sequences for synthesis
 *
 * The core processing function that:
 * 1. Preprocesses the input text (number expansion, etc.)
 * 2. Tokenizes the phrase into words
 * 3. Looks up each word in the dictionary to find corresponding diphones
 * 4. Handles special cases like plural forms and unknown words
 * 5. Combines all diphone sequences into a single string
 * 6. Calls the appropriate vocoder based on selected mode
 *
 * @param phrase The input text to be synthesized
 * @param output_file File handle where synthesized audio will be written
 * @param mode Synthesis mode ("psola" or "ola")
 * @param tempo Speech rate multiplier (1.0 = normal speed)
 * @param av Audio volume amplification factor
 * @return int 0 on success, non-zero on failure
 */
int process_phrase(const char* phrase, FILE* output_file, char* mode, float tempo, float av) {
    if (!phrase || !output_file) return 1;

    // Preprocess the text - convert numbers to words
    char* processed_phrase = preprocess_text(phrase);
    if (!processed_phrase) {
        printf("Warning: Failed to preprocess text\n");
        return 1;
    }

    printf("After preprocessing: \"%s\"\n", processed_phrase);

    // Allocate memory dynamically to prevent stack smashing
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
    char combined_diphones[8192] = {0}; 
    int first_word = 1;

    while (word != NULL) {
        if (strlen(word) == 0) {
            word = strtok(NULL, " \t\n\r");
            continue;
        }

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
                char singular_root[64];
                int copy_len = w_len - 1;
                if (copy_len > 63) copy_len = 63; 
                memcpy(singular_root, word, copy_len);
                singular_root[copy_len] = '\0';

                WordEntry* singular_entry = find_word_entry(singular_root);
                if (singular_entry != NULL) {
                    char root_diphones[4096];
                    strncpy(root_diphones, singular_entry->diphone_str, sizeof(root_diphones) - 1);
                    root_diphones[sizeof(root_diphones) - 1] = '\0';

                    char* pau_pos = strstr(root_diphones, "_pau");
                    if (pau_pos) {
                        *pau_pos = '\0'; 
                    }

                    if (!first_word) {
                        strncat(combined_diphones, " ", remaining_space);
                        remaining_space = sizeof(combined_diphones) - strlen(combined_diphones) - 1;
                    }
                    strncat(combined_diphones, root_diphones, remaining_space);
                    remaining_space = sizeof(combined_diphones) - strlen(combined_diphones) - 1;

                    char plural_suffix[32];
                    char last_char = singular_root[copy_len - 1];
                    snprintf(plural_suffix, sizeof(plural_suffix), "_%c %c_s s_pau", last_char, last_char);
                    strncat(combined_diphones, plural_suffix, remaining_space);
                    first_word = 0;
                    handled_plural = 1;
                }
            }

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
        printf("combined diphones: \"%s\"\n", combined_diphones);
        
        if (strcmp(mode, "psola") == 0) {
            int pitch_points = 100;
            
            // CRITICAL UPGRADE: Build the adaptive calendar envelope array dynamically
            float* intonation_envelope = create_calendar_intonation_envelope(pitch_points, combined_diphones);
            if (!intonation_envelope) return 1;

            printf("Executing PSOLA Mode with Phrase-Level Declination Curve...\n");
            vocoder_psola(combined_diphones, output_file, tempo, av, intonation_envelope, pitch_points);
            free(intonation_envelope); 
        } 
        else {
            printf("Executing Standard OLA Mode...\n");
            vocoder_ola(combined_diphones, output_file, tempo, av); 
        }
        return 0;
    } else {
        printf("No valid diphone sequences found for the phrase.\n");
        return 1;
    }
}

/**
 * @brief Main entry point of the Dippo speech synthesizer application
 *
 * Handles command-line argument parsing and orchestrates the synthesis process.
 * Supports various options to control synthesis parameters including:
 * - Synthesis mode (PSOLA or OLA)
 * - Speech tempo/rate
 * - Audio volume amplification
 * - Output file specification
 *
 * @param argc Number of command-line arguments
 * @param argv Array of command-line argument strings
 * @return int Exit status (0 for success, non-zero for errors)
 */
int main(int argc, char **argv) {
    if (argc < 2) {
        print_help();
        return 1;
    } 
	char* mode = "psola";
	float tempo = 1.0f;
	float av = 1.0f;
	char* wavfilename = NULL;
	char text_raw[512];
	memset(text_raw, 0, sizeof(text_raw));
	int i = 1;
	while (i < argc) {
	if (argv[i][0] == '-') {
	if (strcmp(argv[i], "-tempo") == 0 && (i + 1) < argc) {
	tempo = (float)atof(argv[i + 1]);
	if (tempo <= 0.0f) {
	fprintf(stderr, "Error: Tempo must be positive\n");
	return 1;
	}
	i += 2;
	continue;
	} else if (strcmp(argv[i], "-mode") == 0 && (i + 1) < argc) {
	mode = argv[i + 1];
	if (!mode || strlen(mode) == 0) {
	fprintf(stderr, "Error: Invalid mode\n");
	return 1;
	}
	i += 2;
	continue;
	} else if (strcmp(argv[i], "-wav") == 0 && (i + 1) < argc) {
	wavfilename = argv[i + 1];
	if (!wavfilename || strlen(wavfilename) == 0) {
	fprintf(stderr, "Error: Invalid WAV filename\n");
	return 1;
	}
	i += 2;
	continue;
	} else if (strcmp(argv[i], "-av") == 0 && (i + 1) < argc) {
	av = (float)atof(argv[i + 1]);
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
	printf("Input: %s\n", text_raw);
	printf("Mode: %s Tempo: %.2f, Volume Factor: %.2f\n", mode, tempo, av);
	FILE* output_file = NULL;
	if (wavfilename == NULL) {
	wavfilename = "out.wav";
	}
	output_file = fopen(wavfilename, "wb");
	if (!output_file) {
	fprintf(stderr, "Error: Cannot open %s for writing\n", wavfilename);
	return 1;
	}
	int result = process_phrase(text_raw, output_file, mode, tempo, av);
	fclose(output_file);
	if (result != 0) {
	printf("Failed to generate speech for: %s\n", text_raw);
	}
	return result;
	}
