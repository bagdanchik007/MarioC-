#include "../include/Game.h"
#include "../include/Commands.h"
#include "../include/CollisionSystem.h"
#include <memory>

namespace {

// Baut ein kleines Demo-Level fuer Etappe 2: Boden mit einer Luecke (zum
// Testen des Fallens), eine schwebende Plattform (zum Testen von Sprung +
// Landung in der Luft) und ein Spawn-Marker. In Etappe 6 wird das durch
// echtes Laden aus JSON-Dateien ersetzt - Level/Tilemap kennen den Unterschied
// nicht, sie bekommen ohnehin nur einen vector<string>.
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
    : m_window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Mario Clone - Etappe 2")
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

    // Reihenfolge ist entscheidend:
    // 1) Player::update() wendet Gravitation an und laesst den aktuellen
    //    State (mit dem onGround-Wert vom LETZTEN Frame) reagieren.
    // 2) CollisionSystem::resolve() bewegt die Entity tatsaechlich und
    //    loest Kollisionen gegen die Tilemap auf - erst hier "passiert" die
    //    Bewegung wirklich, mit korrekter Kollisionsantwort.
    // 3) Das Ergebnis (onGround) wird fuer den NAECHSTEN Frame gespeichert.
    m_player.update(deltaTime);
    const bool onGround = CollisionSystem::resolve(m_player, m_level.getTilemap(), deltaTime);
    m_player.setOnGround(onGround);

    m_camera.follow(m_player.getPosition(), deltaTime);
}

void Game::render() {
    m_window.clear(sf::Color(92, 148, 252)); // Mario-typisches Himmelblau
    m_window.setView(m_camera.getView());

    m_level.render(m_window);
    m_player.render(m_window);

    m_window.display();
}
