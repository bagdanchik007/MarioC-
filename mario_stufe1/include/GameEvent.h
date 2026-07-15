#pragma once
#include <SFML/System/Vector2.hpp>
#include "EventType.h"

// Ein einzelnes Ereignis. Bewusst EIN generischer Struct statt einer Klasse
// pro Ereignistyp - fuer ein Spiel dieser Groesse reicht "Typ + Position +
// optionaler Wert" fuer alle Ereignisse aus (Position fuer Partikel/Sound-Ort,
// value z.B. fuer Punktwert). Waechst der Bedarf (Etappe 6+), waere ein
// std::variant-Payload pro Typ der naechste Schritt.
struct GameEvent {
    EventType type;
    sf::Vector2f position{};
    int value{0};
};
