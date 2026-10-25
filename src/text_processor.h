/* text_processor.h
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
 * @file text_processor.h
 * @brief Header file for text preprocessing functions in Dippo.
 * 
 * This header file contains declarations and prototypes for the text processing
 * functions used by Dippo to convert numbers into words and handle various formats
 * before speech synthesis.
 */

#ifndef TEXT_PROCESSOR_H
#define TEXT_PROCESSOR_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
char* preprocess_text(const char* input);

#endif // TEXT_PROCESSOR_H

