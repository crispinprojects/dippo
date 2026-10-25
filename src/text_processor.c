/* text_processor.c
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
 * @file text_processor.c
 * @brief Implementation of text preprocessing functions for Dippo.
 * 
 * This file contains functions for converting numbers to words, handling ordinals,
 * and processing time formats in the input text before speech synthesis.
 */
#include "text_processor.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Lookup table for ordinals (1st through 31st)
static const char* ordinal_words[] = {
 "first", "second", "third", "fourth", "fifth", 
 "sixth", "seventh", "eighth", "ninth", "tenth",
 "eleventh", "twelfth", "thirteenth", "fourteenth", "fifteenth",
 "sixteenth", "seventeenth", "eighteenth", "nineteenth", "twentieth",
 "twenty first", "twenty second", "twenty third", "twenty fourth", "twenty fifth",
 "twenty sixth", "twenty seventh", "twenty eighth", "twenty ninth", "thirtieth",
 "thirty first"
};

// Lookup table for cardinal numbers 0-99
static const char* cardinal_words[] = {
 "zero", "one", "two", "three", "four", "five", 
 "six", "seven", "eight", "nine", "ten",
 "eleven", "twelve", "thirteen", "fourteen", "fifteen",
 "sixteen", "seventeen", "eighteen", "nineteen", "twenty",
 "twenty one", "twenty two", "twenty three", "twenty four", "twenty five",
 "twenty six", "twenty seven", "twenty eight", "twenty nine", "thirty",
 "thirty one", "thirty two", "thirty three", "thirty four", "thirty five",
 "thirty six", "thirty seven", "thirty eight", "thirty nine", "forty",
 "forty one", "forty two", "forty three", "forty four", "forty five",
 "forty six", "forty seven", "forty eight", "forty nine", "fifty",
 "fifty one", "fifty two", "fifty three", "fifty four", "fifty five",
 "fifty six", "fifty seven", "fifty eight", "fifty nine"
};

// Special case for 01-09 (zero-prefixed numbers)
static const char* zero_prefixed_words[] = {
 "zero one", "zero two", "zero three", "zero four", "zero five",
 "zero six", "zero seven", "zero eight", "zero nine"
};

/**
 * @brief Check if a string ends with an ordinal suffix
 * 
 * This function determines whether the given string has an ordinal suffix such as
 * 'st', 'nd', 'rd', or 'th'.
 * 
 * @param str Pointer to null-terminated string to check
 * @return Non-zero value if string ends with ordinal suffix, zero otherwise
 */
int is_ordinal_suffix(const char* str) {
 int len = strlen(str);
 if (len < 3) return 0;
 
 // Get last 2 characters
 char last_two[3] = {str[len-2], str[len-1], '\0'};
 
 return (strcmp(last_two, "st") == 0 || 
 strcmp(last_two, "nd") == 0 ||
 strcmp(last_two, "rd") == 0 ||
 strcmp(last_two, "th") == 0);
}

/**
 * @brief Extract number from ordinal string
 * 
 * This function extracts the numeric value from an ordinal string such as "23rd".
 * It returns -1 for invalid ordinals.
 * 
 * @param word Pointer to null-terminated string containing the ordinal
 * @return Integer representing the extracted number, or -1 if invalid
 */
int extract_ordinal_number(const char* word) {
 int len = strlen(word);
 
 if (len >= 3 && isdigit((unsigned char)word[0])) { 
 // Get last 2 characters
 char last_two[3] = {word[len-2], word[len-1], '\0'};
 
 if (strcmp(last_two, "st") == 0 || 
 strcmp(last_two, "nd") == 0 ||
 strcmp(last_two, "rd") == 0 ||
 strcmp(last_two, "th") == 0) {
 
 // Extract the number part by copying all but last 2 characters
 char num_part[16];
 if (len >= 2) {
 int copy_len = len - 2;
 if (copy_len < 15) { // Ensure we don't overflow buffer
 memcpy(num_part, word, copy_len);
 num_part[copy_len] = '\0';
 } else {
 memcpy(num_part, word, 14);
 num_part[14] = '\0';
 }
 } else {
 num_part[0] = '\0';
 }
 
 return atoi(num_part);
 }
 }
 
 return -1; // Invalid ordinal
}

