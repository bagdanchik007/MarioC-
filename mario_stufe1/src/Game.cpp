#include "../include/Game.h"
#include "../include/Commands.h"
#include "../include/CollisionSystem.h"
#include "../include/InteractionSystem.h"
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
    : m_window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Mario Clone - Etappe 3")
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
}

void Game::spawnDemoEntities() {
    const float floorRow = static_cast<float>(m_level.getTilemap().getHeight() - 3);

    m_enemies.add(std::make_unique<Enemy>(EnemyType::Goomba,
        sf::Vector2f(15.f * TILE_SIZE, floorRow * TILE_SIZE)));
    m_enemies.add(std::make_unique<Enemy>(EnemyType::Goomba,
        sf::Vector2f(35.f * TILE_SIZE, floorRow * TILE_SIZE)));

    // Coin ueber der schwebenden Plattform (col 28-33, siehe buildDemoLevelLayout)
    m_coins.add(std::make_unique<Coin>(sf::Vector2f(30.f * TILE_SIZE, (floorRow - 5.f) * TILE_SIZE)));
    m_coins.add(std::make_unique<Coin>(sf::Vector2f(10.f * TILE_SIZE, (floorRow - 1.f) * TILE_SIZE)));
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
        m_input.handleEvent(event, m_player);
    }
}

void Game::update(float deltaTime) {
    m_input.pollContinuousInput(m_player);

    // 1) Player: Gravitation + State-Update (reagiert auf onGround vom LETZTEN Frame).
    m_player.update(deltaTime);
    const CollisionResult playerCollision = CollisionSystem::resolve(m_player, m_level.getTilemap(), deltaTime);
    m_player.setOnGround(playerCollision.onGround);

    // 2) Enemies: gleiche Physik-Pipeline wie der Player, wiederverwendet
    //    ueber CollisionSystem (arbeitet auf Entity&, kennt weder Player noch Enemy).
    for (auto& enemyPtr : m_enemies.getAll()) {
        Enemy& enemy = *enemyPtr;
        enemy.update(deltaTime);
        const CollisionResult enemyCollision = CollisionSystem::resolve(enemy, m_level.getTilemap(), deltaTime);
        enemy.setOnGround(enemyCollision.onGround);
        enemy.setHitWall(enemyCollision.hitWall);
    }

    // 3) Coins: keine Physik/Kollision noetig, nur eigenes Update (Schweb-Animation).
    m_coins.update(deltaTime);

    // 4) Interaktionen: NACH der Bewegung, damit Positionen fuer diesen Frame final sind.
    InteractionSystem::resolvePlayerEnemies(m_player, m_enemies.getAll());
    m_coinCount += InteractionSystem::resolvePlayerCoins(m_player, m_coins.getAll());

    // 5) Tote Entities entfernen (erst NACH den Interaktionen, die isAlive() lesen).
    m_enemies.removeDead();
    m_coins.removeDead();

    m_camera.follow(m_player.getPosition(), deltaTime);

    m_window.setTitle("Mario Clone - Etappe 3 | Coins: " + std::to_string(m_coinCount));
}

void Game::render() {
    m_window.clear(sf::Color(92, 148, 252)); // Mario-typisches Himmelblau
    m_window.setView(m_camera.getView());

    m_level.render(m_window);
    m_coins.render(m_window);
    m_enemies.render(m_window);
    m_player.render(m_window);

    m_window.display();
}
