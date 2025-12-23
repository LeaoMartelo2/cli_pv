#ifndef SKILL_FETCHER_H
#define SKILL_FETCHER_H

#include "../vendor/json/json.hpp"
#include <cstdint>
#include <map>
#include <string>
#include <vector>

using json = nlohmann::json;

struct skill_data {
    int max_level = 50;
    std::vector<int64_t> cumulative_xp;
};

struct player_skills {
    std::string profile_name = "Unknown";
    int64_t farming_xp = 0;
    int64_t mining_xp = 0;
    int64_t combat_xp = 0;
    int64_t foraging_xp = 0;
    int64_t fishing_xp = 0;
    int64_t enchanting_xp = 0;
    int64_t alchemy_xp = 0;
    int64_t taming_xp = 0;
    int farming_cap_unlocks = 0;
    int taming_cap_unlocks = 50;
};

std::string get_uuid(const std::string &username);
player_skills fetch_player_skills(const std::string &api_key, const std::string &username);
std::map<std::string, skill_data> load_skill_tables(const std::string &filename = "static/skills.json");

#endif
