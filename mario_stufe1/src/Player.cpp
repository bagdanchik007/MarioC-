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

    if (m_invulnerabilityTimer > 0.f) {
        m_invulnerabilityTimer -= deltaTime;
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

namespace {
sf::Vector2f sizeForPowerState(PowerState state) {
    switch (state) {
        case PowerState::Small: return {16.f, 16.f};
        case PowerState::Big:
        case PowerState::Fire: return {16.f, 28.f};
    }
    return {16.f, 16.f};
}
} // namespace

void Player::setPowerState(PowerState newState) {
    const sf::Vector2f oldSize = getSize();
    const sf::Vector2f newSize = sizeForPowerState(newState);

    if (newSize.y != oldSize.y) {
        // Beim Wachsen/Schrumpfen bleiben die Fuesse am Boden - dafuer wird
        // die Position um die Hoehendifferenz nach oben (Wachsen) bzw.
        // unten (Schrumpfen) verschoben, statt am oberen Rand zu wachsen.
        sf::Vector2f pos = getPosition();
        pos.y -= (newSize.y - oldSize.y);
        setPosition(pos);
    }

    m_powerState = newState;
    setSize(newSize);
}

void Player::applyPowerUp(ItemType type) {
    switch (type) {
        case ItemType::Mushroom:
            if (m_powerState == PowerState::Small) {
                setPowerState(PowerState::Big);
            }
            break;
        case ItemType::FireFlower:
            setPowerState(PowerState::Fire);
            break;
        case ItemType::Coin:
            break; // Coins geben keinen Power-up-Effekt, nur Punkte (siehe InteractionSystem)
    }
}

bool Player::takeDamage() {
    if (isInvulnerable()) {
        return false; // Treffer wird ignoriert, keine erneute Herabstufung
    }
    switch (m_powerState) {
        case PowerState::Fire:
            setPowerState(PowerState::Big);
            m_invulnerabilityTimer = INVULNERABILITY_DURATION;
            return false;
        case PowerState::Big:
            setPowerState(PowerState::Small);
            m_invulnerabilityTimer = INVULNERABILITY_DURATION;
            return false;
        case PowerState::Small:
            return true; // Bereits Small getroffen -> Tod. Game kuemmert sich um Leben/Respawn.
    }
    return true;
}

bool Player::consumeFireballRequest() noexcept {
    if (!m_fireballRequested) {
        return false;
    }
    m_fireballRequested = false;
    return true;
}

bool Player::consumeJumpEvent() noexcept {
    if (!m_jumpEventPending) {
        return false;
    }
    m_jumpEventPending = false;
    return true;
}

bool Player::consumeLandedEvent() noexcept {
    if (!m_landedEventPending) {
        return false;
    }
    m_landedEventPending = false;
    return true;
}

sf::Vector2f Player::getFireballSpawnPosition() const {
    const float offsetX = m_facingRight ? getSize().x : -Player::SIZE.x / 2.f;
    return sf::Vector2f(getPosition().x + offsetX, getPosition().y + getSize().y / 2.f);
}

void Player::respawn(sf::Vector2f position) {
    setPosition(position);
    setVelocity({0.f, 0.f});
    m_alive = true;
    m_onGround = false;
    m_invulnerabilityTimer = 0.f;
    m_fireballRequested = false;
    setPowerState(PowerState::Small);
    setState(std::make_unique<IdleState>());
}
