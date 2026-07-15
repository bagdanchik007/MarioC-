#include "../include/Player.h"
#include "../include/PlayerStates.h"
#include <algorithm>
#include <cmath>

const sf::Vector2f Player::SIZE = {16.f, 16.f};

Player::Player(sf::Vector2f startPosition)
    : Entity(startPosition) {
    setSize(SIZE);
    m_sprite.setPosition(m_position);
    // Startzustand: Idle. enter() wird hier explizit aufgerufen, da der
    // Konstruktor nicht ueber setState laeuft (kein "voriger" State zum exit()-en).
    m_currentState = std::make_unique<IdleState>();
    m_currentState->enter(*this);
}

void Player::update(float deltaTime) {
    if (!m_alive) {
        return;
    }

    // Gravitation wirkt permanent, unabhaengig vom State. Das CollisionSystem
    // (Etappe 2) hebt sie beim Landen wieder auf und setzt onGround - dadurch
    // muessen Idle/Run selbst keine Gravitation kennen, nur Jump/Fall/Attack
    // reagieren auf ihre Auswirkung (steigende Fallgeschwindigkeit).
    applyGravity(deltaTime);

    m_currentState->update(*this, deltaTime);

    // WICHTIG: Position wird HIER NICHT mehr integriert (kein m_position +=
    // velocity*deltaTime). Das uebernimmt jetzt CollisionSystem::resolve(),
    // das von Game::update() nach diesem Aufruf explizit ausgefuehrt wird -
    // nur dort kann waehrend der Bewegung gegen die Tilemap geprueft werden.
    m_animationController.update(deltaTime);
}

void Player::render(sf::RenderWindow& window) {
    m_animationController.applyTo(m_sprite);
    m_sprite.setPosition(m_position);
    m_sprite.setScale(m_facingRight ? 1.f : -1.f, 1.f);
    window.draw(m_sprite);
}

void Player::handleAction(PlayerAction action) {
    m_currentState->handleAction(*this, action);
}

void Player::setState(std::unique_ptr<IPlayerState> newState) {
    m_currentState->exit(*this);
    m_currentState = std::move(newState);
    m_currentState->enter(*this);
}

const char* Player::getStateName() const {
    return m_currentState->name();
}

void Player::applyGravity(float deltaTime) {
    m_velocity.y = std::min(m_velocity.y + Physics::GRAVITY * deltaTime, Physics::MAX_FALL_SPEED);
}

void Player::jump() {
    m_velocity.y = JUMP_VELOCITY;
    m_onGround = false;
}

void Player::cutJumpShort() {
    // Variable Sprunghoehe (wie im Original-Mario): wird die Taste waehrend
    // des Aufstiegs losgelassen, wird die Aufwaertsgeschwindigkeit gekappt.
    if (m_velocity.y < 0.f) {
        m_velocity.y *= 0.45f;
    }
}

void Player::moveHorizontal(float direction) {
    m_velocity.x = direction * WALK_SPEED;
    if (direction != 0.f) {
        m_facingRight = direction > 0.f;
    }
}

void Player::stopHorizontal() {
    m_velocity.x = 0.f;
}
