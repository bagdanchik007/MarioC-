#include "../include/Player.h"
#include "../include/PlayerStates.h"
#include <algorithm>
#include <cmath>

const sf::Vector2f Player::SIZE = {16.f, 16.f};

Player::Player(sf::Vector2f startPosition)
    : Entity(startPosition) {
    updateBounds(SIZE);
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
    m_currentState->update(*this, deltaTime);

    // Integration der Physik: Geschwindigkeit -> Position.
    // Die eigentliche Kollisionsaufloesung passiert erst in Etappe 2 (AABB).
    m_position += m_velocity * deltaTime;
    updateBounds(SIZE);

    m_animationController.update(deltaTime);
    m_sprite.setPosition(m_position);
    m_sprite.setScale(m_facingRight ? 1.f : -1.f, 1.f);
}

void Player::render(sf::RenderWindow& window) {
    m_animationController.applyTo(m_sprite);
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
    m_velocity.y = std::min(m_velocity.y + GRAVITY * deltaTime, MAX_FALL_SPEED);
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
