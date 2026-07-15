#pragma once
#include "Tilemap.h"

class Entity;

// Ergebnis einer Kollisionsaufloesung. hitWall wird von der Enemy-Patrol-KI
// genutzt (Umdrehen an Waenden), onGround von Player UND Enemy (Gravitation/
// Sprung-Logik).
struct CollisionResult {
    bool onGround = false;
    bool hitWall = false;
};

// Loest AABB-Kollisionen zwischen einer Entity und der Tilemap auf.
// Separate-Axis-Sweep: X und Y werden UNABHAENGIG voneinander bewegt und
// aufgeloest (erst X, dann Y) - verhindert Tunneling bei diagonaler Bewegung.
// Statische Klasse ohne State: arbeitet nur auf der uebergebenen Entity und
// Tilemap, daher fuer beliebig viele Entities wiederverwendbar (Player, Enemy).
class CollisionSystem {
public:
    static CollisionResult resolve(Entity& entity, const Tilemap& tilemap, float deltaTime);

private:
    static bool resolveAxisX(Entity& entity, const Tilemap& tilemap, float deltaTime); // -> hitWall
    static bool resolveAxisY(Entity& entity, const Tilemap& tilemap, float deltaTime); // -> onGround
};
