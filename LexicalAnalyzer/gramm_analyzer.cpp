//
//  gramm_analyzer.cpp
//  LexicalAnalyzer
//
//  Created by Ivan on 4/13/16.
//  Copyright © 2016 Ivan Tkachenko. All rights reserved.
//

#include "gramm_analyzer.hpp"

const std::string FILE_PATH_GRAM = "/Users/ivan/Developer/uniCode/ASM_2/LexAnalyzer/LexicalAnalyzer/LexicalAnalyzer/gram.txt";

// Grammatic analysis
void gram_analyze(std::vector<std::vector<std::string>>& vect) {
    std::ofstream file;
    file.open(FILE_PATH_GRAM);
    const int size_changer = 8;
    std::vector<user_id_type> user_id;
    std::vector<label_type> label_container;
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
            for (int j = i; j < offset.size(); ++j) {
                std::vector<std::string> tmp = vect[j];
                file << "000" << std::hex << offset[j] << "\t";
                copy(tmp.begin(), tmp.end(), std::ostream_iterator<std::string>(file, "\t"));
                file << "\n";
                copy(tmp.begin(), tmp.end(), std::ostream_iterator<std::string>(std::cout, "\t"));
                printf("\n");
                if (!tmp[1].compare("ENDS")) {
                    user_id[0].value_in_offset = offset[j];
                }
                
            }
            if (i < size_changer)
                i += offset.size();
            else
                i++;
            
        } else if(str.compare("ASSUME") == 0) {
            file << "\t";
            file << tmp_str[0] << "  " << tmp_str[1] << tmp_str[2] << tmp_str[3] << tmp_str[4]
                 << "  " << tmp_str[5] << tmp_str[6] << tmp_str[7];
            file << "\n";
            if ((i + 1) <= vect.size())
                i++;
        } else if(label_check(vect[i], offset, label_container, user_id)) {
            int last_elem_size = offset[offset.size() - 1];
            if (last_elem_size < 16)
                file << "000";
            else
                file << "00";
            file << std::hex << last_elem_size << "\t";
            copy(tmp_str.begin(), tmp_str.end(), std::ostream_iterator<std::string>(file, ""));
            file << "\n";
            if ((i + 1) <= vect.size())
                i++;
        } else if (!str.compare("END")){
            file << "\t" << "\t";
            copy(tmp_str.begin(), tmp_str.end(), std::ostream_iterator<std::string>(file, "\t"));
            file << "\n";
            if (i + 1 <= vect.size())
                i++;
            for (int i = 0; i < user_id.size(); ++i) {
                if (!user_id[i].name.compare("CODE")) {
                    user_id[i].value_in_offset = offset[offset.size() - 1];
                    break;
                }
            }
        } else {
            if (offset[offset.size() - 1] < 16) {
                file << "000" << std::hex << offset[offset.size() - 1] << "\t";
            } else {
                file << "00" << std::hex << offset[offset.size() - 1] << "\t";
            }
            command_run(vect[i], offset, i, label_container);
            copy(tmp_str.begin(), tmp_str.end(), std::ostream_iterator<std::string>(file, " "));
            file << "\n";
            if ((i + 1) <= vect.size())
                i++;
        }
    }
    information_output(user_id, file);
    file.close();
}

// Segment validation check
bool segment_check(std::vector<std::vector<std::string>>& vector, std::vector<user_id_type>& usr_id, std::vector<int>& offset, int index) {
    if (valid_segment_name(vector[index], usr_id)) {
        offset.push_back(0);
        offset.push_back(0);
        if ((index + 1) < vector.size())
            index++;
        
        while (mnem_check(vector[index], usr_id, offset, index)) {
            if ((index + 1) < vector.size())
                index++;
        }
        
    } else {
        return false;
    }
    
    return true;
}

