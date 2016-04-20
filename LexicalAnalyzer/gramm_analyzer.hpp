//
//  gramm_analyzer.hpp
//  LexicalAnalyzer
//
//  Created by Ivan on 4/13/16.
//  Copyright © 2016 Ivan Tkachenko. All rights reserved.
//

#ifndef gramm_analyzer_hpp
#define gramm_analyzer_hpp

#include "stdafx.h"
#include "common_data.h"

enum instructions {MOV, ADD, AND, OR, XCHG, JBE, WAIT, RETN, STOS};

void gram_analyze(std::vector<std::vector<std::string>>&);

bool segment_check(std::vector<std::vector<std::string>>& vector, std::vector<std::string>& usr_id, std::vector<int>& offset, int index);
bool mnem_check(std::vector<std::string>& vector, std::vector<std::string>& usr_id, std::vector<int>& offset, int indx);
void command_run(std::vector<std::string> instuct, std::vector<int>& offset, int indx);

bool id_check(std::string str);
bool valid_segment_name(std::vector<std::string> sentence);
bool label_check(std::vector<std::string> label);


void mov_command(std::vector<std::string> instr, std::vector<int>& offset, int indx);
void add_command(std::vector<std::string> instr, std::vector<int>& offset, int indx);
void and_command(std::vector<std::string> instr, std::vector<int>& offset, int indx);
void or_command(std::vector<std::string> instr, std::vector<int>& offset, int indx);
void stos_command(std::vector<std::string> instr, std::vector<int>& offset, int indx);
void xchg_command(std::vector<std::string> instr, std::vector<int>& offset, int indx);
void jbe_command(std::vector<std::string> instr, std::vector<int>& offset, int indx);



#endif /* gramm_analyzer_hpp */
