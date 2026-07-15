#pragma once
#include <memory>
#include <SFML/Graphics.hpp>
#include "Entity.h"
#include "IPlayerState.h"
#include "PlayerAction.h"
#include "AnimationController.h"
#include "PhysicsConstants.h"

// Player: konkrete Entity mit Physik, State Machine und Animation.
// Bewusst KEINE ECS-Komponente in Etappe 1 - Vererbung von Entity reicht fuer
// ein einzelnes, klar definiertes Objekt. ECS lohnt sich erst, wenn viele
// unterschiedliche Objektarten dieselben Faehigkeiten teilen (siehe Etappe 3/6).
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

    AnimationController& getAnimationController() noexcept { return m_animationController; }

    static constexpr float WALK_SPEED = 150.f;
    static constexpr float JUMP_VELOCITY = -420.f;
    static const sf::Vector2f SIZE; // Hitbox in Pixeln (Mario-typisch klein), definiert in Player.cpp

private:
    std::unique_ptr<IPlayerState> m_currentState;
    AnimationController m_animationController;
    sf::Sprite m_sprite;

    bool m_onGround{false};
    bool m_facingRight{true};
};
