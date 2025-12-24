#include "progress_bar.h"
#include <algorithm>
#include <cmath>
#include "config.h"

//const int PROGRESS_BAR_WIDTH = 60;

std::string make_progress_bar(double current, double max, bool is_special, double potential) {

    int filled_unlocked = static_cast<int>(std::round(current / max * g_config.progress_width));
    int filled_potential = is_special ? static_cast<int>(std::round(potential / max * g_config.progress_width)) : filled_unlocked;

    filled_unlocked = std::min(filled_unlocked, g_config.progress_width);
    filled_potential = std::min(filled_potential, g_config.progress_width);

    /* 
    ─
     ^  em dash (bigger than -)
   */

    // bold open square bracket
    std::string bar = "\033[1m" "["  "\033[0m";

    for( int i = 0; i  < g_config.progress_width; ++i ) {
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

