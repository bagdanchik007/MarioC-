#pragma once
#include <SFML/Graphics.hpp>
#include "Player.h"
#include "InputHandler.h"
#include "Level.h"
#include "Camera.h"

// Game: Wurzel-Klasse der Anwendung. Besitzt Fenster, Game-Loop und alle
// Top-Level-Systeme. Abhaengigkeitsrichtung bleibt strikt einseitig:
// Game -> Level/Player/Camera/InputHandler, nie umgekehrt.
class Game {
public:
    Game();

    void run();

private:
    void processEvents();
    void update(float deltaTime);
    void render();

    sf::RenderWindow m_window;
    sf::Clock m_clock;

    // Deklarationsreihenfolge = Konstruktionsreihenfolge: m_level muss vor
    // m_player existieren (Player braucht m_level.getPlayerSpawn()), und
    // m_camera braucht m_level.getWorldBounds().
    Level m_level;
    Player m_player;
    Camera m_camera;
    InputHandler m_input;

    static constexpr unsigned int WINDOW_WIDTH = 800;
    static constexpr unsigned int WINDOW_HEIGHT = 600;
    static constexpr float TILE_SIZE = 32.f;
};
