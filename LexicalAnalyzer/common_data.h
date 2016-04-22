//
//  common_data.h
//  LexicalAnalyzer
//
//  Created by Ivan on 4/13/16.
//  Copyright © 2016 Ivan Tkachenko. All rights reserved.
//

#ifndef common_data_h
#define common_data_h

const int max_lexems_length = 7;
const std::map<std::string, std::string> lexems = {
    {"END", "DIRECTIVE"},
    {"SEGMENT", "DIRECTIVE"},
    {"ENDS", "DIRECTIVE"},
    {"ASSUME", "DIRECTIVE"},
    
    {"DB", "TYPE OF DATA"},
    {"DD", "TYPE OF DATA"},
    {"DW", "TYPE OF DATA"},
    {"BYTE", "TYPE OF DATA"},
    {"WORD", "TYPE OF DATA"},
    {"DWORD", "TYPE OF DATA"},
    {"PTR", "TYPE OF DATA"},

    {"AX", "REGISTR 16-BIT"},
    {"BX", "REGISTR 16-BIT"},
    {"CX", "REGISTR 16-BIT"},
    {"DX", "REGISTR 16-BIT"},
    {"AH", "REGISTR 8-BIT"},
    {"BH", "REGISTR 8-BIT"},
    {"CH", "REGISTR 8-BIT"},
    {"DH", "REGISTR 8-BIT"},
    {"AL", "REGISTR 8-BIT"},
    {"BL", "REGISTR 8-BIT"},
    {"CL", "REGISTR 8-BIT"},
    {"DL", "REGISTR 8-BIT"},
    {"SI", "REGISTR 16-BIT"},
    {"BP", "REGISTR 16-BIT"},
    
    {"CS", "CODE SEGEMENT REGISTR"},
    {"DS", "DATA SEGMENT REGISTR"},
    {"ES", "ADRESS SEGEMNT REGISTR"},
    {"SS", "STACK SEMENT REGISTR"},
    
    {"MOV", "INSTRUCTION"},
    {"STOS", "INSTRUCTION"},
    {"ADD", "INSTRUCTION"},
    {"WAIT", "INSTRUCTION"},
    {"RETN", "INSTRUCTION"},
    {"XCHG", "INSTRUCTION"},
    {"AND", "INSTRUCTION"},
    {"OR", "INSTRUCTION"},
    {"JBE", "INSTRUCTION"},
    
    
    //    {", ';:[]+-*", "onesymbol"}
    
    // Add another
};


#endif /* common_data_h */
