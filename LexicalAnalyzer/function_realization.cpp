/*
**  function_realization.cpp
**  LexicalAnalyzer
**  Created by Ivan on 4/13/16.
**  Copyright © 2016 Ivan Tkachenko. All rights reserved.
*/

#include "function_realization.hpp"

const std::string FILE_PATH = "/Users/ivan/Developer/uniCode/ASM_2/LexAnalyzer/LexicalAnalyzer/LexicalAnalyzer/res.txt";
const std::string FILE_PATH_TWO = "/Users/ivan/Developer/uniCode/ASM_2/LexAnalyzer/LexicalAnalyzer/LexicalAnalyzer/res_two.txt";
const std::string FILE_PATH_ASM = "/Users/ivan/Developer/uniCode/ASM_2/LexAnalyzer/LexicalAnalyzer/LexicalAnalyzer/test.asm";

// Check sentence for valid consistensy
bool is_valid_sentence(std::vector<std::string> sentence, std::set<std::string> delimits, std::set<std::string> instructions,
                       std::vector<int>& res_sentence) {
    const int first_pushes = 2;
    // Feeling two first elemnts with 0
    for (int i = 0; i < first_pushes; ++i) {
        res_sentence.push_back(0);
    }
    
    for(int i = 0; i < sentence.size(); ++i) {
        auto set_it = delimits.find(sentence[i]);
        auto instr_it = instructions.find(sentence[i]);
        
        std::string tmp_str = sentence[i];
        
        if(res_sentence.size() == 3 && set_it == delimits.end()) {
            if (!check_operand(sentence, instructions, res_sentence, i))
                return false;
            break;
        } else {
            if (instr_it == instructions.end() && std::isalpha(tmp_str[0])) {
                res_sentence[0] = 1;
            } else if (instr_it != instructions.end()) {
                res_sentence[1] = i + 1;
                res_sentence.push_back(1);
            } else if (sentence.size() == 2) {
                if (std::isalpha(tmp_str[0]) && !sentence[sentence.size()].compare(":"))
                    res_sentence[0] = 1;
            } else {
                return false;
            }
        }
        
    }
    return true;
}


// Check operand on errors
bool check_operand(std::vector<std::string> sent, std::set<std::string> instr, std::vector<int>& res, int index) {
    bool flag = false;
    int i = 0;
    // Looking for commas
    for (i = 0; i < sent.size(); ++i) {
        if (sent[i].compare(",") == 0) {
            flag = true;
            break;
        }
        else {
            flag = false;
        }
    }
    
    if (flag) {
        for (int j = index; j < i; ++j) {
            auto set_it = instr.find(sent[j]);
            if(set_it != instr.end())
                return false;
        }
        
        res.push_back(index + 1);
        int lexem_number = 0;
        for (int j = index; j < i; j++)
            lexem_number++;
        res.push_back(lexem_number);
        
        if (i == sent.size() - 1) {
            return false;
        } else {
            for (int j = i + 1; j < sent.size(); ++j) {
                auto set_it_one = instr.find(sent[j]);
                if (set_it_one != instr.end())
                    return false;
            }
            res.push_back(i + 2);
            
            int lexem_amount = 0;
            for (int j = i + 1; j < sent.size(); j++)
                if (sent[j] != "'")
                    lexem_amount++;
            res.push_back(lexem_amount);
        }
        
    } else {
        for (int j = index; j < sent.size(); ++j) {
            auto set_it_two = instr.find(sent[j]);
            if (set_it_two != instr.end())
                return false;
        }
        res.push_back(index + 1);
        int lexem_amount = 0;
        for (int j = index; j < sent.size(); j++)
            if (!sent[j].compare("'") && !sent[sent.size() - 1].compare("'")) {
                lexem_amount = 0;
                break;
            } else {
                lexem_amount++;
            }
        if (!lexem_amount)
            res.push_back(lexem_amount + 1);
        else
            res.push_back(lexem_amount);
    }
    
    return true;
}

// Validation of identifiers
bool identifier_check(std::string str) {
    if (!str.empty() && str.length() <= max_lexems_length) {
        for (int i = 0; i < str.length(); ++i)
            if ((!isalpha(str[i]) || !isdigit(str[i])) && islower(str[i]))
                return false;
        return true;
    } else {
        return false;
    }
    
}

