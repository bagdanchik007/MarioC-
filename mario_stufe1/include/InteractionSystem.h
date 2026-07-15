#pragma once
#include <vector>
#include <memory>

class Player;
class Enemy;
class Item;
class Fireball;
class EventBus;

// Ergebnis des Einsammelns von Items in einem Frame.
struct ItemPickupResult {
    int coinsCollected = 0;
    int scoreGained = 0;
};

// Kapselt alle Interaktionen zwischen Player/Fireball und anderen Entities.
// Eigenes System statt Interaktionslogik in Player/Enemy/Item zu verstreuen:
// "was passiert bei Kontakt" ist Spielregel-Logik, kein Bestandteil der
// beteiligten Objekte selbst (die kennen sich gegenseitig nicht).
//
// Etappe 5: publiziert zusaetzlich GameEvents (ueber den uebergebenen
// EventBus) fuer alles, was Sound/Partikel ausloesen soll - InteractionSystem
// selbst kennt weder SoundManager noch ParticleSystem, nur den Bus.
class InteractionSystem {
public:
    static int resolvePlayerEnemies(Player& player, std::vector<std::unique_ptr<Enemy>>& enemies,
                                     EventBus& events);

    static ItemPickupResult resolvePlayerItems(Player& player, std::vector<std::unique_ptr<Item>>& items,
                                                EventBus& events);

    static int resolveFireballEnemies(std::vector<std::unique_ptr<Fireball>>& fireballs,
                                       std::vector<std::unique_ptr<Enemy>>& enemies,
                                       EventBus& events);

private:
    static bool isStomp(const Player& player, const Enemy& enemy);
};
