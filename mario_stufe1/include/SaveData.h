#pragma once
#include <string>

// Datencontainer fuer einen Spielstand. Bewusst simpel gehalten (kein Level-
// Fortschritt/Inventar-System noetig bei nur einem Level) - waechst das
// Projekt um mehrere Level, kaeme hier z.B. "currentLevelId" dazu.
struct SaveData {
    int score = 0;
    int coinCount = 0;
    int lives = 3;
    std::string powerState = "small"; // "small" | "big" | "fire"
    float playerX = 0.f;
    float playerY = 0.f;
};
