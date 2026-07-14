#pragma once
#include <SFML/Graphics.hpp>

// Basisklasse fuer alle Spielobjekte (Entities).
// Jede Entity hat eine Position, eine Geschwindigkeit, eine feste Groesse und
// eine daraus abgeleitete Bounding-Box. Die Bounding-Box wird IMMER automatisch
// aktualisiert (setPosition/move/setSize) - das ist wichtig fuer Etappe 2:
// das CollisionSystem verlaesst sich darauf, dass getBounds() nach jedem
// setPosition() sofort korrekt ist, ohne dass jemand manuell "updateBounds" ruft.
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
    sf::Vector2f getSize() const noexcept { return m_size; }
    const sf::FloatRect& getBounds() const noexcept { return m_bounds; }

    void setPosition(const sf::Vector2f& position) noexcept;
    void setVelocity(const sf::Vector2f& velocity) noexcept { m_velocity = velocity; }
    void move(const sf::Vector2f& offset) noexcept;

    bool isAlive() const noexcept { return m_alive; }
    void kill() noexcept { m_alive = false; }

protected:
    // Wird von abgeleiteten Klassen einmalig gesetzt (i.d.R. im Konstruktor).
    void setSize(sf::Vector2f size) noexcept;

    sf::Vector2f m_position;
    sf::Vector2f m_velocity{0.f, 0.f};
    sf::Vector2f m_size{0.f, 0.f};
    sf::FloatRect m_bounds;
    bool m_alive{true};

private:
    void refreshBounds() noexcept;
};