bool valid_segment_name(std::vector<std::string> sentence, std::vector<user_id_type>& use_id) {
    if (sentence.size() == 2 && id_check(sentence[0])) {
        if (!sentence[1].compare("SEGMENT")) {
            user_id_type tmp;
            tmp.name = sentence[0];
            use_id.push_back(tmp);
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

// Mnemonic validation
bool mnem_check(std::vector<std::string>& vector, std::vector<user_id_type>& usr_id, std::vector<int>& offset, int indx) {
    std::string type_array[] = {"DB", "DD", "DW"};
    user_id_type tmp;
    int last_elem_size = offset[offset.size() - 1];
    if (vector.size() >= 3) {
        tmp.name = vector[0];
        tmp.segment_name = "DATA1";
        if (vector[1] == type_array[0]) {
            tmp.value_in_offset = offset[offset.size() - 1];
            if (!vector[2].compare("'") && !vector[vector.size() - 1].compare("'")) {
                offset.push_back((int)(last_elem_size + vector[3].size()));
            } else {
                offset.push_back((int)(last_elem_size + 1));
            }
            tmp.type = "L BYTE";
            usr_id.push_back(tmp);
            return true;
        } else if(vector[1] == type_array[1]) {
            tmp.value_in_offset = offset[offset.size() - 1];
            offset.push_back((last_elem_size + 4));
            tmp.type = "L DWORD";
            usr_id.push_back(tmp);
            return true;
        } else if(vector[1] == type_array[2]) {
            tmp.value_in_offset = offset[offset.size() - 1];
            offset.push_back((last_elem_size + 2));
            tmp.type = "L WORD";
            usr_id.push_back(tmp);
            return true;
        }
    }
    
    return false;
}

// Label validation
bool label_check(std::vector<std::string> label, std::vector<int>& offset, std::vector<label_type>& lab_cont, std::vector<user_id_type>& use_id) {
    int last_elem_size = offset[offset.size() - 1];
    if (!label[1].compare(":") && id_check(label[0])) {
        label_type n_label;
        n_label.name = label[0];
        n_label.offset_number = offset[offset.size() - 1];
        lab_cont.push_back(n_label);
        offset.push_back(last_elem_size);
        
        user_id_type tmp;
        tmp.name = label[0];
        tmp.segment_name = "CODE";
        tmp.type = "L NEAR";
        tmp.value_in_offset = last_elem_size;
        use_id.push_back(tmp);
        return true;
    }
    return false;
}

// Run commands depending in instruction
void command_run(std::vector<std::string> instuct, std::vector<int>& offset, int indx, std::vector<label_type> lab_cont) {
    std::string instruct_name = instuct[0];
    int last_elem_size = offset[offset.size() - 1];
    if (!instruct_name.compare("MOV"))
        mov_command(instuct, offset, indx);
    else if(!instruct_name.compare("ADD"))
        add_command(instuct, offset, indx);
    else if (!instruct_name.compare("AND"))
        and_command(instuct, offset, indx);
    else if(!instruct_name.compare("OR"))
        or_command(instuct, offset, indx);
    else if (!instruct_name.compare("STOS"))
        stos_command(instuct, offset, indx);
    else if (!instruct_name.compare("WAIT"))
        offset.push_back(last_elem_size + 1);
    else if(!instruct_name.compare("XCHG"))
        xchg_command(instuct, offset, indx);
    else if(!instruct_name.compare("RETN"))
        offset.push_back(last_elem_size + 1);
    else if(!instruct_name.compare("JBE"))
        jbe_command(instuct, offset, indx, lab_cont);
    
}

// "MOV" command algorithm
void mov_command(std::vector<std::string> instr, std::vector<int>& offset, int indx) {
    auto it = lexems.find(instr[1]);
    int last_elem_size = offset[offset.size() - 1];
    if (it != lexems.end()) {
        std::string tmp = it->second;
        auto found = tmp.find("REGISTR 16-BIT");
        if (found != std::string::npos) {
            offset.push_back(last_elem_size + 3);
        } else {
            offset.push_back(last_elem_size + 2);
        }
    }
}

// "ADD" command algorithm
void add_command(std::vector<std::string> instr, std::vector<int>& offset, int indx) {
    auto it = lexems.find(instr[1]);
    int last_elem_size = offset[offset.size() - 1];
    if (it != lexems.end()) {
        std::string tmp = it->second;
        auto found = tmp.find("REGISTR 16-BIT");
        if (found != std::string::npos)
            // For 16 bit registre
            offset.push_back(last_elem_size + 2);
        else
            // For 8 bit registre
            offset.push_back(last_elem_size + 2);
    }
}

// "ADD" command algorithm
void and_command(std::vector<std::string> instr, std::vector<int>& offset, int indx) {
    // Opcode explanation
    // And is 2 byte in offset
    // For db register user_id to offset 7
    // For dw register user_id to offset 8
    // For db register user_id[] to offset 6
    // For dw register user_id[] to offset 7
    // For db register ds:[] to offset 4
    // For dw register ds:[] to offset 5
    // For db register ds:user_id to offset 6
    // For dw register ds:user_id to offset 7
    int last_elem_size = offset[offset.size() - 1];
    auto it = lexems.find(instr[0]);
    for (int i = 1; i < lexems.size(); ++i) {
        it = lexems.find(instr[instr.size() - 1]);
    }
    if (it != lexems.end()) {
        std::string tmp = it->second;
        auto found_one = tmp.find("REGISTR 16-BIT");
        auto found_two = tmp.find("REGISTR 8-BIT");
        if (found_one != std::string::npos) {
            int var = 0;
            for (int i = 0; i < instr.size(); ++i)
                if (!instr[i].compare("["))
                    var = 1;
            
            for (int i = 0; i < instr.size(); ++i) {
                if (!instr[i].compare(":") && var != 1)
                    var = 2;
                else if(!instr[i].compare(":") && var == 1)
                    var = 3;
            }
            
            switch (var) {
                case 0:
                    offset.push_back(last_elem_size + 8);
                    break;
                case 1:
                    offset.push_back(last_elem_size + 7);
                    break;
                case 2:
                    offset.push_back(last_elem_size + 7);
                    break;
                case 3:
                    offset.push_back(last_elem_size + 5);
                    break;
                default:
                    break;
            }
            
        } else if(found_two != std::string::npos) {
            int var = 0;
            for (int i = 0; i < instr.size(); ++i)
                if (!instr[i].compare("["))
                    var = 1;
            
            for (int i = 0; i < instr.size(); ++i) {
                if (!instr[i].compare(":"))
                    var = 2;
                else if(!instr[i].compare(":") && var == 1)
                    var = 3;
            }
        
            switch (var) {
                case 0:
                    offset.push_back(last_elem_size + 7);
                    break;
                case 1:
                    offset.push_back(last_elem_size + 5);
                    break;
                case 2:
                    offset.push_back(last_elem_size + 6);
                    break;
                case 3:
                    offset.push_back(last_elem_size + 4);
                    break;
                default:
                    break;
            }
        }
    }
}

// "OR" command algorithm
void or_command(std::vector<std::string> instr, std::vector<int>& offset, int indx) {
    // Opcode explanation
    // For db user_id to offset 8
    // For dw user_id to offset 9
    // For db user_id[] to offset 7
    // For dw user_id[] to offset 8
    // For db ds:[] to offset 5
    // For dw ds:[] to offset 5
    // For db ds:user_id to offset 7
    // For dw ds:user_id to offset 8
    int last_elem_size = offset[offset.size() - 1];
    int var = -1;
    std::string last_word = instr[instr.size() - 1];
    auto found = last_word.find("B");
    if (found != std::string::npos) {
        // For 8 byte
        for (int i = 0; i < instr.size(); ++i)
            if (!instr[i].compare("["))
                var = 0;
        
        for (int i = 0; i < instr.size(); ++i)
            if (!instr[i].compare(":") && var != 0)
                var = 1;
            else if(!instr[i].compare(":") && var == 0)
                var = 2;
        
        switch (var) {
            case 0:
                offset.push_back(last_elem_size + 7);
                break;
            case 1:
                offset.push_back(last_elem_size + 7);
                break;
            case 2:
                // Changed
                offset.push_back(last_elem_size + 3);
                break;
            default:
                break;
        }
    } else {
        // For 16 byte
        for (int i = 0; i < instr.size(); ++i)
            if (!instr[i].compare("["))
                var = 0;
        
        for (int i = 0; i < instr.size(); ++i)
            if (!instr[i].compare(":") && var != 0)
                var = 1;
            else if(!instr[i].compare(":") && var == 0)
                var = 2;
        
        switch (var) {
            case 0:
                offset.push_back(last_elem_size + 6);
                break;
            case 1:
                offset.push_back(last_elem_size + 8);
                break;
            case 2:
                offset.push_back(last_elem_size + 5);
                break;
            default:
                break;
        }
    }
}

// "STOS" command algorithm
void stos_command(std::vector<std::string> instr, std::vector<int>& offset, int indx) {
    // For user_id[] to offset 2
    // For es:user_id[] to offset 2
    // For es:[] to offset 2
    int last_elem_size = offset[offset.size() - 1];
    offset.push_back(last_elem_size + 1);
}

// "XCHG" command algorithm
void xchg_command(std::vector<std::string> instr, std::vector<int>& offset, int indx) {
    // For db user_id[] to offset 6
    // For dw user_id[] to offset 8
    // For db ds:user_id[] to offset 6
    // For dw ds:user_id[] to offset 6
    // For db ds:[] to offset 4
    // For dw ds:[] to offset 4
    auto it = lexems.find(instr[1]);
    int last_elem_size = offset[offset.size() - 1];
    if (it != lexems.end()) {
        std::string tmp = it->second;
        auto found = tmp.find("REGISTR 16-BIT");
        if (found != std::string::npos) {
            int var = -1;
            for (int i = 0; i < instr.size(); ++i)
                if (!instr[i].compare("["))
                    var = 0;
            
            for (int i = 0; i < instr.size(); ++i)
                if (!instr[i].compare(":")) {
                    if (!instr[i + 1].compare("["))
                        var = 1;
                    else
                        var = 2;
                }
            
    
            switch (var) {
                case 0:
                    offset.push_back(last_elem_size + 8);
                    break;
                case 1:
                    offset.push_back(last_elem_size + 4);
                    break;
                case 2:
                    offset.push_back(last_elem_size + 6);
                    break;
                default:
                    break;
            }
            
        } else {
            int var = -1;
            for (int i = 0; i < instr.size(); ++i)
                if (!instr[i].compare("["))
                    var = 0;
            
            for (int i = 0; i < instr.size(); ++i)
                if (!instr[i].compare(":")) {
                    if (!instr[i + 1].compare("["))
                        var = 1;
                    else
                        var = 2;
                }
            
            switch (var) {
                case 0:
                    offset.push_back(last_elem_size + 5);
                    break;
                case 1:
                    offset.push_back(last_elem_size + 4);
                    break;
                case 2:
                    offset.push_back(last_elem_size + 6);
                    break;
                default:
                    break;
            }
        }
    }
}

// "JBE" command algorithm
void jbe_command(std::vector<std::string> instr, std::vector<int>& offset, int indx, std::vector<label_type> lab_cont) {
    int last_elem_size = offset[offset.size() - 1];
    for (int  i = 0; i < lab_cont.size(); ++i) {
        if (!lab_cont[i].name.compare(instr[1])) {
            int diff_in_offset = last_elem_size - lab_cont[i].offset_number;
            if (diff_in_offset > 0 && diff_in_offset < 128) {
                offset.push_back(last_elem_size + 2);
                return;
            } else if(diff_in_offset >= 128) {
                offset.push_back(last_elem_size + 4);
                return;
            }
        }
    }
    offset.push_back(last_elem_size + 4);
}

void information_output(std::vector<user_id_type> user_id, std::ofstream& file_to_write) {
    int code_position = 0;
    file_to_write << "\n\n";
    for (int i = 0; i < user_id.size(); ++i) {
        if (user_id[i].name.compare("CODE") == 0) {
            code_position = i;
            break;
        }
    }
    
    // Write to File the name of Code Segment
    file_to_write << user_id[code_position].name << "\t\t\t\t" << "32 bit  00"
                  << user_id[code_position].value_in_offset << "  PARA  " << "NONE\n";
    
    // Write to File the name of Data segment
    file_to_write << user_id[0].name << "\t\t\t\t" << "32 bit  000"
    << user_id[0].value_in_offset << "  PARA  " << "NONE\n\n";
    
    file_to_write << "Symbols: \n";
    // Write to file the header of information space
    file_to_write << "\t\tNAME\t\t Type\t Value\t Attr\n";
    
    // Write the user variables
    for (int i = 1; i < user_id.size(); i++) {
        if (i == code_position)
            continue;

        file_to_write << user_id[i].name << "\t\t\t\t" << user_id[i].type << "  000"
                      << user_id[i].value_in_offset << "  " << user_id[i].segment_name << "\n";
    }
    
    file_to_write << "@FILENAME\t\t\t\t" << "TEXT  " << "test\n";
}