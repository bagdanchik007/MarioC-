#include "../include/Game.h"
#include "../include/Commands.h"
#include <memory>

Game::Game()
    : m_window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Mario Clone - Etappe 1")
    , m_player(sf::Vector2f(100.f, GROUND_Y - Player::SIZE.y)) {
    m_window.setFramerateLimit(60);

    // Action Mapping: Tasten -> Commands. Bewegung ueber Continuous-Polling,
    // Jump/Attack ueber Events (siehe InputHandler-Kommentar fuer den Grund).
    m_input.bindContinuous(sf::Keyboard::Left,  std::make_unique<MoveLeftCommand>());
    m_input.bindContinuous(sf::Keyboard::Right, std::make_unique<MoveRightCommand>());
    m_input.bindOnPressed(sf::Keyboard::Up,     std::make_unique<JumpCommand>());
    m_input.bindOnReleased(sf::Keyboard::Up,    std::make_unique<JumpReleaseCommand>());
    m_input.bindOnPressed(sf::Keyboard::X,      std::make_unique<AttackCommand>());

    // Spieler startet stehend auf dem Boden-Stub, nicht fallend.
    m_player.setOnGround(true);
}

void Game::run() {
    while (m_window.isOpen()) {
        // Delta Time: Zeit seit dem letzten Frame in Sekunden.
        // Macht Bewegung/Physik framerate-unabhaengig (60 FPS vs 144 FPS -> gleiches Spielgefuehl).
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
        // Diskrete Aktionen (Jump/Attack) werden pro Event ausgewertet,
        // nicht pro Frame - siehe InputHandler::handleEvent.
        m_input.handleEvent(event, m_player);
    }
}

void Game::update(float deltaTime) {
    // Kontinuierliche Aktionen (Laufen) werden jeden Frame per Polling geprueft.
    m_input.pollContinuousInput(m_player);

    m_player.update(deltaTime);

    applyTemporaryGroundStub();
}

void Game::render() {
    m_window.clear(sf::Color(92, 148, 252)); // Mario-typisches Himmelblau
    m_player.render(m_window);
    m_window.display();
}

void Game::applyTemporaryGroundStub() {
    const float playerBottom = m_player.getPosition().y + Player::SIZE.y;
    if (playerBottom >= GROUND_Y) {
        sf::Vector2f pos = m_player.getPosition();
        pos.y = GROUND_Y - Player::SIZE.y;
        m_player.setPosition(pos);
        m_player.setVelocity({m_player.getVelocity().x, 0.f});
        m_player.setOnGround(true);
    } else {
        m_player.setOnGround(false);
    }
}
