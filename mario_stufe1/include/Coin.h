#pragma once
#include <SFML/Graphics.hpp>
#include "Entity.h"

// Minimaler Platzhalter-Collectible fuer Etappe 3 - demonstriert das
// Player<->Item Interaction System end-to-end. Das vollstaendige Collectible-
// System (mehrere Item-Typen, Punktewerte-Tabelle, Power-ups) folgt in
// Etappe 4; Coin bleibt hier bewusst simpel (eine Klasse, ein Verhalten).
class Coin final : public Entity {
public:
    explicit Coin(sf::Vector2f position);

    void update(float deltaTime) override;
    void render(sf::RenderWindow& window) override;

    void collect() { kill(); }

    static const sf::Vector2f SIZE;

private:
    sf::Vector2f m_basePosition;
    float m_bobPhase{0.f};
    sf::CircleShape m_shape; // Platzhalter-Grafik statt Sprite/Textur
};
