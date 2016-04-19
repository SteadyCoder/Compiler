//
//  function_realization.hpp
//  LexicalAnalyzer
//
//  Created by Ivan on 4/13/16.
//  Copyright © 2016 Ivan Tkachenko. All rights reserved.
//

#ifndef function_realization_hpp
#define function_realization_hpp

#pragma once
#include "stdafx.h"
#include "common_data.h"

std::vector<std::vector<std::string>>read_file();

void vector_of_words_print(std::vector<std::vector<std::string> > vect);

bprinter::TablePrinter tableCreator(std::vector<std::vector<std::string>> vect);
bprinter::TablePrinter tableCreatorTwo(std::vector<std::vector<std::string>>& vect);

bool identifier_check(std::string str);
bool number_constant_check(std::string str);
bool check_operand(std::vector<std::string> sent, std::set<std::string> instr, std::vector<int>& res, int index);

bool is_valid_sentence(std::vector<std::string> sentence, std::set<std::string> delimits,
                       std::set<std::string> instructions, std::vector<int>& res_sentence);



#endif /* function_realization_hpp */
