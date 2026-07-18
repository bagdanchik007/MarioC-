#pragma once
#include <vector>
#include <string>
#include <SFML/Graphics.hpp>
#include "Tilemap.h"

// Level: fasst alles zusammen, was einen Spielabschnitt ausmacht.
// Aktuell nur die Tilemap + Spawnpunkt + Weltgrenzen; in Etappe 3/4 kommen
// Enemies, Items und Trigger dazu. Level ist der EINZIGE Ort, der weiss WIE
// ein Level aufgebaut ist - Game kennt nur dieses Interface, nicht die
// Rohdaten (Layout-Strings, Parsing-Details etc.).
class Level {
public:
    Level(std::vector<std::string> layout, float tileSize);

    const Tilemap& getTilemap() const noexcept { return m_tilemap; }
    sf::Vector2f getPlayerSpawn() const noexcept { return m_playerSpawn; }
    sf::FloatRect getWorldBounds() const noexcept { return m_worldBounds; }

    void render(sf::RenderWindow& window) const;

private:
    Tilemap m_tilemap;
    sf::Vector2f m_playerSpawn;
    sf::FloatRect m_worldBounds;
};
