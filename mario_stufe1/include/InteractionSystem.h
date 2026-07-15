#pragma once
#include <vector>
#include <memory>

class Player;
class Enemy;
class Coin;

// Kapselt alle Interaktionen zwischen Player und anderen Entities.
// Bewusst ein EIGENES System statt Interaktionslogik in Player/Enemy/Coin zu
// verstreuen: "was passiert bei Kontakt" ist Spielregel-Logik, keine Physik
// (das ist CollisionSystem) und keine Objekteigenschaft (das waere in
// Player/Enemy selbst schlecht gekapselt, da beide Seiten involviert sind).
class InteractionSystem {
public:
    // Prueft den Player gegen alle lebenden Enemies. Bei Ueberlappung:
    // Stomp (Player faellt von oben) -> Enemy::squish() + kleiner Bounce,
    // sonst -> Kontaktschaden (aktuell nur Knockback, echtes Health-System
    // kommt in Etappe 4).
    static void resolvePlayerEnemies(Player& player, std::vector<std::unique_ptr<Enemy>>& enemies);

    // Prueft den Player gegen alle lebenden Coins und sammelt beruehrte ein.
    // Rueckgabe: Anzahl der in diesem Frame frisch eingesammelten Coins.
    static int resolvePlayerCoins(Player& player, std::vector<std::unique_ptr<Coin>>& coins);

private:
    // Stomp-Erkennung: Player bewegt sich abwaerts UND seine untere Kante
    // liegt (mit kleiner Toleranz) an oder ueber der oberen Kante des Gegners.
    static bool isStomp(const Player& player, const Enemy& enemy);
};
