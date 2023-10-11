#ifndef TOOLS_H
#define TOOLS_H
#include "ZHash.h"

#include "ZHash.h"

#include <string>
#include <iostream>
#include <string>
#include <vector>
#include <regex>
#include <chrono>


class Tools {
public:
    static std::string ZHX(const std::string &input);
    static std::chrono::seconds DecodeTimeNotation(const std::string &TimeStr);
    static std::string Tools::EncodeTimeNotation(int ts);
    static std::string Tools::EncodeTimeNotation(std::chrono::seconds t);
    static std::chrono::seconds Tools::GetCurrentUnixTimestamp();
};

#endif