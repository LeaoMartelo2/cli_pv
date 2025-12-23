#ifndef LEVEL_CALCULATOR_H
#define LEVEL_CALCULATOR_H

#include "skill_fetcher.h"
#include <string>

double calculate_level(int64_t xp, const skill_data &data);
std::string format_level(double lvl, int max_lvl);
std::string format_special_level(int current, int potential, int max_possible);

#endif
