#include "../include/LevelLoader.h"
#include <nlohmann/json.hpp>
#include <fstream>

namespace {

using json = nlohmann::json;

bool parseEnemyType(const std::string& name, EnemyType& out) {
    if (name == "goomba") {
        out = EnemyType::Goomba;
        return true;
    }
    return false; // Unbekannter Typ - wird von loadFromFile uebersprungen, nicht abgestuerzt.
}

bool parseItemType(const std::string& name, ItemType& out) {
    if (name == "coin") {
        out = ItemType::Coin;
        return true;
    }
    if (name == "mushroom") {
        out = ItemType::Mushroom;
        return true;
    }
    if (name == "fireflower") {
        out = ItemType::FireFlower;
        return true;
    }
    return false;
}

} // namespace

bool LevelLoader::loadFromFile(const std::string& filepath, LevelData& outData) {
    std::ifstream file(filepath);
    if (!file.is_open()) {
        return false; // Datei fehlt - erwarteter Fall, kein Fehlerzustand.
    }

    json root;
    try {
        file >> root;
    } catch (const json::parse_error&) {
        return false; // Ungueltiges JSON - ebenfalls kein Crash, nur Fallback beim Aufrufer.
    }

    LevelData data;
    data.tileSize = root.value("tileSize", 32.f);

    if (!root.contains("tiles") || !root["tiles"].is_array()) {
        return false; // Ohne Tiles ist das Level unbrauchbar.
    }
    for (const auto& row : root["tiles"]) {
        data.tiles.push_back(row.get<std::string>());
    }

    if (root.contains("enemies")) {
        for (const auto& enemyJson : root["enemies"]) {
            EnemyType type;
            if (!parseEnemyType(enemyJson.value("type", ""), type)) {
                continue; // Unbekannter Enemy-Typ - ueberspringen statt abzustuerzen.
            }
            const float x = enemyJson.value("x", 0.f) * data.tileSize;
            const float y = enemyJson.value("y", 0.f) * data.tileSize;
            data.enemies.push_back({type, sf::Vector2f(x, y)});
        }
    }

    if (root.contains("items")) {
        for (const auto& itemJson : root["items"]) {
            ItemType type;
            if (!parseItemType(itemJson.value("type", ""), type)) {
                continue;
            }
            const float x = itemJson.value("x", 0.f) * data.tileSize;
            const float y = itemJson.value("y", 0.f) * data.tileSize;
            data.items.push_back({type, sf::Vector2f(x, y)});
        }
    }

    outData = std::move(data);
    return true;
}
