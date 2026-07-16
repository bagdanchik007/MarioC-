#include "../include/SaveManager.h"
#include <nlohmann/json.hpp>
#include <fstream>

using json = nlohmann::json;

bool SaveManager::save(const std::string& filepath, const SaveData& data) {
    json root;
    root["score"] = data.score;
    root["coinCount"] = data.coinCount;
    root["lives"] = data.lives;
    root["powerState"] = data.powerState;
    root["playerX"] = data.playerX;
    root["playerY"] = data.playerY;

    std::ofstream file(filepath);
    if (!file.is_open()) {
        return false; // z.B. Verzeichnis existiert nicht - kein Crash, nur false.
    }
    file << root.dump(2); // 2 = eingerueckt, damit die Datei von Hand lesbar bleibt
    return true;
}

bool SaveManager::load(const std::string& filepath, SaveData& outData) {
    std::ifstream file(filepath);
    if (!file.is_open()) {
        return false; // Kein Spielstand vorhanden - erwarteter Fall (erster Start).
    }

    json root;
    try {
        file >> root;
    } catch (const json::parse_error&) {
        return false;
    }

    SaveData data;
    data.score = root.value("score", 0);
    data.coinCount = root.value("coinCount", 0);
    data.lives = root.value("lives", 3);
    data.powerState = root.value("powerState", std::string("small"));
    data.playerX = root.value("playerX", 0.f);
    data.playerY = root.value("playerY", 0.f);

    outData = data;
    return true;
}
