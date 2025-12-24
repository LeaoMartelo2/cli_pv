#include "sb_level.h"
#include <string>

void print_sb_lvl(double lvl) {

    int64_t lvl_cmp = static_cast<int64_t>(lvl);

    std::string color_prefix = "\033[90m"; // GRAY

    if (lvl_cmp > LVL_COLOR::WHITE) {
        color_prefix = "\033[97m";
    }

    if (lvl_cmp > LVL_COLOR::YELLOW) {
        color_prefix = "\033[97m";
    }

    if (lvl_cmp > LVL_COLOR::GREEN) {
        color_prefix = "\033[92m";
    }

    if (lvl_cmp > LVL_COLOR::DARK_GREEN) {
        color_prefix = "\033[32m";
    }

    if (lvl_cmp > LVL_COLOR::AQUA) {
        color_prefix = "\033[96m";
    }

    if (lvl_cmp > LVL_COLOR::CYAN) {
        color_prefix = "\033[36m";
    }

    if (lvl_cmp > LVL_COLOR::BLUE) {
        color_prefix = "\033[94m";
    }

    if (lvl_cmp > LVL_COLOR::PINK) {
        color_prefix = "\033[95m";
    }

    if (lvl_cmp > LVL_COLOR::PURPLE) {
        color_prefix = "\033[35m";
    }

    if (lvl_cmp > LVL_COLOR::GOLD) {
        color_prefix = "\033[33m";
    }

    if (lvl_cmp > LVL_COLOR::RED) {
        color_prefix = "\033[91m";
    }

    if (lvl_cmp > LVL_COLOR::DARK_RED) {
        color_prefix = "\033[31m";
    }

    printf("\033[1mSkyblock Level: %s%.2f \033[0m\n", color_prefix.c_str(), lvl);
}

void test_sb_lvl() {

    print_sb_lvl(1);    // GRAY
    print_sb_lvl(40);   // WHITE
    print_sb_lvl(80);   // YELLOW
    print_sb_lvl(120);  // GREEN
    print_sb_lvl(160);  // DARK_GREEN
    print_sb_lvl(200);  // AQUA
    print_sb_lvl(240);  // CYAN
    print_sb_lvl(280);  // BLUE
    print_sb_lvl(320);  // PINK
    print_sb_lvl(360);  // PURPLE
    print_sb_lvl(400);  // GOLD
    print_sb_lvl(440);  // RED
    print_sb_lvl(480);  // DARK_RED

}
