#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <unordered_map>
//#include "optab.hpp"

using namespace std;

class OP{
public:
    std::string op;
    int opcode;
    short format;

    OP(){}
    OP(std::string _op, int _opcode, short _format): op{_op},
                                                     opcode{_opcode},
                                                     format{_format}{}
};

class Instruction{
public:
    string label;
    string op;
    string params;
    int format;

    Instruction(){}
    Instruction(string _label, string _op, string _params, int _format): label(_label),
                                                                     op(_op),
                                                                     params(_params),
                                                                     format(_format){}
};

Instruction parse(string&);

// TODO: move to optab.cpp when i figure out how extern works
std::unordered_map<std::string, OP> OPTAB(
    {{"ADD", OP("ADD", 0x18, 3)},
    {"ADDF", OP("ADDF", 0x58, 3)},
    {"ADDR", OP("ADDR", 0x90, 2)},
    {"AND", OP("AND", 0x40, 3)},
    {"CLEAR", OP("CLEAR", 0xB4, 2)},
    {"COMP", OP("COMP", 0x28, 3)},
    {"COMPF", OP("COMPF", 0x88, 3)},
    {"COMPR", OP("COMPR", 0xA0, 2)},
    {"DIV", OP("DIV", 0x24, 3)},
    {"DIVF", OP("DIVF", 0x64, 3)},
    {"DIVR", OP("DIVR", 0x9C, 2)},
    {"FIX", OP("FIX", 0xC4, 1)},
    {"FLOAT", OP("FLOAT", 0xC0, 1)},
    {"HIO", OP("HIO", 0xF4, 1)},
    {"J", OP("J", 0x3C, 3)},
    {"JEQ", OP("JEQ", 0x30, 3)},
    {"JGT", OP("JGT", 0x34, 3)},
    {"JLT", OP("JLT", 0x38, 3)},
    {"JSUB", OP("JSUB", 0x48, 3)},
    {"LDA", OP("LDA", 0x00, 3)},
    {"LDB", OP("LDB", 0x68, 3)},
    {"LDCH", OP("LDCH", 0x50, 3)},
    {"LDF", OP("LDF", 0x70, 3)},
    {"LDL", OP("LDL", 0x08, 3)},
    {"LDS", OP("LDS", 0x6C, 3)},
    {"LDT", OP("LDT", 0x74, 3)},
    {"LDX", OP("LDX", 0x04, 3)},
    {"LPS", OP("LPS", 0xD0, 3)},
    {"MUL", OP("MUL", 0x20, 3)},
    {"MULF", OP("MULF", 0x60, 3)},
    {"MULR", OP("MULR", 0x98, 2)},
    {"NORM", OP("NORM", 0xC8, 1)},
    {"OR", OP("OR", 0x44, 3)},
    {"RD", OP("RD", 0xD8, 3)},
    {"RMO", OP("RMO", 0xAC, 2)},
    {"RSUB", OP("RSUB", 0x4C, 3)},
    {"SHIFTL", OP("SHIFTL", 0xA4, 2)},
    {"SHIFTR", OP("SHIFTR", 0xA8, 2)},
    {"SIO", OP("SIO", 0xF0, 1)},
    {"SSK", OP("SSK", 0xEC, 3)},
    {"STA", OP("STA", 0x0C, 3)},
    {"STB", OP("STB", 0x78, 3)},
    {"STCH", OP("STCH", 0x54, 3)},
    {"STF", OP("STF", 0x80, 3)},
    {"STI", OP("STI", 0xD4, 3)},
    {"STL", OP("STL", 0x14, 3)},
    {"STS", OP("STS", 0x7C, 3)},
    {"STSW", OP("STSW", 0xE8, 3)},
    {"STT", OP("STT", 0x84, 3)},
    {"STX", OP("STX", 0x10, 3)},
    {"SUB", OP("SUB", 0x1C, 3)},
    {"SUBF", OP("SUBF", 0x5C, 3)},
    {"SUBR", OP("SUBR", 0x94, 2)},
    {"SVC", OP("SVC", 0xB0, 2)},
    {"TD", OP("TD", 0xE0, 3)},
    {"TIO", OP("TIO", 0xF8, 1)},
    {"TIX", OP("TIX", 0x2C, 3)},
    {"TIXR", OP("TIXR", 0xB8, 2)},
    {"WD", OP("WD", 0xD, 3)}}
);

std::unordered_map<std::string, OP> DIRECTIVES(
    {{"START", OP("START", 0, 0)},
    {"BASE", OP("BASE", 0, 0)},
    {"WORD", OP("WORD", 0, 0)},
    {"RESW", OP("RESW", 0, 0)},
    {"RESB", OP("RESB", 0, 0)},
    {"BYTE", OP("BYTE", 0, 0)},
    {"END", OP("END", 0, 0)}}
);
int main(){
    ifstream source("sample/source.txt");
    for(string line; getline(source, line); ){
        Instruction instruction = parse(line);
        if(instruction.op != "")
        cout << instruction.op << " " << instruction.format << endl;
    }

    return 0;
}

Instruction parse(string& line){
    // handle line comment and empty line
    if(line[0] == '.' || line.length() == 0){
        return Instruction("", "", "", 0);
    }

    vector<string> fields;
    size_t pos = line.find(" ");
    size_t startpos = 0;

    while(pos != string::npos){
        fields.push_back(line.substr(startpos, pos - startpos));
        startpos = pos + 1;
        // handle trailing comment
        if(line[startpos] == '.') break;

        pos = line.find(" ", startpos);
    }
    if(pos == string::npos) fields.push_back(line.substr(startpos, line.size() - startpos));

    // if the first field starts with +, it's a format 4 op field
    if(fields[0][0] == '+'){
        // +op [params]
        if(fields.size() <= 1) return Instruction("", fields[0].substr(1), "", 4);
        else return Instruction("", fields[0].substr(1), fields[1], 4);
    }
    // otherwise determine if first field is a label, op, or directive
    else{
        if(OPTAB.count(fields[0]) > 0){
            // op [params]
            if(fields.size() <= 1) return Instruction("", fields[0], "", OPTAB[fields[0]].format);
            else return Instruction("", fields[0], fields[1], OPTAB[fields[0]].format);
        }
        else if(DIRECTIVES.count(fields[0]) > 0){
            // directive [params]
            if(fields.size() <= 1) return Instruction("", fields[0], "", 0); 
            else return Instruction("", fields[0], fields[1], 0);
        }
        // the first field is label, check if second field is op or directive
        else{
            // check if the second field is a format 4 op
            if(fields[1][0] == '+'){
                // label +op [params]
                if(fields.size() <= 2) return Instruction(fields[0], fields[1].substr(1), "", 4);
                else return Instruction(fields[0], fields[1].substr(1), fields[2], 4);
            }
            else if(OPTAB.count(fields[1]) > 0){
                // label op [params]
                if(fields.size() <= 2) return Instruction(fields[0], fields[1], "", OPTAB[fields[1]].format);
                else return Instruction(fields[0], fields[1], fields[2], OPTAB[fields[1]].format);
            }
            else{
                // label directive [params]
                if(fields.size() <= 2) return Instruction(fields[0], fields[1], "", 0); 
                else return Instruction(fields[0], fields[1], fields[2], 0);
            }
        }
    }
}