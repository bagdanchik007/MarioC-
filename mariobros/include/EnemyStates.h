#pragma once
#include "IEnemyState.h"

// PatrolState: laeuft geradeaus, dreht bei Wandkontakt um. Absichtlich KEINE
// Kantenerkennung (Goombas laufen im Original auch von Plattformen runter) -
// das ist eine bewusste Design-Entscheidung, kein vergessenes Feature.
class PatrolState final : public IEnemyState {
public:
    void enter(Enemy& enemy) override;
    void update(Enemy& enemy, float deltaTime) override;
    const char* name() const override { return "patrol"; }
};

// SquishedState: kurze Animation nach einem Stomp, danach kill().
// Kein Zurueck zu Patrol - dieser Zustand ist final fuer das Enemy-Leben.
class SquishedState final : public IEnemyState {
public:
    void enter(Enemy& enemy) override;
    void update(Enemy& enemy, float deltaTime) override;
    const char* name() const override { return "squished"; }

private:
    float m_elapsed{0.f};
    static constexpr float DURATION = 0.5f;
};
