#ifndef OPTAB_H
#define OPTAB_H
#include <string>
#include <unordered_map>

/*
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
*/

extern std::unordered_map<std::string, OP> OPTAB;
#endif