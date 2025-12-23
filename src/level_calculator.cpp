#include "level_calculator.h"
#include <cmath>

double calculate_level(int64_t xp, const skill_data &data) {

    if (xp < 50 || data.cumulative_xp.empty()) return 0;

    for (size_t i = 0; i < data.cumulative_xp.size(); ++i) {
        if (xp < data.cumulative_xp[i]) {
            /* if we stopped at the first one just say we are lvl 0 */
            int64_t prev = (i == 0 ? 0LL : data.cumulative_xp[i - 1]);
            double progress = static_cast<double>(xp - prev) / (data.cumulative_xp[i] - prev);
            return i + progress;
        }
    }

    return static_cast<double>(data.cumulative_xp.size());
}

// @TODO: cleanup all of this shit

std::string format_level(double lvl, int max_lvl) {

    // 033[1m = bold
    // 033[0m = regular

    int current = static_cast<int>(std::floor(lvl));
    bool maxed = current == max_lvl;

    /* if maxed make it bold */
    std::string str = maxed ? "\033[1m" : "";

    str += std::to_string(current) + "\033[0m/\033[1m" + std::to_string(max_lvl) + "\033[0m";

    return str;
}

std::string format_special_level(int current, int potential, int max_possible) {

    bool maxed = current == max_possible;
    std::string str = maxed ? "\033[1m" : "";
    str += std::to_string(current) + "\033[0m";
    if (potential > current) {
        str += "(" + std::to_string(potential) + ")";
    }
    str += "/\033[1m" + std::to_string(max_possible) + "\033[0m";
    return str;
}
