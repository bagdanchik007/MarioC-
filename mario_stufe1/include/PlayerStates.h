#pragma once
#include "IPlayerState.h"

// Konkrete States. Jede Klasse ist bewusst klein: sie kennt nur ihre eigenen
// Uebergaenge, keine globale Zustandslogik ("if state == X") irgendwo sonst im Code.

class IdleState final : public IPlayerState {
public:
    void enter(Player& player) override;
    void update(Player& player, float deltaTime) override;
    void handleAction(Player& player, PlayerAction action) override;
    const char* name() const override { return "idle"; }
};

class RunState final : public IPlayerState {
public:
    void enter(Player& player) override;
    void update(Player& player, float deltaTime) override;
    void handleAction(Player& player, PlayerAction action) override;
    const char* name() const override { return "run"; }
};

class JumpState final : public IPlayerState {
public:
    void enter(Player& player) override;
    void update(Player& player, float deltaTime) override;
    void handleAction(Player& player, PlayerAction action) override;
    const char* name() const override { return "jump"; }
};

class FallState final : public IPlayerState {
public:
    void enter(Player& player) override;
    void exit(Player& player) override;
    void update(Player& player, float deltaTime) override;
    void handleAction(Player& player, PlayerAction action) override;
    const char* name() const override { return "fall"; }
};

class AttackState final : public IPlayerState {
public:
    void enter(Player& player) override;
    void update(Player& player, float deltaTime) override;
    void handleAction(Player& player, PlayerAction action) override;
    const char* name() const override { return "attack"; }

private:
    float m_elapsed{0.f};
    static constexpr float DURATION = 0.35f; // Angriffsdauer in Sekunden
};
