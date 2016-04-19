# make some imports
import os
import sys
import texttable as tt

""" Useful constants """

reserved_names = {
    "SEGMENT": "directive",
    "END": "directive",
    "ENDS": "directive",
    "IF": "directive",
    "ELSE": "directive",
    "ENDIF": "directive",
    "DW": "directive",
    "DD": "directive",
    "DB": "directive",
    "=": "directive",
    "CMPSD": "machine command",
    "NEG": "machine command",
    "ADD": "machine command",
    "CMP": "machine command",
    "AND": "machine command",
    "SHR": "machine command",
    "OR": "machine command",
    "JBE": "machine command",
    "EAX": "32-bit \n register id",
    "ECX": "32-bit \n register id",
    "EDX": "32-bit \n register id",
    "EBX": "32-bit \n register id",
    "ESP": "32-bit \n register id",
    "EBP": "32-bit \n register id",
    "EDI": "32-bit \n register id",
    "ESI": "32-bit \n register id",
    "EAX": "32-bit \n register id",
    "AH": "8-bit \n register id",
    "AL": "8-bit \n register id",
    "BH": "8-bit \n register id",
    "BL": "8-bit \n register id",
    "CH": "8-bit \n register id",
    "Cl": "8-bit \n register id",
    "DH": "8-bit \n register id",
    "DL": "8-bit \n register id",
    ":": "symbol",
    "+": "symbol",
    ",": "symbol",
    "[": "symbol",
    "]": "symbol",
    "DS": "segment register",
    "ES": "segment register",
    "CS": "segment register"
}
machine_commands = ["SEGMENT", "END", "ENDS", "CMPSD", "SHR", "NEG", "JBE", "OR", "IF", "ELSE", "ENDIF", "DW", "DD", "DB", "=", "DEC", "CMP", "ADD", "AND"]
alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789"
delimiters = [':', ',', ']', '[', ';', ' ', '\n', '\t']

def is_valid(str, valid_symbols):
    return [char for char in str if char not in valid_symbols]

def check_op(indx, lst, res_list):
    if ',' in lst:
        for item in xrange(lst.index(',')):
            if item in machine_commands:
                return False
        res_list.extend([indx + 1, len(lst[: lst.index(',')])])
        #

        if lst.index(',') != len(lst) - 1:
            for item in xrange(lst.index(',') + 1, len(lst)):
                if item in machine_commands:
                    return False
            res_list.extend([lst.index(',') + 1, len(lst[lst.index(',') + 1 : len(lst)])])
        else:
            return False

    else:
        for item in lst:
            if (item in machine_commands):
                return False
        res_list.extend([indx + 1, len(lst)])
    return True

def is_valid_structure(lst, res_list = ['-']):
    for item in xrange(len(lst)):
        if (len(res_list) == 3) and (lst[item] not in delimiters):
            if check_op(item, lst[item:], res_list) == False:
                return False
            break
        else:
            if (lst[item] not in machine_commands) and (lst[item][0] in alphabet[:26] or lst[item] == ':'):
                res_list[0] = 1
            elif lst[item] in machine_commands:
                res_list.extend([item + 1, 1])
            else:
                return False
    return True

""" parses each line in file """
def line_parser(line):
    tokens = []
    word = ""
    for item in line:
        if item not in delimiters:
            word += item
        elif item in delimiters[: 5]:
            if word != "":
                tokens.append(word)
            tokens.append(item)
            word = ""
        elif word != "":
            tokens.append(word)
            word = ""
    return tokens

