#pragma once
#include <vector>
#include <memory>

class Player;
class Enemy;
class Item;
class Fireball;

// Ergebnis des Einsammelns von Items in einem Frame.
struct ItemPickupResult {
    int coinsCollected = 0;
    int scoreGained = 0;
};

// Kapselt alle Interaktionen zwischen Player/Fireball und anderen Entities.
// Eigenes System statt Interaktionslogik in Player/Enemy/Item zu verstreuen:
// "was passiert bei Kontakt" ist Spielregel-Logik, kein Bestandteil der
// beteiligten Objekte selbst (die kennen sich gegenseitig nicht).
class InteractionSystem {
public:
    // Stomp -> Enemy::squish() + Punkte + Bounce. Kontakt von der Seite ->
    // Player::takeDamage() + Knockback. Rueckgabe: in diesem Frame erzielte Punkte.
    static int resolvePlayerEnemies(Player& player, std::vector<std::unique_ptr<Enemy>>& enemies);

    // Sammelt beruehrte Items ein und wendet ihren Effekt auf den Player an
    // (Coin -> nur Punkte, Mushroom/FireFlower -> Player::applyPowerUp).
    static ItemPickupResult resolvePlayerItems(Player& player, std::vector<std::unique_ptr<Item>>& items);

    // Fireball trifft Enemy -> Enemy::squish() + Fireball verbraucht sich.
    // Rueckgabe: in diesem Frame erzielte Punkte.
    static int resolveFireballEnemies(std::vector<std::unique_ptr<Fireball>>& fireballs,
                                       std::vector<std::unique_ptr<Enemy>>& enemies);

private:
    // Stomp-Erkennung: Player bewegt sich abwaerts UND seine untere Kante
    // liegt (mit kleiner Toleranz) an oder ueber der oberen Kante des Gegners.
    static bool isStomp(const Player& player, const Enemy& enemy);
};
