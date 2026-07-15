#include "../include/PlayerStates.h"
#include "../include/Player.h"

// ----------------------------------------------------------------------------
// IdleState
// ----------------------------------------------------------------------------
void IdleState::enter(Player& player) {
    player.getAnimationController().play("idle");
}

void IdleState::update(Player& player, float /*deltaTime*/) {
    if (!player.isOnGround()) {
        player.setState(std::make_unique<FallState>());
    }
}

void IdleState::handleAction(Player& player, PlayerAction action) {
    switch (action) {
        case PlayerAction::MoveLeft:
            player.moveHorizontal(-1.f);
            player.setState(std::make_unique<RunState>());
            break;
        case PlayerAction::MoveRight:
            player.moveHorizontal(1.f);
            player.setState(std::make_unique<RunState>());
            break;
        case PlayerAction::Jump:
            player.jump();
            player.setState(std::make_unique<JumpState>());
            break;
        case PlayerAction::Attack:
            player.setState(std::make_unique<AttackState>());
            break;
        case PlayerAction::StopHorizontal:
            player.stopHorizontal();
            break;
        default:
            break;
    }
}

// ----------------------------------------------------------------------------
// RunState
// ----------------------------------------------------------------------------
void RunState::enter(Player& player) {
    player.getAnimationController().play("run");
}

void RunState::update(Player& player, float /*deltaTime*/) {
    if (!player.isOnGround()) {
        player.setState(std::make_unique<FallState>());
    }
}

void RunState::handleAction(Player& player, PlayerAction action) {
    switch (action) {
        case PlayerAction::MoveLeft:
            player.moveHorizontal(-1.f);
            break;
        case PlayerAction::MoveRight:
            player.moveHorizontal(1.f);
            break;
        case PlayerAction::StopHorizontal:
            player.stopHorizontal();
            player.setState(std::make_unique<IdleState>());
            break;
        case PlayerAction::Jump:
            player.jump();
            player.setState(std::make_unique<JumpState>());
            break;
        case PlayerAction::Attack:
            player.setState(std::make_unique<AttackState>());
            break;
        default:
            break;
    }
}

// ----------------------------------------------------------------------------
// JumpState
// ----------------------------------------------------------------------------
void JumpState::enter(Player& player) {
    player.getAnimationController().play("jump");
}

void JumpState::update(Player& player, float /*deltaTime*/) {
    if (player.getVelocity().y >= 0.f) {
        player.setState(std::make_unique<FallState>());
    }
}

void JumpState::handleAction(Player& player, PlayerAction action) {
    switch (action) {
        case PlayerAction::MoveLeft:
            player.moveHorizontal(-1.f);
            break;
        case PlayerAction::MoveRight:
            player.moveHorizontal(1.f);
            break;
        case PlayerAction::StopHorizontal:
            player.stopHorizontal();
            break;
        case PlayerAction::JumpRelease:
            player.cutJumpShort();
            break;
        default:
            break; // Kein Doppelsprung, kein Angriff in der Luft - bewusste Design-Entscheidung.
    }
}

// ----------------------------------------------------------------------------
// FallState
// ----------------------------------------------------------------------------
void FallState::enter(Player& player) {
    player.getAnimationController().play("fall");
}

void FallState::update(Player& player, float /*deltaTime*/) {
    if (player.isOnGround()) {
        if (player.getVelocity().x != 0.f) {
            player.setState(std::make_unique<RunState>());
        } else {
            player.setState(std::make_unique<IdleState>());
        }
    }
}

void FallState::handleAction(Player& player, PlayerAction action) {
    switch (action) {
        case PlayerAction::MoveLeft:
            player.moveHorizontal(-1.f);
            break;
        case PlayerAction::MoveRight:
            player.moveHorizontal(1.f);
            break;
        case PlayerAction::StopHorizontal:
            player.stopHorizontal();
            break;
        default:
            break;
    }
}

// ----------------------------------------------------------------------------
// AttackState
// ----------------------------------------------------------------------------
void AttackState::enter(Player& player) {
    player.getAnimationController().play("attack", /*forceRestart=*/true);
    player.stopHorizontal(); // Mario bleibt beim Angriff stehen (Design-Entscheidung).
    m_elapsed = 0.f;
    player.requestFireball(); // No-op, falls PowerState != Fire (siehe Player::requestFireball).
}

void AttackState::update(Player& player, float deltaTime) {
    m_elapsed += deltaTime;
    if (m_elapsed >= DURATION) {
        if (player.isOnGround()) {
            player.setState(std::make_unique<IdleState>());
        } else {
            player.setState(std::make_unique<FallState>());
        }
    }
}

void AttackState::handleAction(Player& /*player*/, PlayerAction /*action*/) {
    // Angriff ist "committed": Eingaben werden ignoriert, bis die Animation endet.
    // Alternative waere ein Cancel-Fenster - bewusst weggelassen fuer Etappe 1.
}
