#pragma once

#include <string>

using std::string;

string to_hex_string(uint num)
{
    char buffer[9];
    sprintf(buffer, "%08x", num);

    return string{std::begin(buffer), std::end(buffer)-1};
}