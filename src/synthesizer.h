/* synthesizer.h
 *
 * Copyright 2026 Alan Crispin <crispinalan@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * SPDX-License-Identifier: GNU Lesser General Public License v2.1
 */
#ifndef SYNTHESIZER_H
#define SYNTHESIZER_H

#include <string>
#include <vector>
#include <cstdint>

class Dictionary;

class Synthesizer {
public:
    explicit Synthesizer();

    // The main vocoder logic
    std::vector<int16_t> vocoder(const std::vector<std::string> &diphones, float tempoFactor);

    void speak(const std::string &text, Dictionary *dict, float tempo);

private:
    // Internal helper for WAV files
    void writeWavHeader(FILE *file, int dataSize);
    void saveAndPlay(const std::vector<int16_t> &buffer);
};

#endif // SYNTHESIZER_H
