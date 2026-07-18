#pragma once

// Spiel-Ereignisse, auf die andere Systeme (Partikel, Sound, spaeter HUD)
// reagieren koennen, OHNE dass InteractionSystem/PlayerStates diese Systeme
// selbst kennen muessen. Neues Feature reagiert auf ein bestehendes Ereignis
// = neuer subscribe()-Aufruf, keine Aenderung an der Stelle, die das Ereignis ausloest.
enum class EventType {
    PlayerJumped,
    PlayerLanded,
    PlayerDamaged,
    PlayerDied,
    PlayerPoweredUp,
    EnemyStomped,
    CoinCollected,
    FireballFired,
    LevelCompleted,
};
