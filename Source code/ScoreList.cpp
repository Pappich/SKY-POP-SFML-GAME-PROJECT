#include "ScoreList.h"

ScoreList::ScoreList(std::string file_name) {

    this->file_name = file_name;
}

/**
 * สำหรับโหลดข้อมูลจากไฟล์เก็บไว้ในรายการ
 * 
 * @return bool สำเร็จหรือไม่
 */
bool ScoreList::loadFile() {

    std::ifstream file_read(this->file_name);

    std::string read_string;
    std::string read_name;
    std::string read_score;

    bool is_read_name = true;
    bool is_load_success = false;
    
    while(std::getline(file_read, read_string)) {
        
        if(is_read_name) {
            
            read_name = read_string;
        }
        else { 

            read_score = read_string;

            unsigned long read_score_int;
            std::stringstream ss;
            ss << read_score;
            ss >> read_score_int;

            Score score(read_name, read_score_int);
            this->score_list.push_back(score);
        }

        is_read_name = !is_read_name;

        is_load_success = true;
    }

    file_read.close();

    if(is_load_success) {
        
        std::cout << SUCCESS << "- Loaded " << this->score_list.size() << " entry(s) from \'" << this->file_name << "\' successfully!" << CLEAR << std::endl;
        return true;
    }
    std::cout << WARN << "- Load entry(s) failed, file or data not found." << CLEAR << std::endl;
    return false;
}   

/**
 * สำหรับบันทึกข้อมูลจากรายการลงในไฟล์
 */
void ScoreList::saveFile() {

    std::ofstream file_save(this->file_name);

    for(auto entry : this->score_list) {

        file_save << entry.getName() << std::endl;
        file_save << entry.getScore() << std::endl;
    }
    file_save.close();

    std::cout << SUCCESS << "- Saved " << this->score_list.size() << " entry(s) to \'" << this->file_name << "\' successfully." << CLEAR << std::endl;
}

/**
 * สำหรับเพิ่มข้อมูลคะแนนลงในรายการ โดยเรียงจากมากไปน้อย
 * 
 * @param Score ข้อมูลคะแนน
 * 
 * @return bool สำเร็จหรือไม่
 */
bool ScoreList::addEntry(Score player) {

    return this->addScoreEntry(player);
}

/**
 * สำหรับเพิ่มข้อมูลคะแนนลงในรายการ โดยเรียงจากมากไปน้อย
 * 
 * @param string ชื่อ
 * @param int คะแนน
 * 
 * @return bool สำเร็จหรือไม่
 */
bool ScoreList::addEntry(std::string name, unsigned long score) {

    return this->addScoreEntry(Score(name, score));
}

bool ScoreList::addScoreEntry(Score player) {

    bool is_update = false;
    for(auto entry = this->score_list.begin(); entry != this->score_list.end(); entry++) {
        
        if (player.getName() == entry->getName() && player.getScore() < entry->getScore()) {

            std::cout << ERROR << "- Failed to add \'" << player.getName() <<  "\' entry, found duplicate name and not a highscore." << CLEAR << std::endl;
            
            return false;
        }
        if (player.getName() == entry->getName() && player.getScore() != entry->getScore()) {

            score_list.erase(entry);
            is_update = true;
            
            break;
        }
    }

	for (auto entry = this->score_list.begin(); entry != this->score_list.end(); entry++) {

		if (player.getScore() == entry->getScore() && player.getName() == entry->getName()) {

            std::cout << ERROR << "- Failed to add \'" << player.getName() <<  "\' entry, found duplicate name and score." << CLEAR << std::endl;
            
            return false;
        }
		
        if (player.getScore() > entry->getScore()) {
			
            score_list.insert(entry, player);

            if(!is_update) std::cout << SUCCESS << "- Added \'" << player.getName() << "\' entry with a score of " << player.getScore() << " points." << CLEAR << std::endl;
            else std::cout << WARN << "- Updated \'" << player.getName() << "\' with a score of " << player.getScore() << " points." << CLEAR << std::endl;
			
            return true;
		}
	}
    
	score_list.push_back(player);

    if(!is_update) std::cout << SUCCESS << "- Added \'" << player.getName() << "\' with a score of " << player.getScore() << " points." << CLEAR << std::endl;
    else std::cout << WARN << "- Updated \'" << player.getName() << "\' score to " << player.getScore() << " points." << CLEAR << std::endl;

    return true;
}

/**
 * พิมพ์ข้อมูลในรายการออกมาเป็นตาราง
 * 
 * @param จำนวนข้อมูลที่ต้องการพิมพ์ (default : 0 พิมพ์ทั้งหมด)
 */ 
