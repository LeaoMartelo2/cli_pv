#include "level_calculator.h"
#include "progress_bar.h"
#include "skill_fetcher.h"
#include <curl/curl.h>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <print>
#include <string>

std::string load_api_key() {

    /* this is prob one of the worst ways to do it */

    std::ifstream file(".env");
    std::string line;

    while (std::getline(file, line)) {
        if (line.rfind("HYPIXEL_API_KEY=", 0) == 0) {
            return line.substr(16);
        }
    }
    return "";
}

int main(int argc, char **argv) {

    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <player_name>\n";
        return 1;
    }

    std::string username = argv[1];
    std::string api_key = load_api_key();

    if (api_key.empty()) {
        std::cerr << "Err: HYPIXEL_API_KEY not found\n";
        return 1;
    }

    curl_global_init(CURL_GLOBAL_DEFAULT);

    player_skills skills = fetch_player_skills(api_key, username);

    if (skills.profile_name == "Unknown") {
        std::cerr << "Failed to fetch player data\n";
        curl_global_cleanup();
        return 1;
    }

    auto tables = load_skill_tables();

    // c++23 print
    // std::print("\nUsername: {} | Profile: {}\n\n", username, skills.profile_name);

    std::cout << "\nUsername: " << username << " | Profile: " << skills.profile_name << "\n\n";

    // clang-format off

    double farming_lvl      = tables.count("FARMING")       ? calculate_level(skills.farming_xp,    tables["FARMING"]) : 0.0;
    double mining_lvl       = tables.count("MINING")        ? calculate_level(skills.mining_xp,     tables["MINING"]) : 0.0;
    double combat_lvl       = tables.count("COMBAT")        ? calculate_level(skills.combat_xp,     tables["COMBAT"]) : 0.0;
    double foraging_lvl     = tables.count("FORAGING")      ? calculate_level(skills.foraging_xp,   tables["FORAGING"]) : 0.0;
    double fishing_lvl      = tables.count("FISHING")       ? calculate_level(skills.fishing_xp,    tables["FISHING"]) : 0.0;
    double enchanting_lvl   = tables.count("ENCHANTING")    ? calculate_level(skills.enchanting_xp, tables["ENCHANTING"]) : 0.0;
    double alchemy_lvl      = tables.count("ALCHEMY")       ? calculate_level(skills.alchemy_xp,    tables["ALCHEMY"]) : 0.0;
    double taming_lvl       = tables.count("TAMING")        ? calculate_level(skills.taming_xp,     tables["TAMING"]) : 0.0;

    // clang-format on

    int farming_max_unlocked = 50 + skills.farming_cap_unlocks;
    int farming_max = tables.count("FARMING") ? tables["FARMING"].max_level : 60;

    int taming_max_unlocked = skills.taming_cap_unlocks;
    int taming_max = tables.count("TAMING") ? tables["TAMING"].max_level : 60;

    int current_farming = static_cast<int>(std::floor(std::min(farming_lvl, static_cast<double>(farming_max_unlocked))));
    int potential_farming = static_cast<int>(std::floor(farming_lvl));

    int current_taming = static_cast<int>(std::floor(std::min(taming_lvl, static_cast<double>(taming_max_unlocked))));
    int potential_taming = static_cast<int>(std::floor(taming_lvl));

    auto print = [&](const std::string &name, const std::string &bar, const std::string &level) {
        std::cout << "\033[1m" << std::left << std::setw(12) << name << "\033[0m" << bar << " " << level << "\n";
    };

    print("Farming", make_progress_bar(current_farming, farming_max, true, potential_farming),
          format_special_level(current_farming, potential_farming, farming_max));

    print("Mining", make_progress_bar(mining_lvl, tables["MINING"].max_level),
          format_level(mining_lvl, tables["MINING"].max_level));

    print("Combat", make_progress_bar(combat_lvl, tables["COMBAT"].max_level),
          format_level(combat_lvl, tables["COMBAT"].max_level));

    print("Foraging", make_progress_bar(foraging_lvl, tables["FORAGING"].max_level),
          format_level(foraging_lvl, tables["FORAGING"].max_level));

    print("Fishing", make_progress_bar(fishing_lvl, tables["FISHING"].max_level),
          format_level(fishing_lvl, tables["FISHING"].max_level));

    print("Enchanting", make_progress_bar(enchanting_lvl, tables["ENCHANTING"].max_level),
          format_level(enchanting_lvl, tables["ENCHANTING"].max_level));

    print("Alchemy", make_progress_bar(alchemy_lvl, tables["ALCHEMY"].max_level),
          format_level(alchemy_lvl, tables["ALCHEMY"].max_level));

    print("Taming", make_progress_bar(current_taming, taming_max, true, potential_taming),
          format_special_level(current_taming, potential_taming, taming_max));

    std::cout << "\033[0m";

    curl_global_cleanup();
    return 0;
}
