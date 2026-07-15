#pragma once
#include "IGameState.h"

class MenuState final : public IGameState {
public:
    void enter(Game& game) override;
    void handleEvent(Game& game, const sf::Event& event) override;
    void update(Game& game, float deltaTime) override;
    void render(Game& game, sf::RenderWindow& window) override;
    const char* name() const override { return "menu"; }
};

class PlayingState final : public IGameState {
public:
    void enter(Game& game) override;
    void handleEvent(Game& game, const sf::Event& event) override;
    void update(Game& game, float deltaTime) override;
    void render(Game& game, sf::RenderWindow& window) override;
    const char* name() const override { return "playing"; }
};

// Friert die Spielwelt komplett ein (kein Entity-Update) und legt nur ein
// abgedunkeltes Overlay ueber den letzten Frame.
class PausedState final : public IGameState {
public:
    void enter(Game& game) override;
    void handleEvent(Game& game, const sf::Event& event) override;
    void update(Game& game, float deltaTime) override;
    void render(Game& game, sf::RenderWindow& window) override;
    const char* name() const override { return "paused"; }
};

class GameOverState final : public IGameState {
public:
    void enter(Game& game) override;
    void handleEvent(Game& game, const sf::Event& event) override;
    void update(Game& game, float deltaTime) override;
    void render(Game& game, sf::RenderWindow& window) override;
    const char* name() const override { return "gameover"; }
};

class LevelCompleteState final : public IGameState {
public:
    void enter(Game& game) override;
    void handleEvent(Game& game, const sf::Event& event) override;
    void update(Game& game, float deltaTime) override;
    void render(Game& game, sf::RenderWindow& window) override;
    const char* name() const override { return "levelcomplete"; }
};