void ScoreList::printEntry(unsigned long max_entry) {

    if(!this->score_list.size()) {

        std::cout << ERROR << "- Cannot print score list because the list was empty." << CLEAR << std::endl;
        return;
    }

    unsigned long longest_name_size = 0;
    unsigned long longest_score_size = 0;
    unsigned long longest_number_size = std::to_string(this->score_list.size()).size();
    for(auto entry : this->score_list) {

        if(entry.getName().size() > longest_name_size) longest_name_size = entry.getName().size();
        if(std::to_string(entry.getScore()).size() > longest_score_size) longest_score_size = std::to_string(entry.getScore()).size();
    }

    for(unsigned long i = 0; i < std::max(4 + longest_name_size + 5 + longest_score_size - (int)this->file_name.size(), 4 + longest_name_size + 5 + longest_score_size); i++) {
        if(i == 3) std::cout << " " << this->file_name << " ";
        if(i == 0 || i == std::max(4 + longest_name_size + 5 + longest_score_size - (int)this->file_name.size(), 4 + longest_name_size + 5 + longest_score_size) - 1) std::cout << "*";
        else std::cout << "=";
    }
    std::cout << std::endl;

    for(unsigned long entry = 0; entry < this->score_list.size(); entry++) {

        if(entry >= max_entry && max_entry != 0) break;

        std::cout << " ";
        std::cout << "-";
        std::cout << " ";

        std::cout << entry + 1;
        for(unsigned long i = std::to_string(entry + 1).size(); i < longest_number_size + 2; i++) std::cout << " ";

        std::cout << this->score_list.at(entry).getName();
        for(unsigned long i = this->score_list.at(entry).getName().size(); i < longest_name_size + 5; i++) std::cout << " ";

        std::cout << this->score_list.at(entry).getScore();
        std::cout << std::endl;
    }

    for(unsigned long i = 0; i < std::max(4 + longest_name_size + 5 + longest_score_size - (int)this->file_name.size(), 4 + longest_name_size + 5 + longest_score_size); i++) {
        if(i == 3) for(unsigned long i = 0; i < this->file_name.size() + 2; i++) std::cout << "=";
        if(i == 0 || i == std::max(4 + longest_name_size + 5 + longest_score_size - (int)this->file_name.size(), 4 + longest_name_size + 5 + longest_score_size) - 1) std::cout << "*";
        else std::cout << "=";
    }
    std::cout << std::endl;
}

/**
 * สำหรับดึงค่าข้อมูลรายการ
 * 
 * @return vector<Score>
 */
std::vector <Score> ScoreList::get() {

    return this->score_list;
}

/**
 * สำหรับเซ็ตค่ารายการ
 * 
 * @param vector<Score>
 */
void ScoreList::set(std::vector <Score> score_list) {

    this->score_list = score_list;
}

/**
 * ล้างข้อมูลในรายการทั้งหมด
 * 
 * @return int จำนวนรายการที่ลบ
 */ 
unsigned long ScoreList::clear() {

    std::cout << ERROR << "- Removed " << this->score_list.size() << " entry(s)." << CLEAR << std::endl;

    unsigned long removed_entry = this->score_list.size();

    this->score_list.clear();

    return removed_entry;
}

/**
 * ลบข้อมูลที่ตำแหน่งหนึ่งในรายการ
 * 
 * @param int ลำดับข้อมูลที่ต้องการลบ
 */
void ScoreList::removeEntry(unsigned long index) {

    if(!this->score_list.size()) return;

    if(index >= this->score_list.size()) {
        
        std::cout << ERROR << "- Error, trying to remove an entry that didn't exists!" << CLEAR << std::endl;
        throw "- Error : Trying to reach an remove that didn't exists!";
    }

    this->score_list.erase(this->score_list.begin() + index);
}

/**
 * ดึงข้อมูลจากในรายการที่ต้องการโดยใส่ลำดับที่ต้องการ
 * 
 * @param int ลำดับข้อมูลที่ต้องการ
 * 
 * @return Score
 */
Score ScoreList::getEntry(unsigned long index) {

    if(index >= this->score_list.size()) {
        
        std::cout << ERROR << "- Error, trying to reach an entry that didn't exists!" << CLEAR << std::endl;
        throw "- Error : Trying to reach an entry that didn't exists!";
    }

    return this->score_list.at(index);
}

/**
 * ดึงชื่อไฟล์ที่บันทึก
 * 
 * @return string ชื่อไฟล์ที่บันทึก
 */
std::string ScoreList::getFileName() {

    return this->file_name;
}