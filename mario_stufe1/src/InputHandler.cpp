#include "../include/InputHandler.h"
#include "../include/Player.h"

InputHandler::InputHandler() = default;

void InputHandler::bindContinuous(sf::Keyboard::Key key, std::unique_ptr<ICommand> command) {
    m_continuousBindings[key] = std::move(command);
}

void InputHandler::bindOnPressed(sf::Keyboard::Key key, std::unique_ptr<ICommand> command) {
    m_onPressedBindings[key] = std::move(command);
}

void InputHandler::bindOnReleased(sf::Keyboard::Key key, std::unique_ptr<ICommand> command) {
    m_onReleasedBindings[key] = std::move(command);
}

void InputHandler::pollContinuousInput(Player& player) const {
    // Polling: eignet sich fuer Bewegung, die sofort und pro Frame reagieren soll.
    bool anyDirectionPressed = false;
    for (const auto& [key, command] : m_continuousBindings) {
        if (sf::Keyboard::isKeyPressed(key)) {
            command->execute(player);
            anyDirectionPressed = true;
        }
    }
    if (!anyDirectionPressed) {
        player.handleAction(PlayerAction::StopHorizontal);
    }
}

void InputHandler::handleEvent(const sf::Event& event, Player& player) const {
    // Events: eignen sich fuer einmalige Aktionen (Jump/Attack) - genau ein
    // Trigger pro Tastendruck, unabhaengig davon wie lange die Taste gehalten wird.
    if (event.type == sf::Event::KeyPressed) {
        auto it = m_onPressedBindings.find(event.key.code);
        if (it != m_onPressedBindings.end()) {
            it->second->execute(player);
        }
    } else if (event.type == sf::Event::KeyReleased) {
        auto it = m_onReleasedBindings.find(event.key.code);
        if (it != m_onReleasedBindings.end()) {
            it->second->execute(player);
        }
    }
}
