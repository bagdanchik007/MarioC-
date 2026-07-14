#include "../include/Level.h"

namespace {

// Sucht den Spawn-Marker 'P' im rohen Layout. Getrennt von Tilemap, da 'P'
// kein echter Tile-Typ ist, sondern reine Platzierungsinformation fuer den Player.
sf::Vector2f findSpawnPoint(const std::vector<std::string>& layout, float tileSize) {
    for (std::size_t row = 0; row < layout.size(); ++row) {
        for (std::size_t col = 0; col < layout[row].size(); ++col) {
            if (layout[row][col] == 'P') {
                return sf::Vector2f(static_cast<float>(col) * tileSize,
                                     static_cast<float>(row) * tileSize);
            }
        }
    }
    return sf::Vector2f(tileSize, tileSize); // Fallback, falls kein 'P' vorhanden ist.
}

} // namespace

Level::Level(std::vector<std::string> layout, float tileSize)
    : m_tilemap(layout, tileSize)
    , m_playerSpawn(findSpawnPoint(layout, tileSize)) {
    m_worldBounds = sf::FloatRect(
        0.f, 0.f,
        static_cast<float>(m_tilemap.getWidth()) * tileSize,
        static_cast<float>(m_tilemap.getHeight()) * tileSize);
}

void Level::render(sf::RenderWindow& window) const {
    m_tilemap.render(window);
}
