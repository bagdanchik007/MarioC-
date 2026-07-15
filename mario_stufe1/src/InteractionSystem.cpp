#include "../include/InteractionSystem.h"
#include "../include/Player.h"
#include "../include/Enemy.h"
#include "../include/Coin.h"

namespace {
// Pixel-Toleranz: wie tief der Spieler beim Stomp schon "eingesunken" sein
// darf und trotzdem noch als Stomp (statt Kontaktschaden von der Seite) zaehlt.
constexpr float STOMP_TOLERANCE = 6.f;
}

void InteractionSystem::resolvePlayerEnemies(Player& player, std::vector<std::unique_ptr<Enemy>>& enemies) {
    if (!player.isAlive()) {
        return;
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
            enemy.squish();
            // Klassischer Mario-Bounce nach dem Stomp: direkt ueber setVelocity,
            // NICHT ueber handleAction(Jump) - das wuerde den vollen Jump-State
            // triggern (inkl. Sprung-Animation), obwohl der Spieler gerade fiel.
            player.setVelocity({player.getVelocity().x, -250.f});
        } else {
            // Kontaktschaden von der Seite/unten: Etappe 4 bringt ein echtes
            // Health/Lives-System. Bis dahin nur ein sichtbarer Knockback,
            // damit Kontakt spuerbar und testbar ist.
            const float pushDirection = (player.getPosition().x < enemy.getPosition().x) ? -1.f : 1.f;
            player.setVelocity({pushDirection * 200.f, -150.f});
        }
    }
}

int InteractionSystem::resolvePlayerCoins(Player& player, std::vector<std::unique_ptr<Coin>>& coins) {
    int collected = 0;
    for (auto& coinPtr : coins) {
        Coin& coin = *coinPtr;
        if (!coin.isAlive()) {
            continue;
        }
        sf::FloatRect intersection;
        if (player.getBounds().intersects(coin.getBounds(), intersection)) {
            coin.collect();
            ++collected;
        }
    }
    return collected;
}

bool InteractionSystem::isStomp(const Player& player, const Enemy& enemy) {
    const float playerBottom = player.getBounds().top + player.getBounds().height;
    const float enemyTop = enemy.getBounds().top;
    return player.getVelocity().y > 0.f && playerBottom <= enemyTop + STOMP_TOLERANCE;
}
