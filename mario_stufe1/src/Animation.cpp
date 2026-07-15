#include "../include/Animation.h"

Animation::Animation(const sf::Texture& texture, std::vector<sf::IntRect> frames,
                      float frameDuration, bool loop)
    : m_texture(texture)
    , m_frames(std::move(frames))
    , m_frameDuration(frameDuration)
    , m_loop(loop) {}

void Animation::update(float deltaTime) {
    if (m_finished || m_frames.empty()) {
        return;
    }
    m_elapsedTime += deltaTime;
    if (m_elapsedTime >= m_frameDuration) {
        m_elapsedTime -= m_frameDuration;
        ++m_currentFrameIndex;
        if (m_currentFrameIndex >= m_frames.size()) {
            if (m_loop) {
                m_currentFrameIndex = 0;
            } else {
                m_currentFrameIndex = m_frames.size() - 1;
                m_finished = true;
            }
        }
    }

    if (m_finished && !m_finishedCallbackFired) {
        m_finishedCallbackFired = true;
        if (m_onFinished) {
            m_onFinished();
        }
    }
}

void Animation::reset() {
    m_elapsedTime = 0.f;
    m_currentFrameIndex = 0;
    m_finished = false;
    m_finishedCallbackFired = false;
}

sf::IntRect Animation::getCurrentFrame() const {
    if (m_frames.empty()) {
        return {};
    }
    return m_frames[m_currentFrameIndex];
}
