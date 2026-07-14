#include "../include/CollisionSystem.h"
#include "../include/Entity.h"
#include <cmath>

bool CollisionSystem::resolve(Entity& entity, const Tilemap& tilemap, float deltaTime) {
    resolveAxisX(entity, tilemap, deltaTime);
    return resolveAxisY(entity, tilemap, deltaTime);
}

void CollisionSystem::resolveAxisX(Entity& entity, const Tilemap& tilemap, float deltaTime) {
    sf::Vector2f pos = entity.getPosition();
    pos.x += entity.getVelocity().x * deltaTime;
    entity.setPosition(pos);

    const float tileSize = tilemap.getTileSize();
    sf::FloatRect bounds = entity.getBounds();

    const int firstCol = static_cast<int>(std::floor(bounds.left / tileSize));
    const int lastCol  = static_cast<int>(std::floor((bounds.left + bounds.width) / tileSize));
    const int firstRow = static_cast<int>(std::floor(bounds.top / tileSize));
    const int lastRow  = static_cast<int>(std::floor((bounds.top + bounds.height) / tileSize));

    for (int row = firstRow; row <= lastRow; ++row) {
        for (int col = firstCol; col <= lastCol; ++col) {
            if (!tilemap.isSolidAt(col, row)) {
                continue;
            }
            const sf::FloatRect tileBounds(static_cast<float>(col) * tileSize,
                                            static_cast<float>(row) * tileSize,
                                            tileSize, tileSize);
            sf::FloatRect intersection;
            if (!bounds.intersects(tileBounds, intersection)) {
                continue;
            }

            const sf::Vector2f velocity = entity.getVelocity();
            if (velocity.x > 0.f) {
                pos.x -= intersection.width;  // Kollision von links -> nach links rausschieben
            } else if (velocity.x < 0.f) {
                pos.x += intersection.width;  // Kollision von rechts -> nach rechts rausschieben
            }
            entity.setPosition(pos);
            entity.setVelocity({0.f, velocity.y});
            bounds = entity.getBounds(); // Bounds nach Korrektur neu holen fuer weitere Tile-Checks
        }
    }
}

bool CollisionSystem::resolveAxisY(Entity& entity, const Tilemap& tilemap, float deltaTime) {
    sf::Vector2f pos = entity.getPosition();
    pos.y += entity.getVelocity().y * deltaTime;
    entity.setPosition(pos);

    const float tileSize = tilemap.getTileSize();
    sf::FloatRect bounds = entity.getBounds();
    bool grounded = false;

    const int firstCol = static_cast<int>(std::floor(bounds.left / tileSize));
    const int lastCol  = static_cast<int>(std::floor((bounds.left + bounds.width) / tileSize));
    const int firstRow = static_cast<int>(std::floor(bounds.top / tileSize));
    const int lastRow  = static_cast<int>(std::floor((bounds.top + bounds.height) / tileSize));

    for (int row = firstRow; row <= lastRow; ++row) {
        for (int col = firstCol; col <= lastCol; ++col) {
            if (!tilemap.isSolidAt(col, row)) {
                continue;
            }
            const sf::FloatRect tileBounds(static_cast<float>(col) * tileSize,
                                            static_cast<float>(row) * tileSize,
                                            tileSize, tileSize);
            sf::FloatRect intersection;
            if (!bounds.intersects(tileBounds, intersection)) {
                continue;
            }

            const sf::Vector2f velocity = entity.getVelocity();
            if (velocity.y > 0.f) {
                pos.y -= intersection.height; // von oben auf ein Tile gefallen -> Boden gefunden
                grounded = true;
            } else if (velocity.y < 0.f) {
                pos.y += intersection.height; // gegen die Decke gesprungen
            }
            entity.setPosition(pos);
            entity.setVelocity({velocity.x, 0.f});
            bounds = entity.getBounds();
        }
    }
    return grounded;
}
