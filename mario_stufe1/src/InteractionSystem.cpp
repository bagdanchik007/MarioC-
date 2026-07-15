#include "../include/InteractionSystem.h"
#include "../include/Player.h"
#include "../include/Enemy.h"
#include "../include/Item.h"
#include "../include/Fireball.h"

namespace {
// Pixel-Toleranz: wie tief der Spieler beim Stomp schon "eingesunken" sein
// darf und trotzdem noch als Stomp (statt Kontaktschaden von der Seite) zaehlt.
constexpr float STOMP_TOLERANCE = 6.f;
}

int InteractionSystem::resolvePlayerEnemies(Player& player, std::vector<std::unique_ptr<Enemy>>& enemies) {
    int scoreGained = 0;
    if (!player.isAlive()) {
        return scoreGained;
    }

    for (auto& enemyPtr : enemies) {
        Enemy& enemy = *enemyPtr;
        if (!enemy.isAlive()) {
            continue;
        }

        sf::FloatRect intersection;
        if (!player.getBounds().intersects(enemy.getBounds(), intersection)) {
            continue;
        }

        if (isStomp(player, enemy)) {
            scoreGained += enemy.getScoreValue();
            enemy.squish();
            // Klassischer Mario-Bounce nach dem Stomp: direkt ueber setVelocity,
            // NICHT ueber handleAction(Jump) - das wuerde den vollen Jump-State
            // triggern (inkl. Sprung-Animation), obwohl der Spieler gerade fiel.
            player.setVelocity({player.getVelocity().x, -250.f});
        } else if (!player.isInvulnerable()) {
            const bool died = player.takeDamage();
            const float pushDirection = (player.getPosition().x < enemy.getPosition().x) ? -1.f : 1.f;
            player.setVelocity({pushDirection * 200.f, -150.f});
            if (died) {
                // Player als Entity "tot" markieren - PlayingState reagiert im
                // naechsten Frame (Leben abziehen, respawn oder Game Over).
                player.kill();
            }
        }
    }
    return scoreGained;
}

ItemPickupResult InteractionSystem::resolvePlayerItems(Player& player, std::vector<std::unique_ptr<Item>>& items) {
    ItemPickupResult result;
    for (auto& itemPtr : items) {
        Item& item = *itemPtr;
        if (!item.isAlive()) {
            continue;
        }
        sf::FloatRect intersection;
        if (!player.getBounds().intersects(item.getBounds(), intersection)) {
            continue;
        }

        if (item.getType() == ItemType::Coin) {
            ++result.coinsCollected;
        } else {
            player.applyPowerUp(item.getType());
        }
        result.scoreGained += item.getScoreValue();
        item.collect();
    }
    return result;
}

int InteractionSystem::resolveFireballEnemies(std::vector<std::unique_ptr<Fireball>>& fireballs,
                                               std::vector<std::unique_ptr<Enemy>>& enemies) {
    int scoreGained = 0;
    for (auto& fireballPtr : fireballs) {
        Fireball& fireball = *fireballPtr;
        if (!fireball.isAlive()) {
            continue;
        }
        for (auto& enemyPtr : enemies) {
            Enemy& enemy = *enemyPtr;
            if (!enemy.isAlive()) {
                continue;
            }
            sf::FloatRect intersection;
            if (fireball.getBounds().intersects(enemy.getBounds(), intersection)) {
                scoreGained += enemy.getScoreValue();
                enemy.squish();
                fireball.kill(); // Fireball verbraucht sich bei einem Treffer
                break;           // dieser Fireball hat schon getroffen, naechster Fireball
            }
        }
    }
    return scoreGained;
}

bool InteractionSystem::isStomp(const Player& player, const Enemy& enemy) {
    const float playerBottom = player.getBounds().top + player.getBounds().height;
    const float enemyTop = enemy.getBounds().top;
    return player.getVelocity().y > 0.f && playerBottom <= enemyTop + STOMP_TOLERANCE;
}
