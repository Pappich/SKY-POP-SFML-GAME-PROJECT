#include "Score.h"

Score::Score(std::string name, unsigned long score) {

    this->name = name;
    this->score = score;
}

/**
 * เซ็ตชื่อ
 * 
 * @param string
 */
void Score::setName(std::string name) {

    this->name = name;
}

/**
 * ดึงชื่อ
 * 
 * @return string
 */
std::string Score::getName() {

    return this->name;
}

/**
 * เซ็ตคะแนน
 * 
 * @param int
 */
void Score::setScore(unsigned long score) {

    this->score = score;
}

/**
 * ดึงคะแนน
 * 
 * @return int
 */
unsigned long Score::getScore() {

    return this->score;
}