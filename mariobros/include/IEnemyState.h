#pragma once

class Enemy;

// State-Interface fuer Gegner-KI. Bewusst schlanker als IPlayerState (kein
// handleAction) - Enemies verarbeiten keinen Spieler-Input. Uebergaenge
// passieren entweder intern (Patrol reagiert selbst auf Wandkontakt) oder
// werden von aussen erzwungen (InteractionSystem ruft Enemy::squish() bei
// einem Stomp auf, was intern setState(SquishedState) ausloest).
class IEnemyState {
public:
    virtual ~IEnemyState() = default;
    virtual void enter(Enemy&) {}
    virtual void exit(Enemy&) {}
    virtual void update(Enemy& enemy, float deltaTime) = 0;
    virtual const char* name() const = 0;
};
