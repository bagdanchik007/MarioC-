#pragma once
#include <vector>
#include <string>
#include <SFML/Graphics.hpp>
#include "EnemyType.h"
#include "ItemType.h"

// Rohdaten eines Levels, wie sie aus einer JSON-Datei geladen werden (Etappe 6).
// Bewusst ein einfacher Datencontainer ohne Verhalten - LevelLoader befuellt
// ihn, Game/Level bauen daraus die eigentlichen Spielobjekte. Diese Trennung
// (Daten laden vs. Objekte erzeugen) ist der Kern von "Data-driven Design":
// ein neues Level braucht keine neue Codezeile, nur eine neue JSON-Datei.
struct EnemySpawn {
    EnemyType type;
    sf::Vector2f position;
};

struct ItemSpawn {
    ItemType type;
    sf::Vector2f position;
};

struct LevelData {
    float tileSize = 32.f;
    std::vector<std::string> tiles;
    std::vector<EnemySpawn> enemies;
    std::vector<ItemSpawn> items;
};
