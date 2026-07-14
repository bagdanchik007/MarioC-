#include "../include/Camera.h"
#include <algorithm>
#include <cmath>

Camera::Camera(sf::Vector2f viewSize, sf::FloatRect worldBounds)
    : m_view(sf::FloatRect(0.f, 0.f, viewSize.x, viewSize.y))
    , m_worldBounds(worldBounds) {
    m_view.setCenter(viewSize.x / 2.f, viewSize.y / 2.f);
}

void Camera::follow(sf::Vector2f targetPosition, float deltaTime) {
    const sf::Vector2f currentCenter = m_view.getCenter();

    // Exponentielles Lerp: t haengt von deltaTime ab, daher konvergiert die
    // Kamera bei 30 FPS und bei 144 FPS gleich schnell zum Ziel - ein fixer
    // Lerp-Faktor pro Frame (z.B. 0.1) waere framerate-abhaengig und damit falsch.
    const float t = 1.f - std::exp(-SMOOTHING * deltaTime);
    sf::Vector2f newCenter = currentCenter + (targetPosition - currentCenter) * t;

    const sf::Vector2f halfSize = m_view.getSize() / 2.f;
    const float minX = m_worldBounds.left + halfSize.x;
    const float maxX = m_worldBounds.left + m_worldBounds.width - halfSize.x;
    const float minY = m_worldBounds.top + halfSize.y;
    const float maxY = m_worldBounds.top + m_worldBounds.height - halfSize.y;

    // Falls das Level kleiner als die View ist, wuerde min > max gelten -
    // in dem Fall wird zentriert statt geklemmt, um Flackern zu vermeiden.
    newCenter.x = (minX <= maxX) ? std::clamp(newCenter.x, minX, maxX)
                                  : (m_worldBounds.left + m_worldBounds.width / 2.f);
    newCenter.y = (minY <= maxY) ? std::clamp(newCenter.y, minY, maxY)
                                  : (m_worldBounds.top + m_worldBounds.height / 2.f);

    m_view.setCenter(newCenter);
}
