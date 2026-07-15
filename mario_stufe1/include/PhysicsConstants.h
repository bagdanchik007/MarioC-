#pragma once

// Gemeinsame Physik-Konstanten fuer alle Entities mit Gravitation (Player, Enemy).
// Zentral definiert, damit Anpassungen am Spielgefuehl (z.B. "Schwerkraft weicher
// machen") nicht an mehreren Stellen synchron gehalten werden muessen.
namespace Physics {
    constexpr float GRAVITY = 980.f;
    constexpr float MAX_FALL_SPEED = 600.f;
}
