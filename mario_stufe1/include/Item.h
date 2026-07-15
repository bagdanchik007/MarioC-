#pragma once
#include <SFML/Graphics.hpp>
#include "Entity.h"
#include "ItemType.h"

// Item: generischer Collectible-Pickup - wie bei Enemy Komposition statt
// Vererbung pro Typ. ItemType bestimmt Groesse/Farbe/Punktwert aus einer
// Tabelle in Item.cpp.
//
// Vereinfachung bewusst: echte Mario-Items (Mushroom rutscht physikalisch aus
// einem Block) sind hier statisch und schweben nur leicht wie eine Muenze -
// das volle "Block -> Item spawnt und rutscht" Verhalten ist Politur
// (Etappe 5/6), hier geht es um das Power-up-SYSTEM, nicht die Spawn-Animation.
class Item final : public Entity {
public:
    Item(ItemType type, sf::Vector2f position);

    void update(float deltaTime) override;
    void render(sf::RenderWindow& window) override;

    ItemType getType() const noexcept { return m_type; }
    int getScoreValue() const noexcept;
    void collect() { kill(); }

private:
    ItemType m_type;
    sf::Vector2f m_basePosition;
    float m_bobPhase{0.f};
    sf::RectangleShape m_shape; // Platzhalter-Grafik statt Sprite/Textur
};
