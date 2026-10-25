/* g2p_engine.c
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
 * @file g2p_engine.c
 * @brief Implementation of grapheme-to-phoneme (G2P) engine for Dippo.
 * 
 * This file contains functions for converting words to their phonetic representations
 * using both dictionary lookup and rule-based prediction methods.
 */
#include "g2p_engine.h"
#include "dictionary.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Forward declaration of helper functions
static void convert_to_lower(char* str);
static const char* predict_phoneme(char p, char c, char n, char nn, char nnn);
static char** convert_word_to_phonemes(const char* word, int* phoneme_count);


/**
 * @brief Convert a string to lowercase
 * 
 * Converts all uppercase characters in a string to their lowercase equivalents.
 * The conversion is done in-place on the input string.
 * 
 * @param str Pointer to the null-terminated string to convert
 */
static void convert_to_lower(char* str) {
 if (!str) return;
 
 for (int i = 0; str[i]; i++) {
 if (str[i] >= 'A' && str[i] <= 'Z') {
 str[i] = str[i] + ('a' - 'A');
 }
 }
}

/**
 * @brief Predict phoneme based on character and context
 * 
 * This function uses the predict functions to determine the appropriate phoneme for a given character,
 * taking into account its surrounding characters.
 * 
 * @param p Previous character (or 0x5F if at start)
 * @param c Current character
 * @param n Next character (or 0x5F if at end)
 * @param nn Character after next (or 0x5F if at end)
 * @param nnn Character two positions after next (or 0x5F if at end)
 * @return Pointer to the predicted phoneme string
 */
static const char* predict_phoneme(char p, char c, char n, char nn, char nnn) {

 switch (c) {
 case 'a': return predictA(p, c, n, nn, nnn);
 case 'b': return predictB(p, c, n, nn, nnn);
 case 'c': return predictC(p, c, n, nn, nnn);
 case 'd': return predictD(p, c, n, nn, nnn);
 case 'e': return predictE(p, c, n, nn, nnn);
 case 'f': return predictF(p, c, n, nn, nnn);
 case 'g': return predictG(p, c, n, nn, nnn);
 case 'h': return predictH(p, c, n, nn, nnn);
 case 'i': return predictI(p, c, n, nn, nnn);
 case 'j': return predictJ(p, c, n, nn, nnn);
 case 'k': return predictK(p, c, n, nn, nnn);
 case 'l': return predictL(p, c, n, nn, nnn);
 case 'm': return predictM(p, c, n, nn, nnn);
 case 'n': return predictN(p, c, n, nn, nnn);
 case 'o': return predictO(p, c, n, nn, nnn);
 case 'p': return predictP(p, c, n, nn, nnn);
 case 'q': return predictQ(p, c, n, nn, nnn);
 case 'r': return predictR(p, c, n, nn, nnn);
 case 's': return predictS(p, c, n, nn, nnn);
 case 't': return predictT(p, c, n, nn, nnn);
 case 'u': return predictU(p, c, n, nn, nnn);
 case 'v': return predictV(p, c, n, nn, nnn);
 case 'w': return predictW(p, c, n, nn, nnn);
 case 'x': return predictX(p, c, n, nn, nnn);
 case 'y': return predictY(p, c, n, nn, nnn);
 case 'z': return predictZ(p, c, n, nn, nnn);
 default: return "ae"; // Default fallback
 }
}

/**
 * @brief Build a diphone sequence from phonemes
 * 
 * This function constructs a space-separated string of diphones from an array of phonemes.
 * Each diphone consists of two consecutive phonemes separated by an underscore.
 * 
 * @param phonemes Array of pointers to phoneme strings
 * @param phoneme_count Number of phonemes in the array
 * @return Pointer to allocated string containing the diphone sequence, or NULL on error
 */
