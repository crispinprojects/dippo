/* dictionary.h
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
 * @file dictionary.h
 * @brief Header file for dictionary lookup functionality in Dippo's speech synthesizer.
 * 
 * This file contains the data structures and function prototypes for word lookup
 * and phonetic transcription retrieval in the Dippo system.
 */
#ifndef DICTIONARY_H
#define DICTIONARY_H

#include <stdio.h>
#include <string.h> 
#include <stdlib.h>
#include <stdint.h>

/**
 * @brief Structure representing a word entry in the dictionary.
 * 
 * This structure holds a word and its corresponding diphone phonetic transcription.
 */
typedef struct {
    char *word;
    char *diphone_str; // The complete diphone sequence for this word
    
} WordEntry;

// Function prototypes
/**
 * @brief Find a word entry in the dictionary by word name
 * 
 * This function searches for a given word in the dictionary and returns its 
 * corresponding WordEntry structure if found.
 * 
 * @param word Pointer to null-terminated string containing the word to search for
 * @return Pointer to WordEntry if found, NULL otherwise
 */
WordEntry* find_word_entry(const char* word);

/**
 * @brief Get the total number of entries in dictionary
 * 
 * This function returns the total number of words stored in the dictionary table.
 * 
 * @return Number of entries in the dictionary
 */
int get_dictionary_size(void);

#endif//DICTIONARY_H
