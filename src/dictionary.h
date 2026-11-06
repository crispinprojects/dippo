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
#ifndef DICTIONARY_H
#define DICTIONARY_H

#include <string>
#include <vector>
#include <map>
#include <ctime>

class Dictionary {
public:
    explicit Dictionary();

    std::string getTimePhrase(int hour, int minute, bool isAllDay);
    std::string getDatePhrase(const std::tm& date);
    std::string getOrdinalStr(int day);
    std::string getCardinalStr(int number);

    std::vector<std::string> transcribeWordDecisionTree(const std::string& word);

private:
    std::map<std::string, std::vector<std::string>> m_data;
    std::map<char, std::string> m_consonantMap;
    std::map<std::string, std::string> m_vowelClusterMap;
    std::map<char, std::string> m_vowelMap;

    std::vector<std::string> buildDiphoneList(const std::vector<std::string>& phonemes);

    //Decision tree
    //c=current, p=previous, n=next nn=nextnext nnn=nextnextnext
    // nn and nnn are LOOK-AHEAD

    std::string predictA(char p, char c, char n, char nn, char nnn);
    std::string predictB(char p, char c, char n, char nn, char nnn);
    std::string predictC(char p, char c, char n, char nn, char nnn);
    std::string predictD(char p, char c, char n, char nn, char nnn);
    std::string predictE(char p, char c, char n, char nn, char nnn);
    std::string predictF(char p, char c, char n, char nn, char nnn);
    std::string predictG(char p, char c, char n, char nn, char nnn);
    std::string predictH(char p, char c, char n, char nn, char nnn);
    std::string predictI(char p, char c, char n, char nn, char nnn);
    std::string predictJ(char p, char c, char n, char nn, char nnn);
    std::string predictK(char p, char c, char n, char nn, char nnn);
    std::string predictL(char p, char c, char n, char nn, char nnn);
    std::string predictM(char p, char c, char n, char nn, char nnn);
    std::string predictN(char p, char c, char n, char nn, char nnn);
    std::string predictO(char p, char c, char n, char nn, char nnn);
    std::string predictP(char p, char c, char n, char nn, char nnn);
    std::string predictQ(char p, char c, char n, char nn, char nnn);
    std::string predictR(char p, char c, char n, char nn, char nnn);
    std::string predictS(char p, char c, char n, char nn, char nnn);
    std::string predictT(char p, char c, char n, char nn, char nnn);
    std::string predictU(char p, char c, char n, char nn, char nnn);
    std::string predictV(char p, char c, char n, char nn, char nnn);
    std::string predictW(char p, char c, char n, char nn, char nnn);
    std::string predictX(char p, char c, char n, char nn, char nnn);
    std::string predictY(char p, char c, char n, char nn, char nnn);
    std::string predictZ(char p, char c, char n, char nn, char nnn);

    void initializeLexicon();
    void setupG2PRules();
};

#endif // DICTIONARY_H
