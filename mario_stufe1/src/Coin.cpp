#include "../include/Coin.h"
#include <cmath>

const sf::Vector2f Coin::SIZE = {12.f, 12.f};

Coin::Coin(sf::Vector2f position)
    : Entity(position)
    , m_basePosition(position)
    , m_shape(SIZE.x / 2.f) {
    setSize(SIZE);
    m_shape.setFillColor(sf::Color(255, 215, 0));
    m_shape.setOrigin(SIZE.x / 2.f, SIZE.y / 2.f);
}

void Coin::update(float deltaTime) {
    if (!m_alive) {
        return;
    }
    // Rein kosmetisches Auf-und-Ab-Schweben, keine Physik/Kollision noetig -
    // Coins sind statische Trigger, keine dynamischen Entities.
    m_bobPhase += deltaTime * 3.f;
    sf::Vector2f pos = m_basePosition;
    pos.y += std::sin(m_bobPhase) * 4.f;
    setPosition(pos);
}

void Coin::render(sf::RenderWindow& window) {
    if (!m_alive) {
        return;
    }
    m_shape.setPosition(m_position.x + SIZE.x / 2.f, m_position.y + SIZE.y / 2.f);
    window.draw(m_shape);
}
