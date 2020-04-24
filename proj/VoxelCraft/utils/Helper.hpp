#pragma once
#include <string>
#include <iostream>

/**
 * Some generic helper functions that have no real category
 */

inline std::string GetLine(const char* postFix)
{
    std::string line;
    std::getline(std::cin, line);

    std::cout << postFix;

    return line;
}


inline std::string GetLine()
{
    return GetLine("");
}