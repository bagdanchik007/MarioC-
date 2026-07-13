#pragma once
#include <SFML/Graphics.hpp>
#include <vector>

// Eine einzelne Animation: eine Sequenz von Frames (Ausschnitten) aus einem
// gemeinsamen Spritesheet-Texture. Kennt nichts ueber den Player oder Zustaende.
class Animation {
public:
    Animation(const sf::Texture& texture, std::vector<sf::IntRect> frames,
              float frameDuration, bool loop = true);

    void update(float deltaTime);
    void reset();

    const sf::Texture& getTexture() const noexcept { return m_texture; }
    sf::IntRect getCurrentFrame() const;
    bool hasFinished() const noexcept { return m_finished; }

private:
    const sf::Texture& m_texture;
    std::vector<sf::IntRect> m_frames;
    float m_frameDuration;
    float m_elapsedTime{0.f};
    std::size_t m_currentFrameIndex{0};
    bool m_loop;
    bool m_finished{false};
};
