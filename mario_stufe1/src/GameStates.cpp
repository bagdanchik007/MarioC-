#include "../include/GameStates.h"
#include "../include/Game.h"
#include "../include/CollisionSystem.h"
#include "../include/InteractionSystem.h"
#include <memory>
#include <string>

// ============================================================================
// MenuState
// ============================================================================
void MenuState::enter(Game& game) {
    game.getWindow().setTitle("Mario Clone - MENU | Enter zum Start");
}

void MenuState::handleEvent(Game& game, const sf::Event& event) {
    if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Enter) {
        game.setState(std::make_unique<PlayingState>());
    }
}

void MenuState::update(Game& /*game*/, float /*deltaTime*/) {
    // Bewusst leer: im Menue passiert keine Simulation, nur Warten auf Input.
}

void MenuState::render(Game& /*game*/, sf::RenderWindow& window) {
    // Kein Font-Asset vorhanden (Etappe 5 bringt echtes HUD/Text) - deshalb
    // hier nur eine Farbflaeche als Platzhalter; der eigentliche Text steht
    // im Fenstertitel (siehe enter()).
    window.clear(sf::Color(20, 20, 60));
    window.display();
}

// ============================================================================
// PlayingState - die eigentliche Gameplay-Simulation, vorher in Game::update()
// ============================================================================
void PlayingState::enter(Game& game) {
    game.getWindow().setTitle("Mario Clone - Etappe 4");
}

void PlayingState::handleEvent(Game& game, const sf::Event& event) {
    if (event.type == sf::Event::KeyPressed) {
        if (event.key.code == sf::Keyboard::Escape) {
            game.setState(std::make_unique<PausedState>());
            return;
        }
        if (event.key.code == sf::Keyboard::F5) {
            game.saveGame("savegame.json"); // Rueckgabewert bewusst ignoriert -
            return;                          // ein fehlgeschlagener Save ist hier kein Grund zum Abbruch.
        }
        if (event.key.code == sf::Keyboard::F9) {
            game.loadGame("savegame.json"); // Ebenso: kein Spielstand vorhanden -> einfach nichts tun.
            return;
        }
    }
    // Diskrete Aktionen (Jump/Attack) nur waehrend des Spielens verarbeiten.
    game.getInput().handleEvent(event, game.getPlayer());
}

void PlayingState::update(Game& game, float deltaTime) {
    Player& player = game.getPlayer();
    Level& level = game.getLevel();

    game.getInput().pollContinuousInput(player);

    // 1) Player: Gravitation + State-Update, dann Kollisionsaufloesung.
    player.update(deltaTime);
    const CollisionResult playerCollision = CollisionSystem::resolve(player, level.getTilemap(), deltaTime);
    player.setOnGround(playerCollision.onGround);

    // Event-Marker abholen, die States nur GESETZT haben (Player kennt den
    // EventBus nicht) - hier werden daraus echte, publizierte GameEvents.
    if (player.consumeJumpEvent()) {
        game.getEvents().publish({EventType::PlayerJumped, player.getPosition(), 0});
    }
    if (player.consumeLandedEvent()) {
        game.getEvents().publish({EventType::PlayerLanded, player.getPosition(), 0});
    }

    // 2) Enemies: gleiche Physik-Pipeline, wiederverwendet ueber CollisionSystem.
    //
    // Etappe 6 - Optimierung: einfaches Culling. Bei nur 2 Gegnern in einem
    // 50-Tile-Level macht das keinen messbaren Unterschied, aber das Muster
    // zahlt sich aus, sobald ein Level viele Dutzend Gegner hat, die weit
    // ausserhalb des Bildschirms warten (klassisches Update-Distanz-Problem
    // in Plattformern). Statt jeden Gegner jeden Frame zu simulieren, wird
    // nur simuliert, wer sich (mit Rand-Marge) im sichtbaren Bereich befindet.
    sf::FloatRect activeBounds = game.getCamera().getViewBounds();
    constexpr float CULL_MARGIN = 200.f;
    activeBounds.left -= CULL_MARGIN;
    activeBounds.top -= CULL_MARGIN;
    activeBounds.width += CULL_MARGIN * 2.f;
    activeBounds.height += CULL_MARGIN * 2.f;

    for (auto& enemyPtr : game.getEnemies().getAll()) {
        Enemy& enemy = *enemyPtr;
        if (!activeBounds.intersects(enemy.getBounds())) {
            continue; // ausserhalb des aktiven Bereichs - diesen Frame nicht simulieren
        }
        enemy.update(deltaTime);
        const CollisionResult enemyCollision = CollisionSystem::resolve(enemy, level.getTilemap(), deltaTime);
        enemy.setOnGround(enemyCollision.onGround);
        enemy.setHitWall(enemyCollision.hitWall);
    }

    // 3) Items: kein Kollisionscheck noetig (statische Trigger, nur Schweb-Animation).
    game.getItems().update(deltaTime);

    // 4) Fireballs: eigene geradlinige Bewegung, kein CollisionSystem noetig.
    game.getFireballs().update(deltaTime);
    if (player.consumeFireballRequest()) {
        const float direction = player.isFacingRight() ? 1.f : -1.f;
        const sf::Vector2f spawnPos = player.getFireballSpawnPosition();
        game.getFireballs().add(std::make_unique<Fireball>(spawnPos, direction));
        game.getEvents().publish({EventType::FireballFired, spawnPos, 0});
    }

    // 5) Interaktionen: NACH der Bewegung, damit Positionen fuer diesen Frame final sind.
    game.addScore(InteractionSystem::resolvePlayerEnemies(player, game.getEnemies().getAll(), game.getEvents()));
    game.addScore(InteractionSystem::resolveFireballEnemies(game.getFireballs().getAll(), game.getEnemies().getAll(),
                                                             game.getEvents()));

    const ItemPickupResult pickup = InteractionSystem::resolvePlayerItems(player, game.getItems().getAll(),
                                                                           game.getEvents());
    game.addCoins(pickup.coinsCollected);
    game.addScore(pickup.scoreGained);

    // 6) Tote Entities entfernen - ERST NACH den Interaktionen, die isAlive() lesen.
    game.getEnemies().removeDead();
    game.getItems().removeDead();
    game.getFireballs().removeDead();

    // 7) Partikel: eigener Lebenszyklus, unabhaengig von Entities.
    game.getParticles().update(deltaTime);

    game.getCamera().follow(player.getPosition(), deltaTime);

    // 7) Timer: laeuft ab -> zaehlt wie ein toedlicher Treffer.
    game.tickTimer(deltaTime);
    if (game.getTimeRemaining() <= 0.f && player.isAlive()) {
        player.kill();
    }

    // 8) Tod behandeln: Leben abziehen, respawnen oder Game Over.
    if (!player.isAlive()) {
        game.loseLife();
        if (game.getLives() <= 0) {
            game.setState(std::make_unique<GameOverState>());
            return;
        }
        player.respawn(level.getPlayerSpawn());
        game.resetTimer();
    }

    // 9) Levelende: rechter Bereich der Tilemap erreicht.
    const float levelEndX = static_cast<float>(level.getTilemap().getWidth()) * Game::TILE_SIZE
                             - 5.f * Game::TILE_SIZE;
    if (player.getPosition().x >= levelEndX) {
        game.setState(std::make_unique<LevelCompleteState>());
        return;
    }

    // HUD-Ersatz ohne Font-Asset: Statuszeile im Fenstertitel (Etappe 5 bringt
    // ein echtes On-Screen-HUD mit Textrendering).
    game.getWindow().setTitle(
        "Mario Clone - Etappe 4 | Leben: " + std::to_string(game.getLives()) +
        " | Muenzen: " + std::to_string(game.getCoinCount()) +
        " | Punkte: " + std::to_string(game.getScore()) +
        " | Zeit: " + std::to_string(static_cast<int>(game.getTimeRemaining())));
}

