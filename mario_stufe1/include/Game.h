#pragma once
#include <SFML/Graphics.hpp>
#include "Player.h"
#include "InputHandler.h"

// Game: Wurzel-Klasse der Anwendung. Besitzt das Fenster, die Game-Loop und
// alle Top-Level-Systeme (aktuell Player + InputHandler, spaeter Level,
// Camera, StateManager ...). Andere Klassen kennen Game NICHT - nur Game
// kennt sie. So bleibt die Abhaengigkeitsrichtung immer "von oben nach unten".
class Game {
public:
    Game();

    void run(); // Startet die Game-Loop, blockiert bis das Fenster geschlossen wird.

private:
    void processEvents();
    void update(float deltaTime);
    void render();

    // TEMPORAERER Boden-Stub fuer Etappe 1.
    // Wird in Etappe 2 vollstaendig durch AABB-Kollision gegen die Tilemap
    // ersetzt - hier nur, damit Jump/Fall/Idle/Run schon jetzt sichtbar
    // und testbar sind, ohne auf das Kollisionssystem zu warten.
    void applyTemporaryGroundStub();

    sf::RenderWindow m_window;
    sf::Clock m_clock;

    Player m_player;
    InputHandler m_input;

    static constexpr unsigned int WINDOW_WIDTH = 800;
    static constexpr unsigned int WINDOW_HEIGHT = 600;
    static constexpr float GROUND_Y = 500.f; // Boden-Hoehe in Pixeln (temporaer)
};
