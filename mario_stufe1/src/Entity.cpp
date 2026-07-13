#include "../include/Entity.h"

Entity::Entity(sf::Vector2f position)
    : m_position(position) {}

void Entity::updateBounds(sf::Vector2f size) noexcept {
    m_bounds = sf::FloatRect(m_position, size);
}
