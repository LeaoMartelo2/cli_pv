#include "skill_fetcher.h"
#include <curl/curl.h>
#include <algorithm>
#include <iostream>
#include <fstream>

//@TODO: fix everything


static size_t write_callback(void* contents, size_t size, size_t nmemb, std::string* userp) {
    userp->append((char*)contents, size * nmemb);
    return size * nmemb;
}

std::string get_uuid(const std::string& username) {
    CURL* curl = curl_easy_init();
    if (!curl) return "";

    std::string buffer;
    std::string url = "https://api.mojang.com/users/profiles/minecraft/" + username;
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &buffer);

    CURLcode res = curl_easy_perform(curl);
    curl_easy_cleanup(curl);

    if (res != CURLE_OK) return "";

    try {
        json j = json::parse(buffer);
        return j["id"];
    } catch (...) {
        return "";
    }
}

std::map<std::string, skill_data> load_skill_tables(const std::string& filename) {
    std::map<std::string, skill_data> tables;

    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Warning: Could not open " << filename << "\n";
        return tables;
    }

    std::string buffer((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

    try {
        json j = json::parse(buffer);
        if (!j.contains("skills")) return tables;

        for (const auto& [name, info] : j["skills"].items()) {
            skill_data sd;
            sd.max_level = info.value("maxLevel", 50);

            if (info.contains("levels") && info["levels"].is_array()) {
                for (const auto& level : info["levels"]) {
                    sd.cumulative_xp.push_back(level.value("totalExpRequired", 0LL));
                }
            }
            tables[name] = sd;
        }
    } catch (...) {}

    return tables;
}

player_skills fetch_player_skills(const std::string& api_key, const std::string& username) {
    player_skills info;

    std::string uuid_dashed = get_uuid(username);
    if (uuid_dashed.empty()) {
        std::cerr << "Failed to fetch UUID for " << username << "\n";
        return info;
    }

    std::string uuid = uuid_dashed;
    uuid.erase(std::remove(uuid.begin(), uuid.end(), '-'), uuid.end());

    CURL* curl = curl_easy_init();
    if (!curl) return info;

    std::string buffer;
    std::string url = "https://api.hypixel.net/v2/skyblock/profiles?key=" + api_key + "&uuid=" + uuid;
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &buffer);

    CURLcode res = curl_easy_perform(curl);
    curl_easy_cleanup(curl);

    if (res != CURLE_OK) {
        std::cerr << "Failed to fetch profiles\n";
        return info;
    }

    json j;
    try { j = json::parse(buffer); } catch (...) { return info; }

    if (!j.value("success", false)) {
        std::cerr << "Failed to fetch profiles\n";
        return info;
    }

    std::string selected_profile_id;
    for (const auto& profile : j["profiles"]) {
        if (profile.value("selected", false)) {
            selected_profile_id = profile["profile_id"];
            info.profile_name = profile.value("cute_name", "Unknown");
            break;
        }
    }

    if (selected_profile_id.empty()) {
        std::cerr << "No selected profile found\n";
        return info;
    }

    buffer.clear();
    std::string profile_url = "https://api.hypixel.net/v2/skyblock/profile?key=" + api_key + "&profile=" + selected_profile_id;
    curl = curl_easy_init();
    curl_easy_setopt(curl, CURLOPT_URL, profile_url.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &buffer);
    res = curl_easy_perform(curl);
    curl_easy_cleanup(curl);

    if (res != CURLE_OK) {
        std::cerr << "Failed to fetch profile\n";
        return info;
    }

    json profile_j;
    try { profile_j = json::parse(buffer); } catch (...) { return info; }

    if (!profile_j.value("success", false)) {
        std::cerr << "Failed to fetch profile data\n";
        return info;
    }

    const json& members = profile_j["profile"]["members"];
    const json& member = members.value(uuid, json::object());

    if (member.empty() || !member.contains("player_data") || !member["player_data"].contains("experience")) {
        std::cerr << "Skills data not available (API disabled?)\n";
        return info;
    }

    const json& exp = member["player_data"]["experience"];

    // clang-format off

    info.farming_xp    = exp.value("SKILL_FARMING",    0LL);
    info.mining_xp     = exp.value("SKILL_MINING",     0LL);
    info.combat_xp     = exp.value("SKILL_COMBAT",     0LL);
    info.foraging_xp   = exp.value("SKILL_FORAGING",   0LL);
    info.fishing_xp    = exp.value("SKILL_FISHING",    0LL);
    info.enchanting_xp = exp.value("SKILL_ENCHANTING", 0LL);
    info.alchemy_xp    = exp.value("SKILL_ALCHEMY",    0LL);
    info.taming_xp     = exp.value("SKILL_TAMING",     0LL);

    // clang-format on
    

    
    /* check if player has farming levels above 50 unlocked */
    if (member.contains("jacobs_contest") && member["jacobs_contest"].contains("perks")) {
        info.farming_cap_unlocks = member["jacobs_contest"]["perks"].value("farming_level_cap", 0);
    }

    /* very badly chjeck if player has taming levels over 50 unlocked */
    if (member.contains("pets_data") && member["pets_data"].contains("pet_care") &&
        member["pets_data"]["pet_care"].contains("pet_types_sacrificed") &&
        member["pets_data"]["pet_care"]["pet_types_sacrificed"].is_array()) {
        int count = member["pets_data"]["pet_care"]["pet_types_sacrificed"].size();
        info.taming_cap_unlocks = 50 + count;
    }

    if(member.contains("leveling") && member["leveling"].contains("experience")) {
        info.total_sb_xp = member["leveling"]["experience"];
        info.skyblock_level = info.total_sb_xp / 100.00;
    }

    return info;
}
