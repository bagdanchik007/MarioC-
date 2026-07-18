#include "../include/InteractionSystem.h"
#include "../include/Player.h"
#include "../include/Enemy.h"
#include "../include/Item.h"
#include "../include/Fireball.h"
#include "../include/EventBus.h"
#include "../include/GameEvent.h"

namespace {
constexpr float STOMP_TOLERANCE = 6.f;
}

int InteractionSystem::resolvePlayerEnemies(Player& player, std::vector<std::unique_ptr<Enemy>>& enemies,
                                             EventBus& events) {
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
            events.publish({EventType::EnemyStomped, enemy.getPosition(), enemy.getScoreValue()});
            enemy.squish();
            player.setVelocity({player.getVelocity().x, -250.f});
        } else if (!player.isInvulnerable()) {
            const bool died = player.takeDamage();
            const float pushDirection = (player.getPosition().x < enemy.getPosition().x) ? -1.f : 1.f;
            player.setVelocity({pushDirection * 200.f, -150.f});

            if (died) {
                events.publish({EventType::PlayerDied, player.getPosition(), 0});
                player.kill();
            } else {
                events.publish({EventType::PlayerDamaged, player.getPosition(), 0});
            }
        }
    }
    return scoreGained;
}

ItemPickupResult InteractionSystem::resolvePlayerItems(Player& player, std::vector<std::unique_ptr<Item>>& items,
                                                        EventBus& events) {
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
            events.publish({EventType::CoinCollected, item.getPosition(), item.getScoreValue()});
        } else {
            player.applyPowerUp(item.getType());
            events.publish({EventType::PlayerPoweredUp, item.getPosition(), 0});
        }
        result.scoreGained += item.getScoreValue();
        item.collect();
    }
    return result;
}

int InteractionSystem::resolveFireballEnemies(std::vector<std::unique_ptr<Fireball>>& fireballs,
                                               std::vector<std::unique_ptr<Enemy>>& enemies,
                                               EventBus& events) {
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
                events.publish({EventType::EnemyStomped, enemy.getPosition(), enemy.getScoreValue()});
                enemy.squish();
                fireball.kill();
                break;
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
