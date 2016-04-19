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

void gram_analyze(std::vector<std::vector<std::string>>&);

bool segment_check(std::vector<std::vector<std::string>>& vector, std::vector<std::string>& usr_id, std::vector<int>& offset, int& index);
bool mnem_check(std::vector<std::string>& vector, std::vector<std::string>& usr_id, std::vector<int>& offset, int& indx);

bool id_check(std::string str);
bool valid_segment_name(std::vector<std::string> sentence);


#endif /* gramm_analyzer_hpp */
