#include "../include/Item.h"
#include <cmath>

namespace {

sf::Color colorFor(ItemType type) {
    switch (type) {
        case ItemType::Coin:       return sf::Color(255, 215, 0);
        case ItemType::Mushroom:   return sf::Color(220, 40, 40);
        case ItemType::FireFlower: return sf::Color(255, 120, 0);
    }
    return sf::Color::White;
}

sf::Vector2f sizeFor(ItemType type) {
    switch (type) {
        case ItemType::Coin:       return {12.f, 12.f};
        case ItemType::Mushroom:   return {16.f, 16.f};
        case ItemType::FireFlower: return {16.f, 16.f};
    }
    return {12.f, 12.f};
}

int scoreValueFor(ItemType type) {
    switch (type) {
        case ItemType::Coin:       return 10;
        case ItemType::Mushroom:   return 1000;
        case ItemType::FireFlower: return 1000;
    }
    return 0;
}

} // namespace

Item::Item(ItemType type, sf::Vector2f position)
    : Entity(position)
    , m_type(type)
    , m_basePosition(position)
    , m_shape(sizeFor(type)) {
    setSize(sizeFor(type));
    m_shape.setFillColor(colorFor(type));
    m_shape.setOrigin(sizeFor(type).x / 2.f, sizeFor(type).y / 2.f);
}

void Item::update(float deltaTime) {
    if (!m_alive) {
        return;
    }
    // Rein kosmetisches Schweben - Items sind statische Trigger, keine
    // dynamischen Entities mit Tilemap-Kollision.
    m_bobPhase += deltaTime * 3.f;
    sf::Vector2f pos = m_basePosition;
    pos.y += std::sin(m_bobPhase) * 4.f;
    setPosition(pos);
}

void Item::render(sf::RenderWindow& window) {
    if (!m_alive) {
        return;
    }
    const sf::Vector2f size = getSize();
    m_shape.setPosition(m_position.x + size.x / 2.f, m_position.y + size.y / 2.f);
    window.draw(m_shape);
}

int Item::getScoreValue() const noexcept {
    return scoreValueFor(m_type);
}
