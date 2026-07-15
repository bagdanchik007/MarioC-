#include "../include/Game.h"
#include "../include/Commands.h"
#include "../include/GameStates.h"
#include <memory>

namespace {

// Baut ein kleines Demo-Level: Boden mit Luecke (Fallen testen), eine
// schwebende Plattform (Sprung + Landung in der Luft testen) und ein
// Spawn-Marker. In Etappe 6 wird das durch echtes Laden aus JSON ersetzt -
// Level/Tilemap kennen den Unterschied nicht, sie bekommen ohnehin nur
// einen vector<string>.
std::vector<std::string> buildDemoLevelLayout() {
    constexpr int width = 50;
    constexpr int height = 20;
    std::vector<std::string> layout(height, std::string(width, '.'));

    for (int col = 0; col < width; ++col) {
        const bool gap = (col >= 20 && col <= 23);
        if (!gap) {
            layout[height - 1][col] = '#';
            layout[height - 2][col] = '#';
        }
    }

    for (int col = 28; col <= 33; ++col) {
        layout[height - 7][col] = '#';
    }

    layout[1][2] = 'P';

    return layout;
}

} // namespace

Game::Game()
    : m_window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Mario Clone - Etappe 4")
    , m_level(buildDemoLevelLayout(), TILE_SIZE)
    , m_player(m_level.getPlayerSpawn())
    , m_camera(sf::Vector2f(static_cast<float>(WINDOW_WIDTH), static_cast<float>(WINDOW_HEIGHT)),
               m_level.getWorldBounds()) {
    m_window.setFramerateLimit(60);

    m_input.bindContinuous(sf::Keyboard::Left,  std::make_unique<MoveLeftCommand>());
    m_input.bindContinuous(sf::Keyboard::Right, std::make_unique<MoveRightCommand>());
    m_input.bindOnPressed(sf::Keyboard::Up,     std::make_unique<JumpCommand>());
    m_input.bindOnReleased(sf::Keyboard::Up,    std::make_unique<JumpReleaseCommand>());
    m_input.bindOnPressed(sf::Keyboard::X,      std::make_unique<AttackCommand>());

    spawnDemoEntities();

    m_currentState = std::make_unique<MenuState>();
    m_currentState->enter(*this);
}

void Game::spawnDemoEntities() {
    const float floorRow = static_cast<float>(m_level.getTilemap().getHeight() - 3);

    m_enemies.add(std::make_unique<Enemy>(EnemyType::Goomba,
        sf::Vector2f(15.f * TILE_SIZE, floorRow * TILE_SIZE)));
    m_enemies.add(std::make_unique<Enemy>(EnemyType::Goomba,
        sf::Vector2f(35.f * TILE_SIZE, floorRow * TILE_SIZE)));

    // Items ueber der schwebenden Plattform (col 28-33) und auf dem Boden.
    m_items.add(std::make_unique<Item>(ItemType::Coin,
        sf::Vector2f(30.f * TILE_SIZE, (floorRow - 5.f) * TILE_SIZE)));
    m_items.add(std::make_unique<Item>(ItemType::Coin,
        sf::Vector2f(10.f * TILE_SIZE, (floorRow - 1.f) * TILE_SIZE)));
    m_items.add(std::make_unique<Item>(ItemType::Mushroom,
        sf::Vector2f(6.f * TILE_SIZE, (floorRow - 1.f) * TILE_SIZE)));
    m_items.add(std::make_unique<Item>(ItemType::FireFlower,
        sf::Vector2f(31.f * TILE_SIZE, (floorRow - 5.f) * TILE_SIZE)));
}

void Game::setState(std::unique_ptr<IGameState> newState) {
    if (m_currentState) {
        m_currentState->exit(*this);
    }
    m_currentState = std::move(newState);
    m_currentState->enter(*this);
}

void Game::run() {
    while (m_window.isOpen()) {
        const float deltaTime = m_clock.restart().asSeconds();
        processEvents();
        update(deltaTime);
        render();
    }
}

void Game::processEvents() {
    sf::Event event;
    while (m_window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            m_window.close();
        }
        m_currentState->handleEvent(*this, event);
    }
}

void Game::update(float deltaTime) {
    m_currentState->update(*this, deltaTime);
}

void Game::render() {
    m_currentState->render(*this, m_window);
}

void Game::resetGame() {
    m_player.respawn(m_level.getPlayerSpawn());
    m_enemies.clear();
    m_items.clear();
    m_fireballs.clear();
    spawnDemoEntities();

    m_score = 0;
    m_coinCount = 0;
    m_lives = STARTING_LIVES;
    resetTimer();
}
