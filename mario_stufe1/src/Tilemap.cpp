#include "../include/Tilemap.h"
#include <algorithm>

Tilemap::Tilemap(std::vector<std::string> layout, float tileSize)
    : m_tileSize(tileSize) {
    m_height = static_cast<int>(layout.size());
    m_width = m_height > 0 ? static_cast<int>(layout.front().size()) : 0;

    m_tiles.reserve(m_height);
    for (const auto& row : layout) {
        std::vector<TileType> tileRow;
        tileRow.reserve(m_width);
        for (char c : row) {
            tileRow.push_back(charToTileType(c));
        }
        // Fehlende Spalten am Zeilenende werden als Empty aufgefuellt, damit
        // ungleich lange Layout-Strings nicht sofort zum Absturz fuehren.
        while (static_cast<int>(tileRow.size()) < m_width) {
            tileRow.push_back(TileType::Empty);
        }
        m_tiles.push_back(std::move(tileRow));
    }
}

TileType Tilemap::charToTileType(char c) {
    switch (c) {
        case '#': return TileType::Solid;
        case '^': return TileType::Damage;
        case '?': return TileType::CoinBlock;
        default:  return TileType::Empty; // '.', 'P' (Spawn-Marker) und Unbekanntes -> leer
    }
}

TileType Tilemap::getTile(int col, int row) const {
    if (col < 0 || row < 0 || col >= m_width || row >= m_height) {
        return TileType::Empty; // Ausserhalb der Karte gilt als begehbar/leer.
    }
    return m_tiles[static_cast<std::size_t>(row)][static_cast<std::size_t>(col)];
}

bool Tilemap::isSolidAt(int col, int row) const {
    return TileProperties::isSolid(getTile(col, row));
}

void Tilemap::render(sf::RenderWindow& window) const {
    // Platzhalter-Rendering: farbige Rechtecke statt Sprites/Tileset.
    sf::RectangleShape shape(sf::Vector2f(m_tileSize, m_tileSize));

    for (int row = 0; row < m_height; ++row) {
        for (int col = 0; col < m_width; ++col) {
            const TileType type = m_tiles[static_cast<std::size_t>(row)][static_cast<std::size_t>(col)];
            if (type == TileType::Empty) {
                continue;
            }
            switch (type) {
                case TileType::Solid:     shape.setFillColor(sf::Color(139, 69, 19)); break;
                case TileType::Damage:    shape.setFillColor(sf::Color(200, 30, 30)); break;
                case TileType::CoinBlock: shape.setFillColor(sf::Color(255, 215, 0)); break;
                default: break;
            }
            shape.setPosition(static_cast<float>(col) * m_tileSize,
                               static_cast<float>(row) * m_tileSize);
            window.draw(shape);
        }
    }
}
