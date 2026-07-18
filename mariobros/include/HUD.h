#pragma once
#include <SFML/Graphics.hpp>
#include <string>

// HUD: zeichnet Text-Overlays in fixen Bildschirmkoordinaten (nicht in der
// Welt-Kamera) - Leben/Muenzen/Punkte/Zeit waehrend des Spiels, sowie
// zentrierte Titel-Nachrichten fuer Menu/Pause/GameOver/LevelComplete.
// Ersetzt den bisherigen "Status im Fenstertitel"-Notbehelf aus Etappe 4/5,
// der nur ein Platzhalter war, bis ein Font-Asset verfuegbar war.
//
// Laedt die Schriftart EINMAL im Konstruktor. Fehlt die Datei, wird KEIN
// Text gezeichnet (isLoaded() == false) statt zu crashen - gleiche
// "graceful degradation" wie bei fehlenden Grafik-/Audio-Assets.
class HUD {
public:
    HUD();

    bool isLoaded() const noexcept { return m_loaded; }

    void drawGameplayHUD(sf::RenderWindow& window, int lives, int coins, int score, float timeRemaining) const;

    void drawCenteredMessage(sf::RenderWindow& window, const std::string& title,
                              const std::string& subtitle = "") const;

private:
    sf::Font m_font;
    bool m_loaded{false};
};
