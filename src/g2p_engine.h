/* g2p_engine.h
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
 * @file g2p_engine.h
 * @brief Header file for grapheme-to-phoneme (G2P) engine in Dippo.
 * 
 * This header file contains function prototypes and declarations for the G2P engine,
 * which converts words to their phonetic representations using rule-based prediction.
 */

#ifndef G2P_ENGINE_H
#define G2P_ENGINE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * @brief Predict phoneme for character 'a' with context
 * 
 * This function predicts the phoneme representation of the letter 'a' based on its
 * surrounding characters in a word.
 * 
 * @param p Previous character (or 0x5F if at start)
 * @param c Current character ('a')
 * @param n Next character (or 0x5F if at end)
 * @param nn Character after next (or 0x5F if at end)
 * @param nnn Character two positions after next (or 0x5F if at end)
 * @return Pointer to predicted phoneme string
 */
const char* predictA(char p, char c, char n, char nn, char nnn);

/**
 * @brief Predict phoneme for character 'b' with context
 * 
 * This function predicts the phoneme representation of the letter 'b' based on its
 * surrounding characters in a word.
 * 
 * @param p Previous character (or 0x5F if at start)
 * @param c Current character ('b')
 * @param n Next character (or 0x5F if at end)
 * @param nn Character after next (or 0x5F if at end)
 * @param nnn Character two positions after next (or 0x5F if at end)
 * @return Pointer to predicted phoneme string
 */
const char* predictB(char p, char c, char n, char nn, char nnn);

/**
 * @brief Predict phoneme for character 'c' with context
 * 
 * This function predicts the phoneme representation of the letter 'c' based on its
 * surrounding characters in a word.
 * 
 * @param p Previous character (or 0x5F if at start)
 * @param c Current character ('c')
 * @param n Next character (or 0x5F if at end)
 * @param nn Character after next (or 0x5F if at end)
 * @param nnn Character two positions after next (or 0x5F if at end)
 * @return Pointer to predicted phoneme string
 */
const char* predictC(char p, char c, char n, char nn, char nnn);

/**
 * @brief Predict phoneme for character 'd' with context
 * 
 * This function predicts the phoneme representation of the letter 'd' based on its
 * surrounding characters in a word.
 * 
 * @param p Previous character (or 0x5F if at start)
 * @param c Current character ('d')
 * @param n Next character (or 0x5F if at end)
 * @param nn Character after next (or 0x5F if at end)
 * @param nnn Character two positions after next (or 0x5F if at end)
 * @return Pointer to predicted phoneme string
 */
const char* predictD(char p, char c, char n, char nn, char nnn);

/**
 * @brief Predict phoneme for character 'e' with context
 * 
 * This function predicts the phoneme representation of the letter 'e' based on its
 * surrounding characters in a word.
 * 
 * @param p Previous character (or 0x5F if at start)
 * @param c Current character ('e')
 * @param n Next character (or 0x5F if at end)
 * @param nn Character after next (or 0x5F if at end)
 * @param nnn Character two positions after next (or 0x5F if at end)
 * @return Pointer to predicted phoneme string
 */
const char* predictE(char p, char c, char n, char nn, char nnn);

/**
 * @brief Predict phoneme for character 'f' with context
 * 
 * This function predicts the phoneme representation of the letter 'f' based on its
 * surrounding characters in a word.
 * 
 * @param p Previous character (or 0x5F if at start)
 * @param c Current character ('f')
 * @param n Next character (or 0x5F if at end)
 * @param nn Character after next (or 0x5F if at end)
 * @param nnn Character two positions after next (or 0x5F if at end)
 * @return Pointer to predicted phoneme string
 */
const char* predictF(char p, char c, char n, char nn, char nnn);