char* build_diphone_list(const char** phonemes, int phoneme_count) {
 if (phoneme_count <= 0 || !phonemes) {
 return NULL;
 }
 // Calculate required memory for result string
 size_t total_length = 0;
 
 // Add space separators between diphones and account for null terminators
 for (int i = 0; i < phoneme_count; i++) {
 if (phonemes[i]) {
 total_length += strlen(phonemes[i]);
 }
 if (i < phoneme_count - 1) {
 total_length += 1; // For underscore separator
 }
 }
 
 // Add space separators between words and null terminator
 total_length += phoneme_count + 1;
 
 char* result = malloc(total_length);
 if (!result) return NULL;
 
 result[0] = '\0';
 
 for (int i = 0; i < phoneme_count; i++) {
 if (phonemes[i]) {
 strcat(result, phonemes[i]);
 
 // Add separator between diphones except after last one
 if (i < phoneme_count - 1) {
 strcat(result, " ");
 }
 }
 } 
 return result;
}


/**
 * @brief Convert a word to its phoneme sequence using G2P rules
 * 
 * This function processes each character of the input word and predicts the appropriate phoneme,
 * taking into account context from surrounding characters.
 * 
 * @param word Pointer to null-terminated string containing the word to process
 * @param phoneme_count Pointer to integer that will receive the number of phonemes generated
 * @return Array of pointers to phoneme strings, or NULL on error. Must be freed by caller.
 */
static char** convert_word_to_phonemes(const char* word, int* phoneme_count) {
 if (!word || !phoneme_count) return NULL;
 
 // Create a copy of the word for processing
 char* word_copy = malloc(strlen(word) + 1);
 if (!word_copy) return NULL;
 
 strcpy(word_copy, word);
 convert_to_lower(word_copy);
 
 int len = strlen(word_copy);
 if (len == 0) {
 free(word_copy);
 return NULL;
 }
 
 // Allocate space for phonemes array
 char** phonemes = malloc((len + 1) * sizeof(char*));
 if (!phonemes) {
 free(word_copy);
 return NULL;
 }
 
 int count = 0;
 
 // Process each character with context (0x5F is ASCII code for '_')
 for (int i = 0; i < len; i++) {
 char c = word_copy[i];
 
 // Get context characters - enhanced context handling
 char p = (i > 0) ? word_copy[i-1] : 0x5F;
 char n = (i + 1 < len) ? word_copy[i+1] : 0x5F;
 char nn = (i + 2 < len) ? word_copy[i+2] : 0x5F;
 char nnn = (i + 3 < len) ? word_copy[i+3] : 0x5F;
 
 // Predict phoneme for this character
 const char* phoneme = predict_phoneme(p, c, n, nn, nnn);
 
 if (phoneme && strlen(phoneme) > 0) {
 // Handle special case where we want to skip silent phonemes
 if (strcmp(phoneme, "silent") != 0) {
 phonemes[count] = malloc(strlen(phoneme) + 1);
 if (!phonemes[count]) {
 // Clean up allocated memory on error
 for (int j = 0; j < count; j++) {
 free(phonemes[j]);
 }
 free(phonemes);
 free(word_copy);
 return NULL;
 }
 strcpy(phonemes[count], phoneme);
 count++;
 }
 } else {
 // Fallback to default handling
 phonemes[count] = malloc(3); // Default 2 character phoneme + null terminator
 if (!phonemes[count]) {
 for (int j = 0; j < count; j++) {
 free(phonemes[j]);
 }
 free(phonemes);
 free(word_copy);
 return NULL;
 }
 strcpy(phonemes[count], "ae");
 count++;
 }
 }
 
 // Null terminate the array
 phonemes[count] = NULL;
 
 *phoneme_count = count;
 free(word_copy);
 return phonemes;
}

/**
 * @brief Convert a word to its diphone sequence using G2P rules with proper memory management
 * 
 * This function converts a single word into a complete diphone sequence by combining the
 * predicted phonemes from convert_word_to_phonemes.
 * 
 * @param word Pointer to null-terminated string containing the word to process
 * @return Pointer to allocated string containing the full diphone sequence, or NULL on error
 */
