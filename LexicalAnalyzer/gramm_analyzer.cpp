  //
//  gramm_analyzer.cpp
//  LexicalAnalyzer
//
//  Created by Ivan on 4/13/16.
//  Copyright © 2016 Ivan Tkachenko. All rights reserved.
//

#include "gramm_analyzer.hpp"

void gram_analyze(std::vector<std::vector<std::string>>& vect) {
    std::ofstream file;
    file.open("/Users/ivan/Developer/uniCode/ASM_2/LexAnalyzer/LexicalAnalyzer/LexicalAnalyzer/gram.txt");
    
    std::vector<std::string> user_id;
    std::vector<int> offset;
    int i = 0;
    
    while(i < vect.size()) {
        bool segment_check_flag = segment_check(vect, user_id, offset, i);
        std::vector<std::string> tmp_str = vect.at(i);
        for (int j = 0; j < tmp_str.size(); j++) {
            printf("%s ", tmp_str[j].c_str());
        }
        printf("\n");
        std::string str = tmp_str[0];
        if (segment_check_flag) {
            for (int j = 0; j < offset.size(); ++j) {
                std::vector<std::string> tmp = vect[j];
                file << "000" << std::hex << offset[j] << "\t";
                copy(tmp.begin(), tmp.end(), std::ostream_iterator<std::string>(file, "\t"));
                file << "\n";
                
            }
        } else if(str.compare("ASSUME") == 0) {
            file << "\t";
            copy(tmp_str.begin(), tmp_str.end(), std::ostream_iterator<std::string>(file, "\t"));
            file << "\n";
            if ((i + 1) <= vect.size())
                i++;
        } else {
            tmp_str.clear();
            if ((i + 1) <= vect.size())
                i++;
        }
        offset.clear();
    }
    
    file.close();
}

bool segment_check(std::vector<std::vector<std::string>>& vector, std::vector<std::string>& usr_id, std::vector<int>& offset, int& index) {
    
    if (valid_segment_name(vector[index])) {
        offset.push_back(0);
        offset.push_back(0);
        if ((index + 1) < vector.size())
            index++;
        
        while (mnem_check(vector[index], usr_id, offset, index)) {
            if ((index + 1) < vector.size())
                index++;
        }
        
    } else {
        if ((index + 1) < vector.size())
            index++;
        return false;
    }
    
    return true;
}

bool valid_segment_name(std::vector<std::string> sentence) {
    if (sentence.size() == 2 && id_check(sentence[0])) {
        if (!sentence[1].compare("SEGMENT")) {
            return true;
        }
    }
    
    return false;
}

bool id_check(std::string str) {
    if (!str.empty() && str.length() <= max_lexems_length) {
        for (int i = 0; i < str.length(); ++i)
            if ((!isalpha(str[i]) || !isdigit(str[i])) && islower(str[i]))
                return false;
        return true;
    } else {
        return false;
    }
}

bool mnem_check(std::vector<std::string>& vector, std::vector<std::string>& usr_id, std::vector<int>& offset, int& indx) {
    std::string type_array[] = {"DB", "DD", "DW"};
    int last_elem_size = offset[offset.size() - 1];
    if (vector.size() >= 3) {
        usr_id.push_back(vector[0]);
        if (vector[1] == type_array[0]) {
            if (!vector[2].compare("'") && !vector[vector.size() - 1].compare("'")) {
                offset.push_back((int)(last_elem_size + vector[3].size()));
            } else {
                offset.push_back((int)(last_elem_size + 1));
            }
            return true;
        } else if(vector[1] == type_array[1]) {
            offset.push_back((last_elem_size + 4));
            return true;
        } else if(vector[1] == type_array[2]) {
            offset.push_back((last_elem_size + 2));
            return true;
        } else {
            return false;
        }
    }
    
    return false;
}