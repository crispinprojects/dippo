/*
 * Copyright 2026 Alan Crispin <crispinalan@gmail.com>
 * SPDX-License-Identifier: GPL-2.0-or-later
 */


#ifndef TEXT_PROCESSOR_H
#define TEXT_PROCESSOR_H

#include <string>

/**
 * @brief Preprocess input text by handling ordinals and time formats
 *
 * This function performs text preprocessing, including:
 * - Converting ordinal numbers to words (1st, 2nd, etc.)
 * - Converting cardinal numbers to words (1st, 2nd, etc.)
 * - removing punctuations
 *
 * @param input The raw text string to be processed
 * @return Processed text as a std::string
 */
std::string preprocess_text(const std::string& input);

#endif // TEXT_PROCESSOR_H
