#include "../include/EnemyStates.h"
#include "../include/Enemy.h"

void PatrolState::enter(Enemy& enemy) {
    enemy.getAnimationController().play("walk");
}

void PatrolState::update(Enemy& enemy, float /*deltaTime*/) {
    if (enemy.hasHitWall()) {
        enemy.turnAround();
    }
    enemy.patrol();
}

void SquishedState::enter(Enemy& enemy) {
    enemy.getAnimationController().play("squished", true);
    enemy.setVelocity({0.f, enemy.getVelocity().y}); // horizontale Bewegung stoppt sofort
    m_elapsed = 0.f;
}

void SquishedState::update(Enemy& enemy, float deltaTime) {
    m_elapsed += deltaTime;
    if (m_elapsed >= DURATION) {
        enemy.kill();
    }
}