void PlayingState::render(Game& game, sf::RenderWindow& window) {
    window.clear(sf::Color(92, 148, 252)); // Mario-typisches Himmelblau
    window.setView(game.getCamera().getView());

    game.getLevel().render(window);
    game.getItems().render(window);
    game.getEnemies().render(window);
    game.getFireballs().render(window);
    game.getPlayer().render(window);
    game.getParticles().render(window);

    window.display();
}

// ============================================================================
// PausedState
// ============================================================================
void PausedState::enter(Game& game) {
    game.getWindow().setTitle("Mario Clone - PAUSE (Escape zum Fortsetzen)");
}

void PausedState::handleEvent(Game& game, const sf::Event& event) {
    if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
        game.setState(std::make_unique<PlayingState>());
    }
}

void PausedState::update(Game& /*game*/, float /*deltaTime*/) {
    // Bewusst leer: die Spielwelt friert waehrend der Pause komplett ein -
    // kein Update fuer Player/Enemies/Items/Timer. Das ist der ganze Sinn
    // dieses States: ein "Nicht-Update" ist hier die korrekte Implementierung.
}

void PausedState::render(Game& game, sf::RenderWindow& window) {
    // Letzten Spielstand weiter anzeigen (eingefroren), mit abgedunkeltem Overlay.
    window.setView(game.getCamera().getView());
    game.getLevel().render(window);
    game.getItems().render(window);
    game.getEnemies().render(window);
    game.getPlayer().render(window);

    window.setView(window.getDefaultView());
    sf::RectangleShape overlay(sf::Vector2f(window.getSize()));
    overlay.setFillColor(sf::Color(0, 0, 0, 150));
    window.draw(overlay);

    window.display();
}

// ============================================================================
// GameOverState
// ============================================================================
void GameOverState::enter(Game& game) {
    game.getWindow().setTitle("Mario Clone - GAME OVER | Enter fuer Neustart");
}

void GameOverState::handleEvent(Game& game, const sf::Event& event) {
    if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Enter) {
        game.resetGame();
        game.setState(std::make_unique<PlayingState>());
    }
}

void GameOverState::update(Game& /*game*/, float /*deltaTime*/) {}

void GameOverState::render(Game& /*game*/, sf::RenderWindow& window) {
    window.clear(sf::Color(120, 0, 0));
    window.display();
}

// ============================================================================
// LevelCompleteState
// ============================================================================
void LevelCompleteState::enter(Game& game) {
    game.getWindow().setTitle("Mario Clone - LEVEL GESCHAFFT! Punkte: "
        + std::to_string(game.getScore()) + " | Enter fuers Menue");
}

void LevelCompleteState::handleEvent(Game& game, const sf::Event& event) {
    if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Enter) {
        game.resetGame();
        game.setState(std::make_unique<MenuState>());
    }
}

void LevelCompleteState::update(Game& /*game*/, float /*deltaTime*/) {}

void LevelCompleteState::render(Game& /*game*/, sf::RenderWindow& window) {
    window.clear(sf::Color(0, 120, 0));
    window.display();
}
