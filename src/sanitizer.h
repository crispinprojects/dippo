/*
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

#ifndef SANITIZER_H
#define SANITIZER_H

#include <string>

/**
 * @brief Preprocess input text by handling ordinals and time formats
 *
 * This function performs comprehensive text preprocessing, including:
 * - Converting ordinal numbers to words (1st, 2nd, etc.)
 * - Converting numeric times into word representations
 * - Converting regular numbers to their word equivalents
 *
 * @param input The raw text string to be processed
 * @return Processed text as a std::string
 */
std::string preprocess_text(const std::string& input);

#endif // TEXT_PROCESSOR_H
