#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <functional>

// Eine einzelne Animation: eine Sequenz von Frames (Ausschnitten) aus einem
// gemeinsamen Spritesheet-Texture. Kennt nichts ueber den Player oder Zustaende.
//
// Etappe 5: onFinished-Callback ergaenzt. Damit koennen States (z.B.
// AttackState) auf das ECHTE Ende einer Animation reagieren, statt einen
// separaten Timer zu fuehren, der die Animationslaenge dupliziert - sobald
// echte Spritesheets eingebunden sind, laufen beide (Timer und Animation)
// sonst leicht auseinander.
class Animation {
public:
    Animation(const sf::Texture& texture, std::vector<sf::IntRect> frames,
              float frameDuration, bool loop = true);

    void update(float deltaTime);
    void reset();

    const sf::Texture& getTexture() const noexcept { return m_texture; }
    sf::IntRect getCurrentFrame() const;
    std::size_t getCurrentFrameIndex() const noexcept { return m_currentFrameIndex; }
    bool hasFinished() const noexcept { return m_finished; }

    // Wird EINMAL aufgerufen, in dem Frame, in dem eine nicht-loopende
    // Animation ihr letztes Bild erreicht. Bei loopenden Animationen (loop=true)
    // wird der Callback nie ausgeloest, da hasFinished() dort nie true wird.
    void setOnFinished(std::function<void()> callback) { m_onFinished = std::move(callback); }

private:
    const sf::Texture& m_texture;
    std::vector<sf::IntRect> m_frames;
    float m_frameDuration;
    float m_elapsedTime{0.f};
    std::size_t m_currentFrameIndex{0};
    bool m_loop;
    bool m_finished{false};

    std::function<void()> m_onFinished;
    bool m_finishedCallbackFired{false};
};
