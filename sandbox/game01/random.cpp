#include "random.hpp"

std::mt19937 random::s_RandomEngine;
std::uniform_int_distribution<std::mt19937::result_type> random::s_Distribution;