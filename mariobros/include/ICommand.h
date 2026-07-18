#pragma once

class Player;

// Command-Interface (Command Pattern).
// Entkoppelt "was soll passieren" (Command) von "wodurch wurde es ausgeloest"
// (Taste, Gamepad-Button, Netzwerk-Event, Replay-Datei ...).
class ICommand {
public:
    virtual ~ICommand() = default;
    virtual void execute(Player& player) const = 0;
};
