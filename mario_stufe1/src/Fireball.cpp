#include "../include/Fireball.h"

const sf::Vector2f Fireball::SIZE = {8.f, 8.f};

Fireball::Fireball(sf::Vector2f position, float directionX)
    : Entity(position)
    , m_shape(SIZE.x / 2.f) {
    setSize(SIZE);
    setVelocity({directionX * SPEED, 0.f});
    m_shape.setFillColor(sf::Color(255, 90, 0));
    m_shape.setOrigin(SIZE.x / 2.f, SIZE.y / 2.f);
}

void Fireball::update(float deltaTime) {
    if (!m_alive) {
        return;
    }
    move(getVelocity() * deltaTime);
    m_lifetime += deltaTime;
    if (m_lifetime >= MAX_LIFETIME) {
        kill();
    }
}

void Fireball::render(sf::RenderWindow& window) {
    if (!m_alive) {
        return;
    }
    m_shape.setPosition(m_position.x + SIZE.x / 2.f, m_position.y + SIZE.y / 2.f);
    window.draw(m_shape);
}
