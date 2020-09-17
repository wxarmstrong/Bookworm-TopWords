#include "trie.h"
#include <stdlib.h>
#include <time.h>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <map>
#include <regex>
#include <string>
#include <vector>
#include <iomanip>

const int LETTER_FREQ[] = { 557,116,79,187,667,89,130,86,460,35,23,186,111,
                            264,351,123,26,304,269,281,286,81,75,23,144,23 };

const int NUMFREQ = 4976;

const float LETTER_VAL[] = { 1,1.25,1.25,1,1,1.25,1,1.25,1,1.75,1.75,1,
                           1.25,1,1,1.25,1.75,1,1,1,1,1.5,1.5,2,1.5,2};

const float DMG_VAL[] = { 0, 0, 0, 0.5, 0.75, 1, 1.5, 2, 2.75, 3.5, 4.5, 5.5, 6.75, 8, 9.5, 11, 13 };

char randomLetter() {
    int curSum = 0;
    int randVal = rand() % NUMFREQ;
    for (int i = 0; i < 25; i++)
    {
        curSum += LETTER_FREQ[i];
        if (randVal < curSum) return 'A' + i;
    }
    return 'Z';
}

string randomBoard() {
    int numLetter[26];
    for (int i = 0; i < 26; i++)
        numLetter[i] = 0;
    string curString = "";
    while (curString.size() < 16) {
        char curLetter = randomLetter();
        if (numLetter[curLetter - 'A'] < 4)
        {
            numLetter[curLetter - 'A']++;
            curString += curLetter;
        }
    }
    return curString;
}

float getDmg(string word) {
    float weight = 0;
    float dmg;
    for (int i = 0; i < word.size(); i++) {
        char curLetter = word[i];
        weight += LETTER_VAL[curLetter - 'A'];
    }
    int adjWeight = weight;
    if (adjWeight >= 16) dmg = 13;
    else dmg = DMG_VAL[adjWeight];

    return dmg;
}

int main() {
    srand(time(NULL));
    struct Trie* root = createNode();
    struct Trie* verbs = createNode();
    struct Trie* adjectives = createNode();

    ifstream infile("wordlist.txt");
    ofstream outfile("mid.txt");

    ifstream verblist("verbs.txt");
    ifstream adjlist("adjectives.txt");

    string curWord;
    while (infile >> curWord)
    {
        //cout << "Inserting word: " << curWord << endl;
        insert(root, curWord);
    }
    while (verblist >> curWord)
    {
       // cout << "Inserting verb: " << curWord << endl;
        insert(verbs, curWord);
    }
    while (adjlist >> curWord)
    {
        //cout << "Inserting adjective: " << curWord << endl;
        insert(adjectives, curWord);
    }

    const int ITERATIONS = 1000000;
    map<string, int> wordCount;

    for (int i = 0; i < ITERATIONS; i++)
    {
        vector<string> allWords;
        cout << i << endl;
        string board = randomBoard();
        // cout << "Current board: " << board << endl;
        findSubanagrams(root, "", board, allWords);
        float bestDmg = 0;
        vector<string> bestWords;
        for (int i = 0; i < allWords.size(); i++)
        {
            string curWord = allWords[i];
            float curDmg = getDmg(curWord);
            if (search(verbs, curWord))
            {
                curDmg *= 1.625;
                float round = int(4 * curDmg);
                curDmg = round / 4;
            }
            if (curDmg > bestDmg)
                bestDmg = curDmg;
        }

        for (int i = 0; i < allWords.size(); i++)
        {
            string curWord = allWords[i];
            float curDmg = getDmg(curWord);
            if (search(verbs, curWord))
            {
                curDmg *= 1.625;
                float round = int(4 * curDmg);
                curDmg = round / 4;
            }
            if (curDmg == bestDmg)
                bestWords.push_back(curWord);
        }

        for (int i = 0; i < bestWords.size(); i++)
        {
            curWord = bestWords[i];
            if (wordCount.find(curWord) == wordCount.end())
                wordCount[curWord] = 1;
            else
                wordCount[curWord]++;
     //       cout << curWord << ": " << wordCount[curWord] << endl;
            outfile << curWord << endl;
        }

    }

    infile.close();
    outfile.close();

    regex wordRgx("\\w+");
    map<string, int> freq;
    string line;

    ifstream in("mid.txt");
    ofstream out("results.txt");
    if (!in.is_open()) {
        cerr << "Failed to open file\n";
        return 1;
    }
    while (getline(in, line)) {
        auto words_itr = sregex_iterator(line.cbegin(), line.cend(), wordRgx);
        auto words_end = sregex_iterator();
        while (words_itr != words_end) {
            auto match = *words_itr;
            auto word = match.str();
            if (word.size() > 0) {
                auto entry = freq.find(word);
                if (entry != freq.end()) {
                    entry->second++;
                }
                else {
                    freq.insert(make_pair(word, 1));
                }
            }
            words_itr = next(words_itr);
        }
    }
    in.close();

    vector<pair<string, int>> pairs;
    for (auto iter = freq.cbegin(); iter != freq.cend(); ++iter) {
        pairs.push_back(*iter);
    }
    sort(pairs.begin(), pairs.end(), [=](pair<string, int>& a, pair<string, int>& b) {
        return a.second > b.second;
        });

    cout << " Rank              Word   Freq\n";
    cout << "=====  ================  =====\n";
    int rank = 1;
    for (auto iter = pairs.cbegin(); iter != pairs.cend() && rank <= 10; ++iter) {
        out << setw(17) << iter->first.c_str() << setw(6) << iter->second << endl;
    }

    return 0;
}