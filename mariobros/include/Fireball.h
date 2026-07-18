#pragma once
#include <SFML/Graphics.hpp>
#include "Entity.h"

// Fireball: Projektil, das Fire-Mario beim Angriff verschiesst. Fliegt
// geradeaus - KEINE Gravitation, KEINE Tilemap-Kollision (das waere Politur
// fuer Etappe 5, z.B. Abprallen vom Boden). Despawnt nach fester Lebenszeit
// oder bei Treffer (siehe InteractionSystem::resolveFireballEnemies).
class Fireball final : public Entity {
public:
    Fireball(sf::Vector2f position, float directionX);

    void update(float deltaTime) override;
    void render(sf::RenderWindow& window) override;

    static const sf::Vector2f SIZE;

private:
    sf::CircleShape m_shape;
    float m_lifetime{0.f};
    static constexpr float MAX_LIFETIME = 1.2f;
    static constexpr float SPEED = 400.f;
};
