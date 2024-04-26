#include "json_conversion.hpp"
#include "item.hpp"
#include "player.hpp"
#include "location.hpp"
#include "dungeon.hpp"
#include "enemy.hpp"

void to_json(nlohmann::json& j, const Item& I) {
    j = nlohmann::json{{"object", "Item"}, {"name", I.name}, {"type", I.type}, {"stat", I.stat}};
}

void from_json(const nlohmann::json& j, Item& I) {
    I.name = j.at("name").get<std::string>();
    I.type = j.at("type").get<Type>();
    I.stat = j.at("stat").get<int>();
}

void to_json(nlohmann::json& j, const Location& L) {
    j = nlohmann::json{{"object", "Location"}, {"name", L.name}, {"encounter_rate", L.encounter_rate}, {"enemies", L.enemies}, {"poi", L.poi}};
}

void from_json(const nlohmann::json& j, Location& L) {
    L.name = j.at("name").get<std::string>();
    L.encounter_rate = j.at("encounter_rate").get<float>();
    L.enemies = j.at("enemies").get<std::vector<std::string> >();
    L.poi = j.at("poi").get<std::vector<std::string> >();
}


void to_json(nlohmann::json& j, const Player& P) {
    j = nlohmann::json{{"object", "Player"}, {"name", P.name}, {"health", P.health},
                       {"weapon", P.weapon}, {"armor", P.armor},
                       {"max_health", P.max_health}, {"experience", P.experience},
                       {"level", P.level}, {"next_level", P.next_level}, {"inventory", P.inventory},
                       {"quantity", P.quantity}, {"alive", P.alive}, {"skills", P.skills}};
}

void from_json(const nlohmann::json& j, Player& P) {
    P.name = j.at("name").get<std::string>();
    P.health = j.at("health").get<int>();
    P.max_health = j.at("max_health").get<int>();
    P.weapon = j.at("weapon").get<Item>();
    P.armor = j.at("armor").get<Item>();
    P.experience = j.at("experience").get<int>();
    P.level = j.at("level").get<int>();
    P.next_level = j.at("next_level").get<int>();
    P.inventory = j.at("inventory").get<std::vector<Item> >();
    P.quantity = j.at("quantity").get<std::vector<int> >();
    P.alive = j.at("alive").get<bool>();
    P.skills = j.at("skills").get<std::unordered_map<std::string, int> >();
}

void to_json(nlohmann::json& j, const Dungeon& D) {
    j = nlohmann::json{{"name", D.name}, {"loot", D.loot}, {"enemy", D.enemy}, {"description", D.description}};
}

void from_json(const nlohmann::json& j, Dungeon& D) {
    D.name = j.at("name").get<std::string>();
    D.loot = j.at("loot").get<Item>();
    D.enemy = j.at("enemy").get<std::string>();
    D.description = j.at("description").get<std::string>();
}

void to_json(nlohmann::json& j, const Enemy& E){
  j = nlohmann::json{{"object", "Enemy"}, {"name", E.name}, {"health", E.health},
                     {"weapon", E.weapon}, {"armor", E.armor},
                     {"max_health", E.max_health}, {"experience", E.experience},
                     {"loot", E.loot}, {"loot_rate", E.loot_rate}, {"description", E.description}};
}

void from_json(const nlohmann::json& j, Enemy& E){
  E.name = j.at("name").get<std::string>();
  E.health = j.at("health").get<int>();
  E.max_health = j.at("max_health").get<int>();
  E.weapon = j.at("weapon").get<Item>();
  E.armor = j.at("armor").get<Item>();
  E.experience = j.at("experience").get<int>();
  E.loot = j.at("loot").get<Item>();
  E.loot_rate = j.at("loot_rate").get<float>();
  E.description = j.at("description").get<std::string>();
  E.skills = j.at("skills").get<std::unordered_map<std::string, int> >();
}
