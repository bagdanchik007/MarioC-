#include "../include/Entity.h"

Entity::Entity(sf::Vector2f position)
    : m_position(position) {}

void Entity::setPosition(const sf::Vector2f& position) noexcept {
    m_position = position;
    refreshBounds();
}

void Entity::move(const sf::Vector2f& offset) noexcept {
    m_position += offset;
    refreshBounds();
}

void Entity::setSize(sf::Vector2f size) noexcept {
    m_size = size;
    refreshBounds();
}

void Entity::refreshBounds() noexcept {
    m_bounds = sf::FloatRect(m_position, m_size);
}
