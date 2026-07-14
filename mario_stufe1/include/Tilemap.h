#pragma once
#include <vector>
#include <string>
#include <SFML/Graphics.hpp>
#include "TileType.h"

// Tilemap: rasterbasierte Repraesentation eines Levels als 2D-Grid von TileType.
// Bewusst OHNE Texturen/Sprites - reine Kollisions-/Logikdaten (Single
// Responsibility). Das Rendering hier ist nur ein Platzhalter aus farbigen
// Rechtecken; ein echtes Tileset kommt spaeter dazu, ohne dass sich an der
// Datenstruktur oder am CollisionSystem etwas aendern muss.
class Tilemap {
public:
    Tilemap(std::vector<std::string> layout, float tileSize);

    TileType getTile(int col, int row) const;
    bool isSolidAt(int col, int row) const;

    int getWidth() const noexcept { return m_width; }
    int getHeight() const noexcept { return m_height; }
    float getTileSize() const noexcept { return m_tileSize; }

    void render(sf::RenderWindow& window) const;

private:
    static TileType charToTileType(char c);

    std::vector<std::vector<TileType>> m_tiles; // [row][col]
    int m_width{0};
    int m_height{0};
    float m_tileSize;
};
