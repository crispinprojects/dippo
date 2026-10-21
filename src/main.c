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
 
//This diphone synthesizer example is command line only (no GTK)
// Use Makefile to compile

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdint.h>
#include <time.h>

#include "diphone.h"
#include "wavcat.h"
#include "dictionary.h" // find_word_entry 

// Dictionaries for date components
const char* weekdays[] = {"sunday", "monday", "tuesday", "wednesday", "thursday", "friday", "saturday"};
const char* months[] = {"january", "february", "march", "april", "may", "june", "july", "august", "september", "october", "november", "december"};
const char* ordinal_digits[] = {"first", "second", "third", "fourth", "fifth", "sixth", "seventh", "eighth", "ninth", "tenth", "eleventh", "twelfth", "thirteenth", "fourteenth", "fifteenth", "sixteenth", "seventeenth", "eighteenth", "nineteenth", "twentieth", "twenty first", "twenty second", "twenty third", "twenty fourth", "twenty fifth", "twenty sixth", "twenty seventh", "twenty eighth", "twenty ninth", "thirtieth", "thirty first"};

int main(int argc, char **argv) {
    (void) argc;
    (void) argv;
    
    // Get the current date
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    int current_day_of_week = tm.tm_wday; // 0 = Sunday, 1 = Monday, etc.
    int current_day_of_month = tm.tm_mday; // 1-31
    int current_month = tm.tm_mon; // 0 = January, 1 = February, etc.

    char* words[4];
    int num_words = 0;

    // Add weekday
    words[num_words++] = (char*)weekdays[current_day_of_week];

    // Add day of the month, handling multi-word ordinals
    const char* ordinal_word = ordinal_digits[current_day_of_month - 1];
    char* temp_ordinal = (char*)malloc(strlen(ordinal_word) + 1); // Allocate memory
    if (temp_ordinal == NULL) {
        fprintf(stderr, "Memory allocation failed for ordinal_word!\n");
        return 1;
    }
    strcpy(temp_ordinal, ordinal_word); // Copy the string
    
    char* token = strtok(temp_ordinal, " ");
    while (token != NULL) {
        words[num_words++] = token;
        token = strtok(NULL, " ");
    }
    
    // Add month
    words[num_words++] = (char*)months[current_month];

    printf("Date phrase: ");
    for (int i = 0; i < num_words; i++) {
        printf("%s ", words[i]);
    }
    printf("\n");

    DiphoneEntry** diphone_entries = NULL;
    int diphone_number = 0;
    
    // Process each word to get its diphone string
    for (int i = 0; i < num_words; i++) {
        char* word_to_process = words[i];
        WordEntry* word_entry = find_word_entry(word_to_process);
        
        if (word_entry != NULL) {
            char* diphone_string = (char*)malloc(strlen(word_entry->diphone_str) + 1); // Corrected member name
            if (diphone_string == NULL) {
                fprintf(stderr, "Memory allocation failed!\n");
                return 1;
            }
            strcpy(diphone_string, word_entry->diphone_str); // Corrected member name
            
            char* start_ptr = diphone_string;
            char* end_ptr;

            while ((end_ptr = strchr(start_ptr, ' ')) != NULL) {
                *end_ptr = '\0';
                DiphoneEntry* entry = find_diphone_entry(start_ptr);
                if (entry != NULL) {
                    diphone_entries = (DiphoneEntry**)realloc(diphone_entries, (diphone_number + 1) * sizeof(DiphoneEntry*));
                    if (diphone_entries == NULL) {
                        fprintf(stderr, "Memory reallocation failed!\n");
                        free(diphone_string);
                        return 1;
                    }
                    diphone_entries[diphone_number] = entry;
                    printf("lookup: index=%d diphone=%s\n", diphone_number, start_ptr);
                    diphone_number++;
                }
                *end_ptr = ' ';
                start_ptr = end_ptr + 1;
            }
            
            if (*start_ptr != '\0') {
                DiphoneEntry* entry = find_diphone_entry(start_ptr);
                if (entry != NULL) {
                    diphone_entries = (DiphoneEntry**)realloc(diphone_entries, (diphone_number + 1) * sizeof(DiphoneEntry*));
                    if (diphone_entries == NULL) {
                        fprintf(stderr, "Memory reallocation failed!\n");
                        free(diphone_string);
                        return 1;
                    }
                    diphone_entries[diphone_number] = entry;
                    printf("lookup: index=%d diphone=%s\n", diphone_number, start_ptr);
                    diphone_number++;
                }
            }
            free(diphone_string);
        }
    }
    // Clean up temporary ordinal string
    free(temp_ordinal);
    
    printf("diphone number: %d\n", diphone_number);
    
    char* merge_file = "out.wav";
    printf("Merging with a tempo factor of 2.2  (1.0 =normal speed).\n");
    merge_and_amplify_wav_files(merge_file, diphone_number, diphone_entries, 16000, 2.0, 2.2);
    
    char * command_str = "aplay -c 1 -f S16_LE -r 16000 out.wav";
    system(command_str);
    
    
    // Clean up
    free(diphone_entries);

    return 0;
}