/**
 * @brief Predict phoneme for character 'g' with context
 * 
 * This function predicts the phoneme representation of the letter 'g' based on its
 * surrounding characters in a word.
 * 
 * @param p Previous character (or 0x5F if at start)
 * @param c Current character ('g')
 * @param n Next character (or 0x5F if at end)
 * @param nn Character after next (or 0x5F if at end)
 * @param nnn Character two positions after next (or 0x5F if at end)
 * @return Pointer to predicted phoneme string
 */
const char* predictG(char p, char c, char n, char nn, char nnn);

/**
 * @brief Predict phoneme for character 'h' with context
 * 
 * This function predicts the phoneme representation of the letter 'h' based on its
 * surrounding characters in a word.
 * 
 * @param p Previous character (or 0x5F if at start)
 * @param c Current character ('h')
 * @param n Next character (or 0x5F if at end)
 * @param nn Character after next (or 0x5F if at end)
 * @param nnn Character two positions after next (or 0x5F if at end)
 * @return Pointer to predicted phoneme string
 */
const char* predictH(char p, char c, char n, char nn, char nnn);

/**
 * @brief Predict phoneme for character 'i' with context
 * 
 * This function predicts the phoneme representation of the letter 'i' based on its
 * surrounding characters in a word.
 * 
 * @param p Previous character (or 0x5F if at start)
 * @param c Current character ('i')
 * @param n Next character (or 0x5F if at end)
 * @param nn Character after next (or 0x5F if at end)
 * @param nnn Character two positions after next (or 0x5F if at end)
 * @return Pointer to predicted phoneme string
 */
const char* predictI(char p, char c, char n, char nn, char nnn);

/**
 * @brief Predict phoneme for character 'j' with context
 * 
 * This function predicts the phoneme representation of the letter 'j' based on its
 * surrounding characters in a word.
 * 
 * @param p Previous character (or 0x5F if at start)
 * @param c Current character ('j')
 * @param n Next character (or 0x5F if at end)
 * @param nn Character after next (or 0x5F if at end)
 * @param nnn Character two positions after next (or 0x5F if at end)
 * @return Pointer to predicted phoneme string
 */
const char* predictJ(char p, char c, char n, char nn, char nnn);

/**
 * @brief Predict phoneme for character 'k' with context
 * 
 * This function predicts the phoneme representation of the letter 'k' based on its
 * surrounding characters in a word.
 * 
 * @param p Previous character (or 0x5F if at start)
 * @param c Current character ('k')
 * @param n Next character (or 0x5F if at end)
 * @param nn Character after next (or 0x5F if at end)
 * @param nnn Character two positions after next (or 0x5F if at end)
 * @return Pointer to predicted phoneme string
 */
const char* predictK(char p, char c, char n, char nn, char nnn);

/**
 * @brief Predict phoneme for character 'l' with context
 * 
 * This function predicts the phoneme representation of the letter 'l' based on its
 * surrounding characters in a word.
 * 
 * @param p Previous character (or 0x5F if at start)
 * @param c Current character ('l')
 * @param n Next character (or 0x5F if at end)
 * @param nn Character after next (or 0x5F if at end)
 * @param nnn Character two positions after next (or 0x5F if at end)
 * @return Pointer to predicted phoneme string
 */
const char* predictL(char p, char c, char n, char nn, char nnn);

/**
 * @brief Predict phoneme for character 'm' with context
 * 
 * This function predicts the phoneme representation of the letter 'm' based on its
 * surrounding characters in a word.
 * 
 * @param p Previous character (or 0x5F if at start)
 * @param c Current character ('m')
 * @param n Next character (or 0x5F if at end)
 * @param nn Character after next (or 0x5F if at end)
 * @param nnn Character two positions after next (or 0x5F if at end)
 * @return Pointer to predicted phoneme string
 */
const char* predictM(char p, char c, char n, char nn, char nnn);