/**
 * @brief Convert a number to its cardinal word representation (0-99)
 * 
 * Converts numeric values from 0-99 into their English word representations.
 * Returns "unknown" for numbers outside this range.
 * 
 * @param num Integer value to convert
 * @return Pointer to static string containing the word representation, or "unknown"
 */
const char* convert_to_cardinal(int num) {
 if (num >= 0 && num <= 99) {
 return cardinal_words[num];
 }
 return "unknown";
}

/**
 * @brief Process time format by creating new string without modifying original
 * 
 * This function handles conversion of time formats like:
 * - Single digits: "1" -> "one"
 * - Two digits: "15" -> "fifteen"  
 * - Multi-digit times like "153", "1 53" -> "fifteen fifty three"
 * 
 * @param input_time Pointer to null-terminated string containing time format
 * @return Static array containing the processed time words (not thread-safe)
 */
char* process_time_format(const char* input_time) {
 static char result[128];
 strcpy(result, "");
 
 // Remove all punctuation from the time string
 char clean_time[64];
 int j = 0;
 for (int i = 0; input_time[i]; i++) {
 if (isdigit((unsigned char)input_time[i])) {
 clean_time[j++] = input_time[i];
 }
 }
 clean_time[j] = '\0';
 
 // If we have a time string with only digits, process it
 int len = strlen(clean_time);
 if (len == 0) return result;
 
 // Handle special case: single digit number 
 if (len == 1) {
 int num = atoi(clean_time);
 const char* word_form = convert_to_cardinal(num);
 strcpy(result, word_form);
 return result;
 }
 
 // Handle two-digit time like "15" -> "fifteen"
 if (len == 2) {
 int num = atoi(clean_time);
 const char* word_form = convert_to_cardinal(num);
 strcpy(result, word_form);
 return result;
 }
 
 // Handle multi-digit times - we'll extract hours and minutes
 if (len >= 3 && len <= 4) {
 int num = atoi(clean_time);
 char time_str[64];
 sprintf(time_str, "%d", num);
 
 if (len == 3) {
 int hours = (time_str[0] - '0');
 int minutes = ((time_str[1] - '0') * 10) + (time_str[2] - '0');
 
 if (hours >= 0 && hours <= 9) {
 const char* hour_word = convert_to_cardinal(hours);
 strcat(result, hour_word);
 strcat(result, " ");
 
 if (minutes == 0) {
 strcat(result, "zero");
 } else {
 const char* minute_word = convert_to_cardinal(minutes);
 strcat(result, minute_word);
 }
 }
 } 
 else if (len == 4) {
 int hours = ((time_str[0] - '0') * 10) + (time_str[1] - '0');
 int minutes = ((time_str[2] - '0') * 10) + (time_str[3] - '0');
 
 if (hours >= 0 && hours <= 99) {
 const char* hour_word = convert_to_cardinal(hours);
 strcat(result, hour_word);
 strcat(result, " ");
 
 if (minutes == 0) {
 strcat(result, "zero");
 } else {
 const char* minute_word = convert_to_cardinal(minutes);
 strcat(result, minute_word);
 }
 }
 }
 }
 
 return result;
}

/**
 * @brief Remove structural punctuation from a string
 * 
 * This function removes all punctuation characters except apostrophes and hyphens,
 * which are preserved as they may be part of compound words.
 * The modification is done in-place on the input string.
 * 
 * @param str Pointer to null-terminated string to process
 */
void remove_punctuation(char* str) {
    if (!str) return;
    
    while (*str) {
        unsigned char c = (unsigned char)*str;
        
        // Check if the character is an apostrophe (0x27) or a hyphen (0x2D)
        if (c == 0x27 || c == 0x2D) {
            // Leave it intact so it stays part of the word token
            str++;
            continue;
        }
        
        // Replace all other non-alphanumeric and non-space characters with a space
        if (!isalnum(c) && !isspace(c)) {
            *str = ' ';
        }
        str++;
    }
}

