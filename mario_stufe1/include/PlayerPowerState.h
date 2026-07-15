#pragma once

// Power-Level des Spielers. Downgrade-Reihenfolge bei Kontaktschaden:
// Fire -> Big -> Small -> Tod (siehe Player::takeDamage).
enum class PowerState {
    Small,
    Big,
    Fire,
};