/**
 * @brief Predict phoneme for character 'n' with context
 * 
 * This function predicts the phoneme representation of the letter 'n' based on its
 * surrounding characters in a word.
 * 
 * @param p Previous character (or 0x5F if at start)
 * @param c Current character ('n')
 * @param n Next character (or 0x5F if at end)
 * @param nn Character after next (or 0x5F if at end)
 * @param nnn Character two positions after next (or 0x5F if at end)
 * @return Pointer to predicted phoneme string
 */
const char* predictN(char p, char c, char n, char nn, char nnn);

/**
 * @brief Predict phoneme for character 'o' with context
 * 
 * This function predicts the phoneme representation of the letter 'o' based on its
 * surrounding characters in a word.
 * 
 * @param p Previous character (or 0x5F if at start)
 * @param c Current character ('o')
 * @param n Next character (or 0x5F if at end)
 * @param nn Character after next (or 0x5F if at end)
 * @param nnn Character two positions after next (or 0x5F if at end)
 * @return Pointer to predicted phoneme string
 */
const char* predictO(char p, char c, char n, char nn, char nnn);

/**
 * @brief Predict phoneme for character 'p' with context
 * 
 * This function predicts the phoneme representation of the letter 'p' based on its
 * surrounding characters in a word.
 * 
 * @param p Previous character (or 0x5F if at start)
 * @param c Current character ('p')
 * @param n Next character (or 0x5F if at end)
 * @param nn Character after next (or 0x5F if at end)
 * @param nnn Character two positions after next (or 0x5F if at end)
 * @return Pointer to predicted phoneme string
 */
const char* predictP(char p, char c, char n, char nn, char nnn);

/**
 * @brief Predict phoneme for character 'q' with context
 * 
 * This function predicts the phoneme representation of the letter 'q' based on its
 * surrounding characters in a word.
 * 
 * @param p Previous character (or 0x5F if at start)
 * @param c Current character ('q')
 * @param n Next character (or 0x5F if at end)
 * @param nn Character after next (or 0x5F if at end)
 * @param nnn Character two positions after next (or 0x5F if at end)
 * @return Pointer to predicted phoneme string
 */
const char* predictQ(char p, char c, char n, char nn, char nnn);

/**
 * @brief Predict phoneme for character 'r' with context
 * 
 * This function predicts the phoneme representation of the letter 'r' based on its
 * surrounding characters in a word.
 * 
 * @param p Previous character (or 0x5F if at start)
 * @param c Current character ('r')
 * @param n Next character (or 0x5F if at end)
 * @param nn Character after next (or 0x5F if at end)
 * @param nnn Character two positions after next (or 0x5F if at end)
 * @return Pointer to predicted phoneme string
 */
const char* predictR(char p, char c, char n, char nn, char nnn);

/**
 * @brief Predict phoneme for character 's' with context
 * 
 * This function predicts the phoneme representation of the letter 's' based on its
 * surrounding characters in a word.
 * 
 * @param p Previous character (or 0x5F if at start)
 * @param c Current character ('s')
 * @param n Next character (or 0x5F if at end)
 * @param nn Character after next (or 0x5F if at end)
 * @param nnn Character two positions after next (or 0x5F if at end)
 * @return Pointer to predicted phoneme string
 */
const char* predictS(char p, char c, char n, char nn, char nnn);

/**
 * @brief Predict phoneme for character 't' with context
 * 
 * This function predicts the phoneme representation of the letter 't' based on its
 * surrounding characters in a word.
 * 
 * @param p Previous character (or 0x5F if at start)
 * @param c Current character ('t')
 * @param n Next character (or 0x5F if at end)
 * @param nn Character after next (or 0x5F if at end)
 * @param nnn Character two positions after next (or 0x5F if at end)
 * @return Pointer to predicted phoneme string
 */
const char* predictT(char p, char c, char n, char nn, char nnn);

/**
 * @brief Predict phoneme for character 'u' with context
 * 
 * This function predicts the phoneme representation of the letter 'u' based on its
 * surrounding characters in a word.
 * 
 * @param p Previous character (or 0x5F if at start)
 * @param c Current character ('u')
 * @param n Next character (or 0x5F if at end)
 * @param nn Character after next (or 0x5F if at end)
 * @param nnn Character two positions after next (or 0x5F if at end)
 * @return Pointer to predicted phoneme string
 */
