#pragma once

#include "block.hpp"

#include "stdint-gcc.h"

#include <boost/multiprecision/cpp_int.hpp>

using boost::multiprecision::uint256_t;

std::pair<std::string, int> prove(std::string prev_hash, std::string data, uint difficulty);

bool validate(const Block* block, int difficulty);