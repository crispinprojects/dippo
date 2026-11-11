/* text_processor.h */

#ifndef TEXT_PROCESSOR_H
#define TEXT_PROCESSOR_H

#include <string>

/**
 * @brief Preprocess input text by handling ordinals and time formats
 *
 * This function performs comprehensive text preprocessing, including:
 * - Converting ordinal numbers to words (1st, 2nd, etc.)
 * - Converting numeric times into word representations (153 -> fifteen fifty three)
 * - Handling zero-prefixed numbers like "01", "02"
 * - Converting regular numbers to their word equivalents
 *
 * @param input The raw text string to be processed
 * @return Processed text as a std::string
 */
std::string preprocess_text(const std::string& input);

#endif // TEXT_PROCESSOR_H
