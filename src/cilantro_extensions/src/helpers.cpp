//
// Created by felix on 25.11.2021.
//
#include "../includes/helpers.h"

std::string to_lower(std::string s) {
    std::transform(s.begin(), s.end(), s.begin(),  [](unsigned char c){ return std::tolower(c); });
    return s;
}

