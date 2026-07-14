#pragma once
#include <SFML/Graphics.hpp>

// Kamera mit weichem Follow-Verhalten statt hartem Snap auf die Spielerposition.
// Nutzt exponentielles Lerp (frame-rate-unabhaengig) und klemmt sich an die
// Level-Grenzen, damit sie nie ausserhalb der Tilemap zeigt.
class Camera {
public:
    Camera(sf::Vector2f viewSize, sf::FloatRect worldBounds);

    void follow(sf::Vector2f targetPosition, float deltaTime);
    const sf::View& getView() const noexcept { return m_view; }

    void setWorldBounds(sf::FloatRect bounds) noexcept { m_worldBounds = bounds; }

private:
    sf::View m_view;
    sf::FloatRect m_worldBounds;
    static constexpr float SMOOTHING = 5.f; // hoeher = schnelleres Einholen des Ziels
};
