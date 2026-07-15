#pragma once
#include <SFML/Graphics.hpp>
#include <memory>
#include "Player.h"
#include "InputHandler.h"
#include "Level.h"
#include "Camera.h"
#include "Enemy.h"
#include "Item.h"
#include "Fireball.h"
#include "EntityManager.h"
#include "IGameState.h"

// Game: Wurzel-Klasse UND Kontext der Game State Machine. Haelt alle
// Top-Level-Systeme (Fenster, Welt, Meta-Daten wie Score/Leben/Timer) und
// delegiert Verhalten komplett an den aktuellen IGameState - Game selbst
// entscheidet nicht mehr, WAS in einem Frame passiert, nur DASS der aktuelle
// State befragt wird. Die States greifen ueber die oeffentlichen Getter/
// Setter unten auf die Welt zu.
class Game {
public:
    Game();

    void run();

    // --- Schnittstelle fuer IGameState-Implementierungen ---
    void setState(std::unique_ptr<IGameState> newState);

    sf::RenderWindow& getWindow() noexcept { return m_window; }
    Player& getPlayer() noexcept { return m_player; }
    Level& getLevel() noexcept { return m_level; }
    Camera& getCamera() noexcept { return m_camera; }
    InputHandler& getInput() noexcept { return m_input; }
    EntityManager<Enemy>& getEnemies() noexcept { return m_enemies; }
    EntityManager<Item>& getItems() noexcept { return m_items; }
    EntityManager<Fireball>& getFireballs() noexcept { return m_fireballs; }

    int getScore() const noexcept { return m_score; }
    void addScore(int amount) noexcept { m_score += amount; }
    int getCoinCount() const noexcept { return m_coinCount; }
    void addCoins(int amount) noexcept { m_coinCount += amount; }
    int getLives() const noexcept { return m_lives; }
    void loseLife() noexcept { --m_lives; }
    float getTimeRemaining() const noexcept { return m_levelTimeRemaining; }
    void tickTimer(float deltaTime) noexcept { m_levelTimeRemaining -= deltaTime; }
    void resetTimer() noexcept { m_levelTimeRemaining = LEVEL_TIME_LIMIT; }

    // Setzt Level/Player/Enemies/Items/Score/Leben auf den Anfangszustand
    // zurueck (genutzt von GameOverState/LevelCompleteState bei Neustart).
    void resetGame();

    static constexpr int STARTING_LIVES = 3;
    static constexpr float LEVEL_TIME_LIMIT = 200.f;
    static constexpr float TILE_SIZE = 32.f;

private:
    void processEvents();
    void update(float deltaTime);
    void render();
    void spawnDemoEntities();

    sf::RenderWindow m_window;
    sf::Clock m_clock;

    // Deklarationsreihenfolge = Konstruktionsreihenfolge: m_level muss vor
    // m_player existieren (Player braucht m_level.getPlayerSpawn()).
    Level m_level;
    Player m_player;
    Camera m_camera;
    InputHandler m_input;

    EntityManager<Enemy> m_enemies;
    EntityManager<Item> m_items;
    EntityManager<Fireball> m_fireballs;

    int m_score{0};
    int m_coinCount{0};
    int m_lives{STARTING_LIVES};
    float m_levelTimeRemaining{LEVEL_TIME_LIMIT};

    std::unique_ptr<IGameState> m_currentState;

    static constexpr unsigned int WINDOW_WIDTH = 800;
    static constexpr unsigned int WINDOW_HEIGHT = 600;
};
