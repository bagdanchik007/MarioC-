#pragma once

// Abstrahierte Spieleraktionen. Der InputHandler uebersetzt Rohtasten in diese
// Aktionen, sodass die Spiellogik (Player, States) nie etwas von sf::Keyboard weiss.
enum class PlayerAction {
    MoveLeft,
    MoveRight,
    StopHorizontal,
    Jump,
    JumpRelease,
    Attack,
    Crouch,
    CrouchRelease
};