""" make table of lexem """
def table_of_lexem(file_name, lexem):
    table = tt.Texttable()
    rows = [['#',"Lexem", "Length","Type"]]
    table.set_cols_align(["c", "c", "c", "c"])
    table.set_cols_valign(["m", "m", "m", "m"])
    table_of_lexem =  open(file_name, 'w')
    for list_of_words in lexem:
        error = 0
        for word in list_of_words:
            if word in reserved_names.keys():
                rows.append([list_of_words.index(word) + 1, word, len(word), reserved_names[word.upper()]])

            elif (word[0] == "'" or word[0] == '"') and (word[-1] == "'" or word[-1] == '"'):
                rows.append([list_of_words.index(word) + 1, word, len(word), "text constant"])

            elif word[0] in alphabet[:25] and len(word) <= 8 and is_valid(word, alphabet) == []:
                rows.append([list_of_words.index(word) + 1, word, len(word), "user id \n or \n unspecified"])

            elif word[0] in alphabet[26:] and word[-1] == 'H' and  is_valid(word, "ABCDEFH0123456789") == []:
                rows.append([list_of_words.index(word) + 1, word, len(word), "hexademical constant"])

            elif word[0] in alphabet[26:] and word[-1] == 'B' and is_valid(word, "01B") == []:
                rows.append([list_of_words.index(word) + 1, word, len(word), "binary constant"])

            elif ((word[0] in alphabet[26:] and word[-1].upper() == 'D') or (word[0] in alphabet[26:] and word[-1] in alphabet[26:])) and is_valid(word, "0123456789D") == []:
                rows.append([list_of_words.index(word) + 1, word, len(word), "decimal constant"])

            else:
                error = 1
                break
        if error:
            lexem[lexem.index(list_of_words)] = 0
            rows = [['#',"Lexem", "Length","Type"]]
        else:
            table.add_rows(rows)
            table_of_lexem.write(table.draw() + '\n\n')
            table.reset()
            rows = [['#',"Lexem", "Length","Type"]]

    table_of_lexem.close()
    return None

""" make table of sentence structure  """
def sentence_structure_table(list_of_lexem):
    table = tt.Texttable()
    table.set_cols_align(['c', 'c', 'c', 'c', 'c', 'c', 'c'])
    table.set_cols_valign(['m', 'm', 'm', 'm', 'm', 'm', 'm' ])
    index = 0
    rows = [["Num. tokens\nlabel", "Num. first\ncommand\nlexem\nfield", "Num. of\nfield\nlexem", "First num.\nlexem\nop.",
            "Num. of\nop.\nlexem", "First num.\nlexem\nop.", "Num. of\nop.\nlexem"]]
    element = ['-']
    for item in list_of_lexem:
        for token in item:
            if token == item[0] and token.upper() not in reserved_names.keys():
                element[0] = 1
            elif token.upper() in machine_commands:
                if token not in delimiters[: 5]:
                    element.extend([item.index(token) + 1, 1])
                else:
                    index = item.index(token)
            else:
                element.append(item.index(token) + 1)
                if ',' in item:
                    element.extend([item.index(',') - 1, item.index(',') - index])
                    break
                else:
                    element.append(len( item[item.index(token) :] ))
                    break


        if len(element) != 7:
            element.extend(['-'] * (7 - len(element)))
        rows.append(element)
        element = ['-']
    table.add_rows(rows)
    return table.draw() + '\n'


def sentence_analyser(file_name, lexem):
    k = 0
    table = tt.Texttable()
    table.set_cols_align(['c', 'c', 'c', 'c', 'c', 'c', 'c'])
    table.set_cols_valign(['m', 'm', 'm', 'm', 'm', 'm', 'm' ])
    rows = [["Num. tokens\nlabel", "Num. first\ncommand\nlexem\nfield", "Num. of\nfield\nlexem", "First num.\nlexem\nop.",
            "Num. of\nop.\nlexem", "First num.\nlexem\nop.", "Num. of\nop.\nlexem"]]
    table_of_sentence = open(file_name, 'w')
    element = ['-']
    for item in lexem:
        if item == 0:
            continue
        if is_valid_structure(item, element):
            if len(element) != 7:
                element.extend(['-'] * (7 - len(element)))
            rows.append(element)
            element = ['-']
        else:
            element = ['-']
            lexem[lexem.index(item)] = 0
    table.add_rows(rows)
    table_of_sentence.write(table.draw() + '\n')
    table_of_sentence.close()
    return None


""" parses source files """
def parser(file_name):
    lines = []
    lexem = []
    try:
        source_file = open(file_name , 'r')
    except IOError:
        raise NameError("Oops, the file doesn't exist")

    lines = source_file.readlines()
    [lexem.append(line_parser(line)) for line in lines if line_parser(line) != []]
    lexem = [map(lambda x: x.upper(), item) for item in lexem]
    source_file.close()
    return lexem

if __name__ == "__main__":
    file_name = sys.argv[1]
    lexem = parser(file_name)
    print lexem
    table_of_lexem("lexem_table.txt", lexem)
    print sentence_analyser("sentence.txt", lexem)
    #el = ['-']
    #print is_valid_structure(lexem[1], el)
    print lexem