const char* predictU(char p, char c, char n, char nn, char nnn);

/**
 * @brief Predict phoneme for character 'v' with context
 * 
 * This function predicts the phoneme representation of the letter 'v' based on its
 * surrounding characters in a word.
 * 
 * @param p Previous character (or 0x5F if at start)
 * @param c Current character ('v')
 * @param n Next character (or 0x5F if at end)
 * @param nn Character after next (or 0x5F if at end)
 * @param nnn Character two positions after next (or 0x5F if at end)
 * @return Pointer to predicted phoneme string
 */
const char* predictV(char p, char c, char n, char nn, char nnn);

/**
 * @brief Predict phoneme for character 'w' with context
 * 
 * This function predicts the phoneme representation of the letter 'w' based on its
 * surrounding characters in a word.
 * 
 * @param p Previous character (or 0x5F if at start)
 * @param c Current character ('w')
 * @param n Next character (or 0x5F if at end)
 * @param nn Character after next (or 0x5F if at end)
 * @param nnn Character two positions after next (or 0x5F if at end)
 * @return Pointer to predicted phoneme string
 */
const char* predictW(char p, char c, char n, char nn, char nnn);

/**
 * @brief Predict phoneme for character 'x' with context
 * 
 * This function predicts the phoneme representation of the letter 'x' based on its
 * surrounding characters in a word.
 * 
 * @param p Previous character (or 0x5F if at start)
 * @param c Current character ('x')
 * @param n Next character (or 0x5F if at end)
 * @param nn Character after next (or 0x5F if at end)
 * @param nnn Character two positions after next (or 0x5F if at end)
 * @return Pointer to predicted phoneme string
 */
const char* predictX(char p, char c, char n, char nn, char nnn);

/**
 * @brief Predict phoneme for character 'y' with context
 * 
 * This function predicts the phoneme representation of the letter 'y' based on its
 * surrounding characters in a word.
 * 
 * @param p Previous character (or 0x5F if at start)
 * @param c Current character ('y')
 * @param n Next character (or 0x5F if at end)
 * @param nn Character after next (or 0x5F if at end)
 * @param nnn Character two positions after next (or 0x5F if at end)
 * @return Pointer to predicted phoneme string
 */
const char* predictY(char p, char c, char n, char nn, char nnn);

/**
 * @brief Predict phoneme for character 'z' with context
 * 
 * This function predicts the phoneme representation of the letter 'z' based on its
 * surrounding characters in a word.
 * 
 * @param p Previous character (or 0x5F if at start)
 * @param c Current character ('z')
 * @param n Next character (or 0x5F if at end)
 * @param nn Character after next (or 0x5F if at end)
 * @param nnn Character two positions after next (or 0x5F if at end)
 * @return Pointer to predicted phoneme string
 */
const char* predictZ(char p, char c, char n, char nn, char nnn);

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
char* build_diphone_list(const char** phonemes, int phoneme_count);

/**
 * @brief Convert a word to its diphone sequence using G2P rules 
 * 
 * This function converts a single word into a complete diphone sequence by combining the
 * predicted phonemes from convert_word_to_phonemes.
 * 
 * @param word Pointer to null-terminated string containing the word to process
 * @return Pointer to allocated string containing the full diphone sequence, or NULL on error
 */
char* convert_word_diphone_sequence(char* word);

/**
 * @brief Process a word through the G2P engine
 * 
 * This function attempts to find a word in the dictionary first, and if not found,
 * uses the G2P engine to generate phonetic representation.
 * 
 * @param word Pointer to null-terminated string containing the word to process
 * @return Pointer to allocated string containing the diphone sequence, or NULL on error
 */
char* process_g2p_word(char* word);

#endif // G2P_ENGINE_H
