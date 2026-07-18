#pragma once
#include <memory>
#include <SFML/Graphics.hpp>
#include "Entity.h"
#include "IEnemyState.h"
#include "EnemyType.h"
#include "AnimationController.h"

// Enemy: Basis fuer ALLE Gegnertypen - keine Vererbungshierarchie pro Typ
// (kein "class Goomba : public Enemy"). Stattdessen Komposition: EnemyType
// waehlt typ-spezifisches Tuning (Geschwindigkeit, Groesse, Punktwert) aus
// einer Tabelle in Enemy.cpp aus. Ein neuer Gegnertyp = neuer Enum-Wert +
// Tabelleneintrag, keine neue Klasse.
//
// Warum nicht Vererbung (Goomba/Koopa/... : public Enemy)?
// Bei 2-3 Gegnertypen waere Vererbung genauso gut. Mit mehr Typen wird die
// Hierarchie schnell unuebersichtlich, sobald Verhalten quer zu den Typen
// geteilt werden soll (z.B. "fliegend" + "schiesst" + "patrouilliert" in
// beliebiger Kombination) - klassisches Diamond-Problem. Die hier gewaehlte
// Komposition (Typ + State Machine + Tuning-Tabelle) skaliert dafuer besser
// und bleibt konsistent mit dem State-Pattern-Ansatz von Player.
class Enemy final : public Entity {
public:
    Enemy(EnemyType type, sf::Vector2f startPosition);

    void update(float deltaTime) override;
    void render(sf::RenderWindow& window) override;

    void setState(std::unique_ptr<IEnemyState> newState);
    const char* getStateName() const;

    EnemyType getType() const noexcept { return m_type; }

    void applyGravity(float deltaTime);
    void patrol();     // bewegt sich in aktuelle Blickrichtung mit typ-spezifischer Geschwindigkeit
    void turnAround();

    bool isOnGround() const noexcept { return m_onGround; }
    void setOnGround(bool onGround) noexcept { m_onGround = onGround; }
    bool hasHitWall() const noexcept { return m_hitWall; }
    void setHitWall(bool hit) noexcept { m_hitWall = hit; }

    int getScoreValue() const noexcept;

    // Wird vom InteractionSystem bei einem Stomp aufgerufen. Idempotent -
    // mehrfaches Aufrufen (z.B. durch Ueberlappung mehrerer Frames) hat
    // keinen zusaetzlichen Effekt.
    void squish();

    AnimationController& getAnimationController() noexcept { return m_animationController; }

private:
    EnemyType m_type;
    std::unique_ptr<IEnemyState> m_currentState;
    AnimationController m_animationController;
    sf::Sprite m_sprite;

    bool m_onGround{false};
    bool m_hitWall{false};
    bool m_facingRight{false};
    bool m_squished{false};

    float m_walkSpeed;
};
