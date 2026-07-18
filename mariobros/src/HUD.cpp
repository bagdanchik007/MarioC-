#include "../include/HUD.h"

HUD::HUD() {
    m_loaded = m_font.loadFromFile("assets/fonts/DejaVuSans-Bold.ttf");
}

void HUD::drawGameplayHUD(sf::RenderWindow& window, int lives, int coins, int score, float timeRemaining) const {
    if (!m_loaded) {
        return;
    }

    // WICHTIG: eigene View in Bildschirmkoordinaten (0,0 bis Fenstergroesse),
    // NICHT die Welt-Kamera - sonst wuerde das HUD mit der Kamera durch die
    // Welt "mitscrollen" statt fix am Bildschirmrand zu kleben. Nach dem
    // Zeichnen wird die vorherige View wiederhergestellt, damit nachfolgender
    // Code (z.B. der naechste Frame) nicht ueberrascht wird.
    const sf::View previousView = window.getView();
    window.setView(window.getDefaultView());

    const std::string line = "Leben: " + std::to_string(lives) +
                              "   Muenzen: " + std::to_string(coins) +
                              "   Punkte: " + std::to_string(score) +
                              "   Zeit: " + std::to_string(static_cast<int>(timeRemaining));

    sf::Text text(line, m_font, 20);
    text.setPosition(12.f, 8.f);
    text.setFillColor(sf::Color::White);
    text.setOutlineColor(sf::Color::Black);
    text.setOutlineThickness(2.f);
    window.draw(text);

    window.setView(previousView);
}

void HUD::drawCenteredMessage(sf::RenderWindow& window, const std::string& title, const std::string& subtitle) const {
    if (!m_loaded) {
        return;
    }

    const sf::View previousView = window.getView();
    window.setView(window.getDefaultView());
    const sf::Vector2f windowSize(window.getSize());

    sf::Text titleText(title, m_font, 36);
    titleText.setFillColor(sf::Color::White);
    titleText.setOutlineColor(sf::Color::Black);
    titleText.setOutlineThickness(3.f);
    const sf::FloatRect titleBounds = titleText.getLocalBounds();
    titleText.setOrigin(titleBounds.width / 2.f, titleBounds.height / 2.f);
    titleText.setPosition(windowSize.x / 2.f, windowSize.y / 2.f - 20.f);
    window.draw(titleText);

    if (!subtitle.empty()) {
        sf::Text subtitleText(subtitle, m_font, 18);
        subtitleText.setFillColor(sf::Color(230, 230, 230));
        subtitleText.setOutlineColor(sf::Color::Black);
        subtitleText.setOutlineThickness(2.f);
        const sf::FloatRect subtitleBounds = subtitleText.getLocalBounds();
        subtitleText.setOrigin(subtitleBounds.width / 2.f, subtitleBounds.height / 2.f);
        subtitleText.setPosition(windowSize.x / 2.f, windowSize.y / 2.f + 30.f);
        window.draw(subtitleText);
    }

    window.setView(previousView);
}
