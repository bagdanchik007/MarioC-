#include "../include/Enemy.h"
#include "../include/EnemyStates.h"
#include "../include/PhysicsConstants.h"
#include <algorithm>

namespace {

// Tuning-Tabellen pro Gegnertyp. Zentrale Stelle fuer alle typ-spezifischen
// Werte - ein neuer EnemyType-Eintrag erfordert nur eine neue case-Zeile hier.
float walkSpeedFor(EnemyType type) {
    switch (type) {
        case EnemyType::Goomba: return 40.f;
    }
    return 40.f;
}

int scoreValueFor(EnemyType type) {
    switch (type) {
        case EnemyType::Goomba: return 100;
    }
    return 100;
}

sf::Vector2f sizeFor(EnemyType type) {
    switch (type) {
        case EnemyType::Goomba: return {16.f, 16.f};
    }
    return {16.f, 16.f};
}

} // namespace

Enemy::Enemy(EnemyType type, sf::Vector2f startPosition)
    : Entity(startPosition)
    , m_type(type)
    , m_walkSpeed(walkSpeedFor(type)) {
    setSize(sizeFor(type));
    m_sprite.setPosition(m_position);

    m_facingRight = false; // Konvention: Gegner starten nach links laufend
    m_velocity.x = -m_walkSpeed;

    m_currentState = std::make_unique<PatrolState>();
    m_currentState->enter(*this);
}

void Enemy::update(float deltaTime) {
    if (!m_alive) {
        return;
    }
    applyGravity(deltaTime);
    m_currentState->update(*this, deltaTime);
    m_animationController.update(deltaTime);
}

void Enemy::render(sf::RenderWindow& window) {
    m_animationController.applyTo(m_sprite);
    m_sprite.setPosition(m_position);
    m_sprite.setScale(m_facingRight ? 1.f : -1.f, 1.f);
    window.draw(m_sprite);
}

void Enemy::setState(std::unique_ptr<IEnemyState> newState) {
    m_currentState->exit(*this);
    m_currentState = std::move(newState);
    m_currentState->enter(*this);
}

const char* Enemy::getStateName() const {
    return m_currentState->name();
}

void Enemy::applyGravity(float deltaTime) {
    m_velocity.y = std::min(m_velocity.y + Physics::GRAVITY * deltaTime, Physics::MAX_FALL_SPEED);
}

void Enemy::patrol() {
    m_velocity.x = m_facingRight ? m_walkSpeed : -m_walkSpeed;
}

void Enemy::turnAround() {
    m_facingRight = !m_facingRight;
    m_hitWall = false; // Flag konsumiert - sonst wuerde er jeden Frame erneut umdrehen
}

int Enemy::getScoreValue() const noexcept {
    return scoreValueFor(m_type);
}

void Enemy::squish() {
    if (m_squished) {
        return; // bereits am Sterben - kein erneuter State-Wechsel
    }
    m_squished = true;
    setState(std::make_unique<SquishedState>());
}
