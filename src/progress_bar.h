#ifndef PROGRESS_BAR_H
#define PROGRESS_BAR_H

#include <string>

std::string make_progress_bar(double current, double max, bool is_special = false, double potential = 0.0);

#endif
