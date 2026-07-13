#pragma once
#include <SFML/Graphics.hpp>

// Basisklasse fuer alle Spielobjekte (Entities).
// Jede Entity hat eine Position, eine Geschwindigkeit und einen Bounding-Box.
class Entity {
public:
    explicit Entity(sf::Vector2f position = {0.f, 0.f});
    virtual ~Entity() = default;

    Entity(const Entity&) = delete;
    Entity& operator=(const Entity&) = delete;
    Entity(Entity&&) = default;
    Entity& operator=(Entity&&) = default;

    virtual void update(float deltaTime) = 0;
    virtual void render(sf::RenderWindow& window) = 0;

    const sf::Vector2f& getPosition() const noexcept { return m_position; }
    const sf::Vector2f& getVelocity() const noexcept { return m_velocity; }
    const sf::FloatRect& getBounds() const noexcept { return m_bounds; }

    void setPosition(const sf::Vector2f& position) noexcept { m_position = position; }
    void setVelocity(const sf::Vector2f& velocity) noexcept { m_velocity = velocity; }
    void move(const sf::Vector2f& offset) noexcept { m_position += offset; }

    bool isAlive() const noexcept { return m_alive; }
    void kill() noexcept { m_alive = false; }

protected:
    // Aktualisiert die Bounding-Box relativ zur aktuellen Position.
    void updateBounds(sf::Vector2f size) noexcept;

    sf::Vector2f m_position;
    sf::Vector2f m_velocity{0.f, 0.f};
    sf::FloatRect m_bounds;
    bool m_alive{true};
};
