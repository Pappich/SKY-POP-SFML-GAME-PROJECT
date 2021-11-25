
#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

class Score 
{
private:

    std::string name;
    unsigned long score;
    
public:

    Score(std::string name, unsigned long score);

    void setName(std::string name);
    std::string getName();

    void setScore(unsigned long score);
    unsigned long getScore();
};