char* convert_word_diphone_sequence(char* word) {
 if (!word || strlen(word) == 0) return NULL;
 
 int phoneme_count = 0;
 char** phonemes = convert_word_to_phonemes(word, &phoneme_count);
 
 if (!phonemes || phoneme_count <= 0) {
 // Try to find in dictionary as fallback
 WordEntry* entry = find_word_entry(word);
 if (entry) {
 size_t len = strlen(entry->diphone_str) + 1;
 char* copy = malloc(len);
 if (copy) {
 memcpy(copy, entry->diphone_str, len);
 }
 return copy;
 }
 return NULL;
 }
 
 // Build diphone sequence from phonemes - FIXED VERSION
 char** diphones = malloc((phoneme_count * 2 - 1) * sizeof(char*));
 if (!diphones) {
 for (int i = 0; i < phoneme_count; i++) {
 free(phonemes[i]);
 }
 free(phonemes);
 return NULL;
 }
 
 // Initialize all pointers to NULL
 for (int i = 0; i < phoneme_count * 2 - 1; i++) {
 diphones[i] = NULL;
 }
 
 // First diphone: pau + first phoneme
 if (phonemes[0]) {
 diphones[0] = malloc(strlen("pau_") + strlen(phonemes[0]) + 1);
 if (!diphones[0]) {
 // Clean up previous allocations
 for (int i = 0; i < phoneme_count * 2 - 1; i++) {
 free(diphones[i]);
 }
 free(diphones);
 for (int i = 0; i < phoneme_count; i++) {
 free(phonemes[i]);
 }
 free(phonemes);
 return NULL;
 }
 sprintf(diphones[0], "pau_%s", phonemes[0]);
 }
 
 // Middle diphones: phoneme + next phoneme
 for (int i = 0; i < phoneme_count - 1; i++) {
 int index = i * 2 + 1;
 if (phonemes[i] && phonemes[i+1]) {
 diphones[index] = malloc(strlen(phonemes[i]) + strlen(phonemes[i+1]) + 2);
 if (!diphones[index]) {
 // Clean up previous allocations
 for (int j = 0; j < index; j++) {
 free(diphones[j]);
 }
 free(diphones);
 for (int j = 0; j < phoneme_count; j++) {
 free(phonemes[j]);
 }
 free(phonemes);
 return NULL;
 }
 sprintf(diphones[index], "%s_%s", phonemes[i], phonemes[i+1]);
 } else {
 const char* fallback_str = "pau_pau";
 size_t len = strlen(fallback_str) + 1;
 diphones[index] = malloc(len);
 if (diphones[index]) {
 memcpy(diphones[index], fallback_str, len);
 }
 }
 }
 
 // Last diphone: last phoneme + pau
 int last_index = phoneme_count * 2 - 2;
 if (phonemes[phoneme_count-1]) {
 diphones[last_index] = malloc(strlen(phonemes[phoneme_count-1]) + strlen("_pau") + 1);
 if (!diphones[last_index]) {
 // Clean up previous allocations
 for (int i = 0; i < last_index; i++) {
 free(diphones[i]);
 }
 free(diphones);
 for (int i = 0; i < phoneme_count; i++) {
 free(phonemes[i]);
 }
 free(phonemes);
 return NULL;
 }
 sprintf(diphones[last_index], "%s_pau", phonemes[phoneme_count-1]);
 }
 
 // Validate diphone sequence
 int valid_diphones = 0;
 for (int i = 0; i < last_index + 1; i++) {
 if (diphones[i]) {
 valid_diphones++;
 }
 }
 
 if (valid_diphones == 0) {
 // Clean up
 for (int i = 0; i < last_index + 1; i++) {
 free(diphones[i]);
 }
 free(diphones);
 for (int i = 0; i < phoneme_count; i++) {
 free(phonemes[i]);
 }
 free(phonemes);
 return NULL;
 }
 
 // Build final result string
 size_t total_length = 0;
 for (int i = 0; i < last_index + 1; i++) {
 if (diphones[i]) {
 total_length += strlen(diphones[i]) + 1; // +1 for space separator
 }
 }
 
 char* result = malloc(total_length);
 if (!result) {
 for (int i = 0; i < last_index + 1; i++) {
 free(diphones[i]);
 }
 free(diphones);
 for (int i = 0; i < phoneme_count; i++) {
 free(phonemes[i]);
 }
 free(phonemes);
 return NULL;
 }
 
 result[0] = '\0';
 
 for (int i = 0; i < last_index + 1; i++) {
 if (diphones[i]) {
 strcat(result, diphones[i]);
 if (i < last_index) {
 strcat(result, " ");
 }
 }
 }
 
 // Clean up
 for (int i = 0; i < last_index + 1; i++) {
 free(diphones[i]);
 }
 free(diphones);
 
 for (int i = 0; i < phoneme_count; i++) {
 free(phonemes[i]);
 }
 free(phonemes);
 
 return result;
}

