#pragma once

// Basisinterface fuer optionale Komponenten.
// Player nutzt in Etappe 1 noch Vererbung + Objekt-Komposition (State, Animation),
// echtes ECS folgt optional in Etappe 6. Dieses Interface haelt die Tuer offen,
// damit Enemies/Items in Etappe 3 Komponenten (Health, Hitbox, AI ...) anhaengen koennen.
class IComponent {
public:
    virtual ~IComponent() = default;
    virtual void update(float deltaTime) = 0;
};
