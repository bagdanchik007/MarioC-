#pragma once
#include <memory>
#include <SFML/Graphics.hpp>
#include "Entity.h"
#include "IPlayerState.h"
#include "PlayerAction.h"
#include "AnimationController.h"
#include "PhysicsConstants.h"
#include "PlayerPowerState.h"
#include "ItemType.h"

// Player: konkrete Entity mit Physik, State Machine, Animation und
// Power-up-System (Etappe 4).
class Player final : public Entity {
public:
    explicit Player(sf::Vector2f startPosition);

    void update(float deltaTime) override;
    void render(sf::RenderWindow& window) override;

    // Eingabe-Schnittstelle: Commands rufen ausschliesslich diese Methode auf.
    void handleAction(PlayerAction action);

    // State Machine Steuerung - wird von den States selbst aufgerufen (self-transition).
    void setState(std::unique_ptr<IPlayerState> newState);
    const char* getStateName() const;

    // Physik-Hilfsfunktionen, die von den States genutzt werden.
    void applyGravity(float deltaTime);
    void jump();
    void cutJumpShort(); // variable Sprunghoehe: Taste losgelassen -> Aufstieg kappen
    void moveHorizontal(float direction); // -1 links, 0 stop, 1 rechts
    void stopHorizontal();

    bool isOnGround() const noexcept { return m_onGround; }
    void setOnGround(bool onGround) noexcept { m_onGround = onGround; }
    bool isFacingRight() const noexcept { return m_facingRight; }

    // --- Power-up-System (Etappe 4) ---
    PowerState getPowerState() const noexcept { return m_powerState; }
    void setPowerState(PowerState newState); // passt auch die Hitbox-Groesse an
    void applyPowerUp(ItemType type);        // von InteractionSystem bei Item-Pickup aufgerufen

    // Kontaktschaden. Rueckgabe true = dieser Treffer bedeutet den Tod
    // (Spieler war bereits Small), sonst nur Downgrade + kurze Unverwundbarkeit.
    bool takeDamage();
    bool isInvulnerable() const noexcept { return m_invulnerabilityTimer > 0.f; }

    // --- Fireball-Anfrage (wird von AttackState gesetzt, von Game/PlayingState
    //     abgeholt - Player kennt EntityManager/Fireball absichtlich NICHT,
    //     um die Abhaengigkeitsrichtung Game -> Player nicht umzukehren). ---
    void requestFireball() noexcept { if (m_powerState == PowerState::Fire) { m_fireballRequested = true; } }
    bool consumeFireballRequest() noexcept;
    sf::Vector2f getFireballSpawnPosition() const;

    // Setzt den Spieler auf Startzustand zurueck (neues Leben / neues Spiel).
    void respawn(sf::Vector2f position);

    AnimationController& getAnimationController() noexcept { return m_animationController; }

    static constexpr float WALK_SPEED = 150.f;
    static constexpr float JUMP_VELOCITY = -420.f;
    static constexpr float INVULNERABILITY_DURATION = 1.5f;
    static const sf::Vector2f SIZE; // Hitbox in Pixeln fuer PowerState::Small, definiert in Player.cpp

private:
    std::unique_ptr<IPlayerState> m_currentState;
    AnimationController m_animationController;
    sf::Sprite m_sprite;

    bool m_onGround{false};
    bool m_facingRight{true};

    PowerState m_powerState{PowerState::Small};
    float m_invulnerabilityTimer{0.f};
    bool m_fireballRequested{false};
};
