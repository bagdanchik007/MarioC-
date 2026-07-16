#include "../include/Game.h"
#include "../include/Commands.h"
#include "../include/GameStates.h"
#include "../include/LevelLoader.h"
#include "../include/SaveManager.h"
#include <memory>

namespace {

// Eingebautes Fallback-Layout, falls assets/levels/level1.json fehlt oder
// ungueltig ist - das Spiel bleibt so auch OHNE Level-Assets vollstaendig
// lauffaehig (gleiche "graceful degradation" wie bei fehlenden Grafik-/
// Audio-Assets). Im Normalfall (Datei vorhanden) wird das hier NICHT genutzt.
std::vector<std::string> buildFallbackTiles() {
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

LevelData buildFallbackLevelData() {
    LevelData data;
    data.tileSize = Game::TILE_SIZE;
    data.tiles = buildFallbackTiles();

    constexpr float floorRow = 17.f; // height(20) - 3

    data.enemies.push_back({EnemyType::Goomba, sf::Vector2f(15.f * Game::TILE_SIZE, floorRow * Game::TILE_SIZE)});
    data.enemies.push_back({EnemyType::Goomba, sf::Vector2f(35.f * Game::TILE_SIZE, floorRow * Game::TILE_SIZE)});

    data.items.push_back({ItemType::Coin, sf::Vector2f(30.f * Game::TILE_SIZE, (floorRow - 5.f) * Game::TILE_SIZE)});
    data.items.push_back({ItemType::Coin, sf::Vector2f(10.f * Game::TILE_SIZE, (floorRow - 1.f) * Game::TILE_SIZE)});
    data.items.push_back({ItemType::Mushroom, sf::Vector2f(6.f * Game::TILE_SIZE, (floorRow - 1.f) * Game::TILE_SIZE)});
    data.items.push_back({ItemType::FireFlower, sf::Vector2f(31.f * Game::TILE_SIZE, (floorRow - 5.f) * Game::TILE_SIZE)});

    return data;
}

// Zentrale Stelle fuer "wie kommt Game an seine Leveldaten" - Etappe 6s
// Kernidee: ein neues Level = eine neue JSON-Datei, kein neuer Code hier.
LevelData loadLevelOrFallback() {
    LevelData data;
    if (LevelLoader::loadFromFile("assets/levels/level1.json", data)) {
        return data;
    }
    return buildFallbackLevelData();
}

} // namespace

Game::Game()
    : m_window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Mario Clone - Etappe 6")
    , m_levelData(loadLevelOrFallback())
    , m_level(m_levelData.tiles, m_levelData.tileSize)
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
    subscribeToEvents();

    // Assets fehlen bisher komplett - registerSound()/playMusic() geben dann
    // sauber false zurueck, ohne das Spiel zu crashen (siehe SoundManager).
    m_sounds.registerSound("jump", "assets/sfx/jump.wav");
    m_sounds.registerSound("stomp", "assets/sfx/stomp.wav");
    m_sounds.registerSound("coin", "assets/sfx/coin.wav");
    m_sounds.registerSound("powerup", "assets/sfx/powerup.wav");
    m_sounds.registerSound("fireball", "assets/sfx/fireball.wav");
    m_sounds.playMusic("assets/music/theme.ogg");

    m_currentState = std::make_unique<MenuState>();
    m_currentState->enter(*this);
}

void Game::subscribeToEvents() {
    m_events.subscribe(EventType::EnemyStomped, [this](const GameEvent& e) {
        m_sounds.play("stomp");
        m_particles.emitBurst(e.position, sf::Color(139, 69, 19), 10);
    });
    m_events.subscribe(EventType::CoinCollected, [this](const GameEvent& e) {
        m_sounds.play("coin");
        m_particles.emitBurst(e.position, sf::Color(255, 215, 0), 8);
    });
    m_events.subscribe(EventType::PlayerPoweredUp, [this](const GameEvent& e) {
        m_sounds.play("powerup");
        m_particles.emitBurst(e.position, sf::Color(255, 255, 255), 14);
    });
    m_events.subscribe(EventType::PlayerJumped, [this](const GameEvent& /*e*/) {
        m_sounds.play("jump");
    });
    m_events.subscribe(EventType::PlayerLanded, [this](const GameEvent& e) {
        m_particles.emitBurst(e.position, sf::Color(180, 180, 180), 5);
    });
    m_events.subscribe(EventType::PlayerDamaged, [this](const GameEvent& e) {
        m_particles.emitBurst(e.position, sf::Color(220, 40, 40), 10);
    });
    m_events.subscribe(EventType::FireballFired, [this](const GameEvent& /*e*/) {
        m_sounds.play("fireball");
    });
}

void Game::spawnDemoEntities() {
    // "Demo" nur noch dem Namen nach - die eigentlichen Spawn-Daten kommen
    // jetzt aus m_levelData (JSON oder Fallback), nicht mehr hartcodiert.
    for (const auto& spawn : m_levelData.enemies) {
        m_enemies.add(std::make_unique<Enemy>(spawn.type, spawn.position));
    }
    for (const auto& spawn : m_levelData.items) {
        m_items.add(std::make_unique<Item>(spawn.type, spawn.position));
    }
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

bool Game::saveGame(const std::string& filepath) const {
    SaveData data;
    data.score = m_score;
    data.coinCount = m_coinCount;
    data.lives = m_lives;
    data.playerX = m_player.getPosition().x;
    data.playerY = m_player.getPosition().y;

    switch (m_player.getPowerState()) {
        case PowerState::Small: data.powerState = "small"; break;
        case PowerState::Big:   data.powerState = "big";   break;
        case PowerState::Fire:  data.powerState = "fire";  break;
    }

    return SaveManager::save(filepath, data);
}

bool Game::loadGame(const std::string& filepath) {
    SaveData data;
    if (!SaveManager::load(filepath, data)) {
        return false;
    }

    m_score = data.score;
    m_coinCount = data.coinCount;
    m_lives = data.lives;

    m_player.respawn(sf::Vector2f(data.playerX, data.playerY));

    PowerState power = PowerState::Small;
    if (data.powerState == "big") {
        power = PowerState::Big;
    } else if (data.powerState == "fire") {
        power = PowerState::Fire;
    }
    if (power != PowerState::Small) {
        m_player.setPowerState(power);
    }

    return true;
}
