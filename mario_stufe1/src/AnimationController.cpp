#include "../include/AnimationController.h"

void AnimationController::addAnimation(const std::string& name, std::unique_ptr<Animation> animation) {
    m_animations[name] = std::move(animation);
}

void AnimationController::play(const std::string& name, bool forceRestart) {
    if (m_currentAnimationName == name && !forceRestart) {
        return; // Kein Wechsel noetig - verhindert Frame-Reset bei jedem Update-Aufruf.
    }
    auto it = m_animations.find(name);
    if (it == m_animations.end()) {
        return; // Unbekannte Animation - bewusst kein Crash, nur ignorieren.
    }
    m_currentAnimationName = name;
    it->second->reset();
}

void AnimationController::update(float deltaTime) {
    auto it = m_animations.find(m_currentAnimationName);
    if (it != m_animations.end()) {
        it->second->update(deltaTime);
    }
}

void AnimationController::applyTo(sf::Sprite& sprite) const {
    auto it = m_animations.find(m_currentAnimationName);
    if (it == m_animations.end()) {
        return;
    }
    sprite.setTexture(it->second->getTexture());
    sprite.setTextureRect(it->second->getCurrentFrame());
}
