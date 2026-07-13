#include "../include/Commands.h"
#include "../include/Player.h"

void MoveLeftCommand::execute(Player& player) const {
    player.handleAction(PlayerAction::MoveLeft);
}

void MoveRightCommand::execute(Player& player) const {
    player.handleAction(PlayerAction::MoveRight);
}

void StopHorizontalCommand::execute(Player& player) const {
    player.handleAction(PlayerAction::StopHorizontal);
}

void JumpCommand::execute(Player& player) const {
    player.handleAction(PlayerAction::Jump);
}

void JumpReleaseCommand::execute(Player& player) const {
    player.handleAction(PlayerAction::JumpRelease);
}

void AttackCommand::execute(Player& player) const {
    player.handleAction(PlayerAction::Attack);
}
