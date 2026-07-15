#pragma once
#include <SFML/Graphics.hpp>

class Game;

// State-Interface fuer die Game State Machine (Menu, Playing, Paused,
// GameOver, LevelComplete). Analog zu IPlayerState/IEnemyState, aber auf
// Anwendungsebene: hier wird entschieden OB und WIE die Spielwelt ueberhaupt
// aktualisiert/gerendert wird - nicht wie sich einzelne Entities verhalten.
class IGameState {
public:
    virtual ~IGameState() = default;
    virtual void enter(Game&) {}
    virtual void exit(Game&) {}
    virtual void handleEvent(Game& game, const sf::Event& event) = 0;
    virtual void update(Game& game, float deltaTime) = 0;
    virtual void render(Game& game, sf::RenderWindow& window) = 0;
    virtual const char* name() const = 0;
};
