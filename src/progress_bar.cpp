#include "progress_bar.h"
#include <algorithm>
#include <cmath>

const int PROGRESS_BAR_WIDTH = 30;

std::string make_progress_bar(double current, double max, bool is_special, double potential) {

    int filled_unlocked = static_cast<int>(std::round(current / max * PROGRESS_BAR_WIDTH));
    int filled_potential = is_special ? static_cast<int>(std::round(potential / max * PROGRESS_BAR_WIDTH)) : filled_unlocked;

    filled_unlocked = std::min(filled_unlocked, PROGRESS_BAR_WIDTH);
    filled_potential = std::min(filled_potential, PROGRESS_BAR_WIDTH);

    /* 
    ─
     ^  em dash (bigger than -)
   */

    // bold open square bracket
    std::string bar = "\033[1m" "["  "\033[0m";

    for( int i = 0; i  < PROGRESS_BAR_WIDTH; ++i ) {
        if ( i < filled_unlocked ) {
            // green
            bar += "\033[32m─\033[0m";
        } else if (is_special && i < filled_potential) {
            // yellow
            bar += "\033[33m" "─" "\033[0m";
        } else {
            // red
            bar += "\033[31m" "─" "\033[0m";
        }
    }

    bar += "\033[1m" "]" "\033[0m";

    return bar;
}

