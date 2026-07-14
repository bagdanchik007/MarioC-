#pragma once
#include "Tilemap.h"

class Entity;

// Loest AABB-Kollisionen zwischen einer Entity und der Tilemap auf.
// Separate-Axis-Sweep: X und Y werden UNABHAENGIG voneinander bewegt und
// aufgeloest (erst X, dann Y). Das verhindert das klassische Tunneling-Problem
// bei diagonaler Bewegung (z.B. beim Sprung gegen eine Wand) und macht die
// Aufloesung fuer jede Achse einzeln trivial nachvollziehbar.
//
// Statische Klasse ohne State: das System selbst besitzt nichts, es operiert
// nur auf der uebergebenen Entity und Tilemap. Damit ist es fuer beliebig
// viele Entities (Player, spaeter Enemies) wiederverwendbar.
class CollisionSystem {
public:
    // Bewegt die Entity um velocity*deltaTime (in zwei Schritten, X dann Y)
    // und loest dabei Kollisionen mit soliden Tiles auf.
    // Rueckgabewert: steht die Entity danach auf einem soliden Tile (Boden)?
    static bool resolve(Entity& entity, const Tilemap& tilemap, float deltaTime);

private:
    static void resolveAxisX(Entity& entity, const Tilemap& tilemap, float deltaTime);
    static bool resolveAxisY(Entity& entity, const Tilemap& tilemap, float deltaTime);
};
