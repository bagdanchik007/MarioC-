#pragma once
#include <unordered_map>
#include <vector>
#include <functional>
#include "GameEvent.h"

// Einfacher Publish/Subscribe-Event-Bus (Observer Pattern). Entkoppelt den
// AUSLOESER eines Ereignisses (z.B. InteractionSystem bei einem Stomp) von
// den KONSUMENTEN (SoundManager spielt einen Sound, ParticleSystem erzeugt
// einen Effekt) - keine der beiden Seiten kennt die andere.
//
// Bewusst synchron und ohne Queue: publish() ruft alle Handler SOFORT auf,
// im selben Frame. Fuer ein Spiel dieser Groesse ist das einfacher zu
// debuggen als asynchrones Event-Queuing und race-condition-frei, da alles
// single-threaded laeuft.
class EventBus {
public:
    using Handler = std::function<void(const GameEvent&)>;

    void subscribe(EventType type, Handler handler);
    void publish(const GameEvent& event) const;

private:
    std::unordered_map<EventType, std::vector<Handler>> m_handlers;
};