/**
 * @brief Check if a token is likely to be a time format (contains digits and spaces)
 * 
 * This function determines whether the given string might represent a time format.
 * It looks for strings that contain both digits and space characters.
 * 
 * @param word Pointer to null-terminated string to check
 * @return Non-zero value if token is likely a time format, zero otherwise
 */
int is_time_format(const char* word) {
 int has_digits = 0;
 int has_spaces = 0;
 
 for (int i = 0; word[i]; i++) {
 if (isdigit((unsigned char)word[i])) has_digits = 1;
 if (isspace((unsigned char)word[i])) has_spaces = 1;
 }
 
 return has_digits && has_spaces;
}

/**
 * @brief Preprocess input text by handling ordinals and time formats
 * 
 * This function performs comprehensive text preprocessing, including:
 * - Converting ordinal numbers to words (1st, 2nd, etc.)
 * - Converting numeric times into word representations (153 -> fifteen fifty three)
 * - Handling zero-prefixed numbers like "01", "02"
 * - Converting regular numbers to their word equivalents
 * 
 * @param input Pointer to null-terminated string containing the raw text
 * @return Pointer to allocated string with processed text, or NULL on error.
 *         Caller must free the returned memory.
 */
char* preprocess_text(const char* input) {
 if (!input) return NULL;
 
 int len = strlen(input);
 char* result = malloc(len * 4 + 1); // Allow for expansion
 if (!result) return NULL;
 
 strcpy(result, "");
 
 // First remove punctuation from the entire string to clean it up properly
 char* clean_input = malloc(len + 1);
 if (!clean_input) {
 free(result);
 return NULL;
 }
 strcpy(clean_input, input);
 remove_punctuation(clean_input);
 
 // Tokenize the cleaned string properly to avoid issues with strtok modification
 char temp[512];
 strcpy(temp, clean_input);
 
 char* words[64]; // Array to store all tokens
 int word_count = 0;
 
 // First pass: collect all words (skip empty strings)
 char* word = strtok(temp, " \t\n\r");
 while (word != NULL && word_count < 63) {
 if (strlen(word) > 0) {
 words[word_count] = malloc(strlen(word) + 1);
 if (words[word_count]) {
 strcpy(words[word_count], word);
 word_count++;
 }
 }
 word = strtok(NULL, " \t\n\r");
 }
 
 // Second pass: process each word
 int first_token = 1;
 for (int i = 0; i < word_count; i++) {
 char* current_word = words[i];
 int len_word = strlen(current_word);
 int is_processed = 0;
 
 //printf("DEBUG: Processing token '%s'\n", current_word);
 
 // Handle ordinals like 1st, 2nd, 3rd, etc.
 if (len_word >= 3 && isdigit((unsigned char)current_word[0])) { 
 if (is_ordinal_suffix(current_word)) {
 int ordinal_num = extract_ordinal_number(current_word);
 
 //printf("DEBUG: Processing '%s', extracted number %d\n", current_word, ordinal_num);
 
 if (ordinal_num >= 1 && ordinal_num <= 31) {
 const char* ordinal_word = ordinal_words[ordinal_num - 1];
 
 //printf("DEBUG: Ordinal number %d converted to: %s\n", ordinal_num, ordinal_word);
 
 if (!first_token) strcat(result, " ");
 strcat(result, ordinal_word);
 is_processed = 1;
 }
 }
 }
 
 // Handle time formats like 153, 1 53, etc.
 if (!is_processed && (strchr(current_word, ':') || strchr(current_word, '.'))) {
 //printf("DEBUG: Processing time format '%s'\n", current_word);
 
 char* processed_time = process_time_format(current_word);
 
 if (!first_token) strcat(result, " ");
 strcat(result, processed_time);
 is_processed = 1;
 }
 
 // Handle numbers
 else if (!is_processed && len_word > 0) {
 int all_digits = 1;
 for (int j = 0; j < len_word; j++) {
 if (!isdigit((unsigned char)current_word[j])) {
 all_digits = 0;
 break;
 }
 }
 
 if (all_digits && strlen(current_word) > 0) {
 //printf("DEBUG: Processing numeric token '%s'\n", current_word); 
 int num = atoi(current_word);
 
// Handle three-digit or four-digit numbers like "153", "1053", or "2026"
if (strlen(current_word) == 3 || strlen(current_word) == 4) {
    char time_str[64];
    strcpy(time_str, current_word);
    
    if (strlen(current_word) == 3 && num > 0) {
        // Clear time layout: e.g., "153" -> 1:53 -> "one fifty three"
        int hours = (time_str[0] - '0');
        int minutes = ((time_str[1] - '0') * 10) + (time_str[2] - '0');
        
        const char* hour_word = convert_to_cardinal(hours);
        const char* minute_word = convert_to_cardinal(minutes);
        
        if (!first_token) strcat(result, " ");
        strcat(result, hour_word);
        strcat(result, " ");
        strcat(result, minute_word);
        is_processed = 1;
    }
    else if (strlen(current_word) == 4 && num > 0) {
        int part1 = ((time_str[0] - '0') * 10) + (time_str[1] - '0');
        int part2 = ((time_str[2] - '0') * 10) + (time_str[3] - '0');
        
        // Context Check: Is this a year in the 2000-2009 slot? (e.g., 2005)
        if (part1 == 20 && part2 >= 0 && part2 <= 9) {
            if (!first_token) strcat(result, " ");
            strcat(result, "two thousand");
            if (part2 > 0) {
                strcat(result, " and ");
                strcat(result, convert_to_cardinal(part2));
            }
            is_processed = 1;
        }
        // Context Check: Is it the year 2000 exactly?
        else if (part1 == 20 && part2 == 0) {
            if (!first_token) strcat(result, " ");
            strcat(result, "two thousand");
            is_processed = 1;
        }
        // General 4-digit handling (Works cleanly for both 2026 ("twenty twenty six") and 1053 ("ten fifty three"))
        else {
            const char* word1 = convert_to_cardinal(part1);
            if (!first_token) strcat(result, " ");
            strcat(result, word1);
            strcat(result, " ");
            
            if (part2 == 0) {
                strcat(result, "hundred"); // e.g., 1900 -> "nineteen hundred"
            } else if (part2 < 10) {
                // If it's a time layout like 10:05 -> "ten zero five"
                // If it's a year layout like 1905 -> "nineteen zero five"
                strcat(result, "zero ");
                strcat(result, convert_to_cardinal(part2));
            } else {
                strcat(result, convert_to_cardinal(part2));
            }
            is_processed = 1;
        }
    }
} 
// Catch-all safety net for unhandled larger digits (e.g., "10000" or higher)
else if (strlen(current_word) >= 5) {
    // Large numbers bypass dictionary cardinal mapping gracefully 
    // by passing individual digits sequentially to avoid hitting G2P crash loops
    for (int j = 0; j < len_word; j++) {
        int digit = current_word[j] - '0';
        if (digit >= 0 && digit <= 9) {
            if (!first_token || j > 0) strcat(result, " ");
            strcat(result, convert_to_cardinal(digit));
        }
    }
    is_processed = 1;
}

 
 // Handle zero-prefixed numbers like 01, 02 etc. using zero_prefixed_words table
 else if (len_word == 2 && current_word[0] == '0' && isdigit((unsigned char)current_word[1])) {
 //int digit = current_word[1] - '0';
 int digit = current_word[1] - '0';

 if (digit >= 1 && digit <= 9) {
 const char* word_form = zero_prefixed_words[digit - 1];
 
 if (!first_token) strcat(result, " ");
 strcat(result, word_form);
 is_processed = 1;
 }
 }
 // If not handled as time format yet, treat as regular number
if (!is_processed && num >= 0 && num <= 99) {
const char* word_form = convert_to_cardinal(num);
if (!first_token) strcat(result, " ");
strcat(result, word_form);
is_processed = 1;
}
}
}
// If nothing was processed, keep original
if (!is_processed) {
if (!first_token) strcat(result, " ");
strcat(result, current_word);
}
first_token = 0;
free(words[i]);
}
free(clean_input);
//printf("DEBUG: Final preprocessed text: '%s'\n", result);
return result;
}
