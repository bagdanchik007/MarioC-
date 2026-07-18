#pragma once
#include "ICommand.h"

// Konkrete Commands. Jeder Command ruft nur eine einzige Player-Aktion auf,
// die eigentliche Logik (was "Jump" bedeutet) lebt im Player/State, nicht hier.
class MoveLeftCommand final : public ICommand {
public:
    void execute(Player& player) const override;
};

class MoveRightCommand final : public ICommand {
public:
    void execute(Player& player) const override;
};

class StopHorizontalCommand final : public ICommand {
public:
    void execute(Player& player) const override;
};

class JumpCommand final : public ICommand {
public:
    void execute(Player& player) const override;
};

class JumpReleaseCommand final : public ICommand {
public:
    void execute(Player& player) const override;
};

class AttackCommand final : public ICommand {
public:
    void execute(Player& player) const override;
};
