#pragma once
#include "PlayerAction.h"

class Player;

// State-Interface fuer die Player State Machine (klassisches GoF State Pattern).
// Jeder konkrete State kapselt: Bewegungsverhalten, Reaktion auf Input,
// und Uebergangsbedingungen zu anderen States.
class IPlayerState {
public:
    virtual ~IPlayerState() = default;

    // Wird einmalig beim Betreten/Verlassen des States aufgerufen
    // (z.B. um die passende Animation zu starten).
    virtual void enter(Player&) {}
    virtual void exit(Player&) {}

    // Physik-/Logik-Update, laeuft jeden Frame solange der State aktiv ist.
    virtual void update(Player& player, float deltaTime) = 0;

    // Reaktion auf eine diskrete Spieleraktion (z.B. Jump-Taste gedrueckt).
    virtual void handleAction(Player& player, PlayerAction action) = 0;

    virtual const char* name() const = 0; // fuer Animation-Lookup & Debugging
};