/**
 * @brief Process a word through the G2P engine with better error handling
 * 
 * This function attempts to find a word in the dictionary first, and if not found,
 * uses the G2P engine to generate phonetic representation.
 * 
 * @param word Pointer to null-terminated string containing the word to process
 * @return Pointer to allocated string containing the diphone sequence, or NULL on error
 */
char* process_g2p_word(char* word) {
 if (!word || strlen(word) == 0) return NULL;
 
 // First try to find in dictionary
 WordEntry* entry = find_word_entry(word);
 if (entry) {
 size_t len = strlen(entry->diphone_str) + 1;
 char* copy = malloc(len);
 if (copy) {
 memcpy(copy, entry->diphone_str, len);
 }
 return copy;
 }
 
 // If not found, use G2P engine with error handling
 char* diphone_sequence = convert_word_diphone_sequence(word);
 
 // If still no result, try a simple fallback approach
 if (!diphone_sequence) {
 // Try to build minimal sequence for unknown words
 size_t len = strlen(word);
 if (len > 0) {
 // Simple fallback - create basic diphone structure
 char* fallback = malloc(len * 16 + 20); // Conservative estimate
 if (fallback) {
strcpy(fallback, "pau_");
// Just use first few characters as phonemes with simple mapping
for (int i = 0; i < (int)len && i < 8; i++) { // Limit to prevent overflow
char c = word[i];
if (c >= 'a' && c <= 'z') {
// Simple character-to-phoneme mapping - not ideal but better than nothing
switch(c) {
case 'a': strcat(fallback, "ae_"); break;
case 'b': strcat(fallback, "b_"); break;
case 'c': strcat(fallback, "k_"); break;
case 'd': strcat(fallback, "d_"); break;
case 'e': strcat(fallback, "eh_"); break;
case 'f': strcat(fallback, "f_"); break;
case 'g': strcat(fallback, "g_"); break;
case 'h': strcat(fallback, "hh_"); break;
case 'i': strcat(fallback, "ih_"); break;
case 'j': strcat(fallback, "jh_"); break;
case 'k': strcat(fallback, "k_"); break;
case 'l': strcat(fallback, "l_"); break;
case 'm': strcat(fallback, "m_"); break;
case 'n': strcat(fallback, "n_"); break;
case 'o': strcat(fallback, "aa_"); break;
case 'p': strcat(fallback, "p_"); break;
case 'q': strcat(fallback, "k_"); break;
case 'r': strcat(fallback, "r_"); break;
case 's': strcat(fallback, "s_"); break;
case 't': strcat(fallback, "t_"); break;
case 'u': strcat(fallback, "ah_"); break;
case 'v': strcat(fallback, "v_"); break;
case 'w': strcat(fallback, "w_"); break;
case 'x': strcat(fallback, "ks_"); break;
case 'y': strcat(fallback, "iy_"); break;
case 'z': strcat(fallback, "z_"); break;
default: strcat(fallback, "ae_"); break;
}
}
}
// Add final pau
if (strlen(fallback) > 4) {
char* last_pos = strrchr(fallback, '_');
if (last_pos) *last_pos = '\0';
}
strcat(fallback, "_pau");
return fallback;
}
}
}
return diphone_sequence;
}