// Print the table of lexems
bprinter::TablePrinter tableCreator(std::vector<std::vector<std::string>> vect) {
    std::ofstream fout(FILE_PATH);
    
    std::stringstream ss;
    ss << 34;
    std::string character = ss.str();
    std::set<std::string> delimiters = {",", ";",  character, ":", "\n", "\t", "[", "]", "+", "-", "*"};
    
    bprinter::TablePrinter table(&fout);
    const std::string ch = "'";
    table.AddColumn("№", 10);
    table.AddColumn("Lexsem", 20);
    table.AddColumn("Length of lexsem in symbols", 48);
    table.AddColumn("Type of lexsem", 40);
    
    for (int i = 0; i < vect.size(); ++i) {
        std::vector<std::string> tmp_str = vect[i];
        // cout of sentences
        fout << "'";
        copy(tmp_str.begin(), tmp_str.end(), std::ostream_iterator<std::string>(fout ," "));
        fout << "'";
        fout << "\n";
        table.PrintHeader();
        for (int j = 0; j < tmp_str.size(); ++j) {
            auto it = lexems.find(tmp_str[j]);
            auto set_it = delimiters.find(tmp_str[j]);
            std::string str_to_compare = tmp_str[j];
            if (it != lexems.end())
                table << j << tmp_str[j] << tmp_str[j].size() << lexems.at(tmp_str[j]);
            else if (set_it != delimiters.end())
                table << j << tmp_str[j] << tmp_str[j].size() << "ONE SYMBOL";
            else if (tmp_str[j].compare(ch) != 0 && !identifier_check(tmp_str[j])) {
                table << j << tmp_str[j] << tmp_str[j].size() << "ERROR";
            }
            else if (tmp_str[j - 1].compare(ch) == 0 && tmp_str[j + 1].compare(ch) == 0)
                table << j << tmp_str[j] << tmp_str[j].size() << "CONSTANT";
            else if (isdigit(str_to_compare[0])) {
                bool flag = true;
                for (std::string::iterator it = str_to_compare.begin(); it != str_to_compare.end(); it++) {
                    if ((*it) < '0' && (*it) > '9' && (*it) < 'A' && (*it) > 'H')
                        flag = false;
                }
                if(flag)
                    table << j << tmp_str[j] << tmp_str[j].size() << "CONSTANT";
            } else if (tmp_str[j].compare(ch) != 0) {
                table << j << tmp_str[j] << tmp_str[j].size() << "CUSTOM IDENTIFIER";
            }
        }
        table.PrintFooter();
        fout << "\n\n";
    }
    
    table.PrintFooter();
    
    fout.close();
    
    return table;
}

// Print the table of sentence structure
bprinter::TablePrinter tableCreatorTwo(std::vector<std::vector<std::string>>& vect) {
    std::ofstream fout1(FILE_PATH_TWO);
    std::stringstream ss;
    ss << 34;
    std::string character = ss.str();
    std::set<std::string> delimiters = {",", ";", character, ":", "[", "]", "+", "-", "*"}; // character : "
    std::set<std::string> machine_commands = {"MOV", "STOS", "AND", "ADD", "WAIT", "RETN", "XCHG", "OR", "JBE", "SEGMENT", "END",
        "ENDS", "ASSUME", "DB", "DD", "DW"};
    
    bprinter::TablePrinter table(&fout1);
    const std::string ch = "'";
    table.AddColumn("Num Token", 8);
    table.AddColumn("Num First Command", 14);
    table.AddColumn("Num of lexem", 14);
    table.AddColumn("First Operand", 14);
    table.AddColumn("Num of operand1", 14);
    table.AddColumn("Second Operand", 14);
    table.AddColumn("Num Of Operand2", 14);
    table.PrintHeader();
    
    std::vector<int> result;
    
    for (int i = 0; i < vect.size(); ++i) {
        if (is_valid_sentence(vect[i], delimiters, machine_commands, result)) {
            if (result.size() <= 7) {
                for (int j = (int)result.size(); j < 7; j++) {
                    result.push_back(atoi("'"));
                }
            }
            for (int j = 0; j < result.size(); ++j) {
                table << result[j];
            }
            
            result.clear();
        } else {
            // If sentence is non valid clears it
            vect[i].clear();
            for (int j = 0; j < 7; ++j) {
                table << 0;
            }
        }
    }
    
    table.PrintFooter();
    
    fout1.close();
    return table;
}

// Function that parser of ASM file
std::vector<std::vector<std::string>>read_file() {
    std::vector<std::vector<std::string> > result;
    std::vector<std::string> element_to_res;
    std::string word;
    std::ifstream file;
    
    file.open(FILE_PATH_ASM);
    if (file.is_open()) {
        while (getline(file, word)) {
            std::size_t pos, prev = 0;
            while ((pos = word.find_first_of("\n\t, ';:[]+-*", prev)) != word.npos) {
                if (pos != 0) {
                    element_to_res.push_back(word.substr(prev, pos));
                    word.erase(prev, pos);
                    pos = 0;
                }
                if (word.substr(pos, 1) != " ")
                    element_to_res.push_back(word.substr(pos, 1));
                word.erase(pos, 1);
                if (pos != 0)
                    prev = pos + 1;
            }
            
            if (!word.empty()) {
                element_to_res.push_back(word);
            }
            
            result.push_back(element_to_res);
            element_to_res.clear();
        }
        
        file.close();
        
    } else {
        std::cout << "Unable to open file\n";
    }
    
    return result;
}

// Function to print all content of data structure
void vector_of_words_print(std::vector<std::vector<std::string> > vect) {
    for (int i = 0; i < vect.size(); ++i) {
        std::vector<std::string> tmp = vect[i];
        if (!tmp.empty()) {
            copy(tmp.begin(), tmp.end(), std::ostream_iterator<std::string>(std::cout, " "));
            std::cout << std::endl;
        }
    }
}