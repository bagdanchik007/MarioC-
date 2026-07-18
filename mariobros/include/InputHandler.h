#pragma once
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Event.hpp>
#include <unordered_map>
#include <memory>
#include "ICommand.h"

class Player;

// Uebersetzt rohe Tastatureingaben in Commands (Action Mapping).
// Zwei Eingabearten werden bewusst getrennt und ueber getrennte Tabellen gefuehrt:
//  - Continuous-Bindings: per Polling (isKeyPressed) jeden Frame geprueft.
//    Passend fuer Laufen, das sofort und pro Frame reagieren soll.
//  - Event-Bindings: nur bei KeyPressed/KeyReleased-Events ausgeloest.
//    Passend fuer Jump/Attack, die genau einmal pro Tastendruck feuern duerfen.
class InputHandler {
public:
    InputHandler();

    void bindContinuous(sf::Keyboard::Key key, std::unique_ptr<ICommand> command);
    void bindOnPressed(sf::Keyboard::Key key, std::unique_ptr<ICommand> command);
    void bindOnReleased(sf::Keyboard::Key key, std::unique_ptr<ICommand> command);

    // Wird einmal pro Frame fuer kontinuierlichen Input aufgerufen.
    void pollContinuousInput(Player& player) const;

    // Wird pro SFML-Event aufgerufen (fuer diskrete Aktionen wie Jump/Attack).
    void handleEvent(const sf::Event& event, Player& player) const;

private:
    std::unordered_map<sf::Keyboard::Key, std::unique_ptr<ICommand>> m_continuousBindings;
    std::unordered_map<sf::Keyboard::Key, std::unique_ptr<ICommand>> m_onPressedBindings;
    std::unordered_map<sf::Keyboard::Key, std::unique_ptr<ICommand>> m_onReleasedBindings;
};