/**
 * @brief  Predict functions 
 * These functions implement context-sensitive phoneme prediction rules for individual characters.
 * Each function handles a specific character and considers its surrounding context to determine
 * the appropriate phoneme representation.
 */

// Context check constants: 0x5F is '_', 0x27 is '''
const char* predictA(char p, char c, char n, char nn, char nnn) {
(void)p; (void)c; (void)n; (void)nn; (void)nnn; // Suppress unused parameter warnings
// 1. Terminal Magic E (Lake, Bake, Fate, Ale)
if (n != 0x5F && nn == 'e' && nnn == 0x5F) return "ey";
// 2. Common Vowel Neighbours
if (n == 'i' || n == 'y') return "ey";
if (n == 'r' || n == 'h' || n == 'w') return "aa";
// 3. The "Alex/Apple" logic
if (n == 'l' || n == 'p' || n == 'c' || n == 'd') return "ae";
// 4. Fallback - handle special cases for common words
if (p == 's' && n == 'u') return "ah"; // "suite"
// 5. Default fallback
return "ae";
}
const char* predictB(char p, char c, char n, char nn, char nnn) {
(void)p; (void)c; (void)n; (void)nn; (void)nnn;
return "b";
}
const char* predictC(char p, char c, char n, char nn, char nnn) {
(void)p; (void)c; (void)n; (void)nn; (void)nnn;
// Handle 'ch' combinations properly
if (n == 'h') return "ch";
// Handle 'ck' endings - common in English words like 'back', 'duck'
if (n == 'k' && nn == 0x5F) {
// Check for vowel before the 'ck' to determine pronunciation
if (p == 'a' || p == 'o' || p == 'u') return "k";
else return "k"; // Default to 'k' sound
}
// Handle 'ci' and 'ce' - usually pronounced as /s/
if ((n == 'i' || n == 'e') && (nn == 0x5F || nn == ' ')) {
return "s";
}
// Handle 'c' before 'u' in words like 'culture'
if (n == 'u' && (nn == 'r' || nn == 'l')) {
return "k"; // Usually 'k' sound
}
// Default case
return "k";
}
const char* predictD(char p, char c, char n, char nn, char nnn) {
(void)p; (void)c; (void)n; (void)nn; (void)nnn;
return "d";
}
const char* predictE(char p, char c, char n, char nn, char nnn) {
(void)p; (void)c; (void)n; (void)nn; (void)nnn;
if (n == 0x5F) return "silent"; // silence
if (n == 0x27) return "ah";
if (n == '(') return "ah";
if (n == 'a') return "iy";
if (n == 'd') return "ah";
if (n == 'e') return "iy";
if (n == 'i') return "er";
if (n == 'o') return "iy";
if (n == 'r') return "er";
if (n == 'u') return "ah";
if (n == 'w') return "ah";
if (n == 'y') return "iy";
// Fix for final 'o' in words like "hello"
if (c == 'o' && n == 0x5F) return "ow";
// Handle case where 'e' should be 'ah' in specific contexts
if (p == 'h' && n == 'l') return "ah";
if (p == 'c') return "ah";
if (p == 'e') return "iy";
if (p == 'g') return "er";
if (p == 'i') return "iy";
if (p == 'k') return "ah";
if (p == 'l') return "ah";
if (p == 'n') return "ah";
if (p == 'o') return "er";
if (p == 't') return "ah";
if (p == 'v') return "er";
if (p == 'y') return "er";
if (p == 'z') return "ah";
if (nn == 0x27) return "er";
if (nn == '(') return "er";
if (nn == '1') return "ah";
if (nn == 'm') return "er";
if (nn == 'n') return "iy";
if (nn == 's') return "ah";
if (nn == 'u') return "ah";
if (nn == 'v') return "er";
if (nn == 'w') return "er";
if (nn == 'y') return "iy";
// Handle special cases for common words
if (p == 's' && n == 't') return "ih"; // "set"
if (c == 'e' && nn == 'r') return "eh"; // "her", "ver"
return "eh";
}
const char* predictF(char p, char c, char n, char nn, char nnn) {
(void)p; (void)c; (void)n; (void)nn; (void)nnn;
return "f";
}
const char* predictG(char p, char c, char n, char nn, char nnn) {
(void)p; (void)c; (void)n; (void)nn; (void)nnn;
// Handle 'gh' combinations properly
if (p == 'h' && n == 'g') return "silent"; // "night" -> silent
if (p == 'g' && n == 'h') {
if (nn != 0x5F && nn != 'e' && nn != 'i') return "hh"; // "ghost"
else return "g";
}
// Handle 'gu' combinations - usually 'g' sound
if (n == 'u' && (nn == 'a' || nn == 'o' || nn == 'u')) {
return "g"; // "guess" -> g sound
}
// Handle 'ge', 'gi' endings - can be /dʒ/ or /ɡ/
if ((n == 'e' || n == 'i') && (nn == 0x5F || nn == ' ')) {
return "g";
}
return "g";
}
const char* predictH(char p, char c, char n, char nn, char nnn) {
(void)p; (void)c; (void)n; (void)nn; (void)nnn;
if (n == 'a') return "hh";
if (n == 'o') return "hh";
if (n == 'u') return "hh";
if (p == 'a') return "hh";
if (p == 'd') return "hh";
if (p == 'e') return "hh";
if (p == 'g') return "hh";
if (p == 'k') return "hh";
if (p == 'l') return "hh";
if (p == 'n') return "hh";
if (p == 'o') return "hh";
if (p == 'r') return "hh";
if (p == 'w') return "hh";
if (nn == 'b') return "hh";
if (nn == 'f') return "hh";
if (nn == 'g') return "hh";
if (nn == 'l') return "hh";
if (nn == 'm') return "hh";
if (nn == 'u') return "hh";
// Handle words that start with 'h' - sometimes silent
if (p == 0x5F && n != 0x5F) {
return "silent"; // This is conservative approach
}
return "hh";
}
const char* predictI(char p, char c, char n, char nn, char nnn) {
(void)p; (void)c; (void)n; (void)nn; (void)nnn;
if (n != 0x5F && nn == 'e' && nnn == 0x5F) return "ay"; // Magic E: Like, Kite
if (n == 'g' && nn == 'h') return "ay"; // Night, Light
// Handle 'i' before 'ng' - common in words like "sing", "ring"
if (n == 'n' && nn == 'g') {
return "ih";
}
return "ih";
}
const char* predictJ(char p, char c, char n, char nn, char nnn) {
(void)p; (void)c; (void)n; (void)nn; (void)nnn;
return "jh";
}
const char* predictK(char p, char c, char n, char nn, char nnn) {
(void)p; (void)c; (void)n; (void)nn; (void)nnn;
// Handle 'kn' combinations - usually silent
if (p == 'n') return "silent";
return "k";
}
const char* predictL(char p, char c, char n, char nn, char nnn) {
(void)p; (void)c; (void)n; (void)nn; (void)nnn;
return "l";
}
const char* predictM(char p, char c, char n, char nn, char nnn) {
(void)p; (void)c; (void)n; (void)nn; (void)nnn;
// Handle 'mn' combinations - usually silent
if (p == 'n') return "silent";
return "m";
}
const char* predictN(char p, char c, char n, char nn, char nnn) {
(void)p; (void)c; (void)n; (void)nn; (void)nnn;
// Handle 'ng' combinations - usually /ŋ/
if (n == 'g') return "ng";
return "n";
}
const char* predictO(char p, char c, char n, char nn, char nnn) {
(void)p; (void)c; (void)n; (void)nn; (void)nnn;
// 1. Terminal Magic E (Quote, Note, Rope) - MUST BE AT TOP
if (n != 0x5F && nn == 'e' && nnn == 0x5F) return "ow";
// 2. Common patterns
if (n == 'r' || n == 'f') return "ao";
if (n == 'u' || n == 'o') return "uw";
// 3. Most common 'o' sound (Hot, Box, Dot)
return "aa";
}
const char* predictP(char p, char c, char n, char nn, char nnn) {
(void)p; (void)c; (void)n; (void)nn; (void)nnn;
// Handle 'ph' combinations - usually /f/
if (n == 'h') return "f";
return "p";
}
const char* predictQ(char p, char c, char n, char nn, char nnn) {
(void)p; (void)c; (void)n; (void)nn; (void)nnn;
return "k"; // We return 'k', and let the 'u' handle the 'w'
}
const char* predictR(char p, char c, char n, char nn, char nnn) {
(void)p; (void)c; (void)n; (void)nn; (void)nnn;
// Handle 'r' at beginning - sometimes silent in some dialects
if (p == 0x5F && n != 0x5F) return "r";
return "r";
}
const char* predictS(char p, char c, char n, char nn, char nnn) {
(void)p; (void)c; (void)n; (void)nn; (void)nnn;
// Handle 'sh' combinations
if (n == 'h') return "sh";
// Handle 'sc' combinations
if (n == 'c') {
if (nn == 'i' || nn == 'e') return "sh"; // "science", "scene"
else return "s"; // "school"
}
// Handle 'ssi', 'sso' endings - /ʃ/ sound
if (p == 's' && n == 's') {
if (nn == 'i' || nn == 'o') return "sh";
}
// Handle 's' before vowels that are pronounced as /z/
if ((p == 'a' || p == 'e' || p == 'i' || p == 'o' || p == 'u') &&
(n == 'a' || n == 'e' || n == 'i' || n == 'o' || n == 'u')) {
return "z";
}
if (p == 0x27) return "z";
if (p == 'd') return "z";
if (p == 'g') return "z";
if (p == 'm') return "z";
if (nn == '1') return "z";
return "s";
}
const char* predictT(char p, char c, char n, char nn, char nnn) {
(void)p; (void)c; (void)n; (void)nn; (void)nnn;
// Handle 'th' combinations - /θ/ or /ð/
if (n == 'h') return "th";
// Handle 'tch' combinations
if (n == 'c' && nn == 'h') {
return "ch";
}
// Handle 'tt' combinations - often pronounced as single sound
if (n == 't') {
return "t"; // Usually just one 't'
}
return "t";
}
const char* predictU(char p, char c, char n, char nn, char nnn) {
(void)p; (void)c; (void)n; (void)nn; (void)nnn;
if (p == 'q') return "w"; // Qu -> w
if (n != 0x5F && nn == 'e' && nnn == 0x5F) return "uw"; // Magic E: Mute, Tube
// Handle 'ue' endings
if (n == 'e' && nn == 0x5F && p != 'q') {
return "u";
}
return "ah"; // But, Cut
}
const char* predictV(char p, char c, char n, char nn, char nnn) {
(void)p; (void)c; (void)n; (void)nn; (void)nnn;
return "v";
}
const char* predictW(char p, char c, char n, char nn, char nnn) {
(void)p; (void)c; (void)n; (void)nn; (void)nnn;
// Handle 'wh' combinations - /w/ sound
if (n == 'h') return "w";
return "w";
}
const char* predictX(char p, char c, char n, char nn, char nnn) {
(void)p; (void)c; (void)n; (void)nn; (void)nnn;
// Handle 'x' - usually /ks/ sound
return "ks";
}
const char* predictY(char p, char c, char n, char nn, char nnn) {
(void)p; (void)c; (void)n; (void)nn; (void)nnn;
if (n == 0x5F) return "iy"; // 'y' at the end of a word (happy) -> iy
if (p == 0x5F) return "y"; // 'y' at the start (yellow) -> y
// Handle 'y' before vowels - often /ɪ/ sound
if ((n == 'a' || n == 'e' || n == 'i' || n == 'o' || n == 'u') && nn != 0x5F) {
return "iy";
}
return "ay"; // 'y' in the middle (fly/cycle) -> ay
}
const char* predictZ(char p, char c, char n, char nn, char nnn) {
(void)p; (void)c; (void)n; (void)nn; (void)nnn;
// Handle 'zz' combinations - double z sound
if (n == 'z') return "z";
return "z";
}
