
#pragma once

#define CLEAR   "\033[0m"
#define ERROR   "\033[1;31m"
#define SUCCESS "\033[1;32m"
#define WARN    "\033[1;33m"

#include <algorithm>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

#include "Score.h"

class ScoreList 
{
private:

    std::string file_name;
    std::vector <Score> score_list;

    bool addScoreEntry(Score player);

public:

    ScoreList(std::string file_name);
    
    bool loadFile();
    void saveFile();
    
    bool addEntry(Score player);
    bool addEntry(std::string, unsigned long score);
    void removeEntry(unsigned long index);
    Score getEntry(unsigned long index);

    void printEntry(unsigned long max_entry = 0);
    
    std::string getFileName();

    std::vector <Score> get();
    void set(std::vector <Score> score_list);
    unsigned long clear();
};