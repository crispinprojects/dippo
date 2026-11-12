 /*
  * Copyright 2026 Alan Crispin <crispinalan@gmail.com>
  * SPDX-License-Identifier: GPL-2.0-or-later
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
