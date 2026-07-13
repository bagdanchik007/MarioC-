#pragma once
#include <unordered_map>
#include <string>
#include <memory>
#include <SFML/Graphics.hpp>
#include "Animation.h"

// Verwaltet alle Animationen einer Entity und wechselt zwischen ihnen.
// Die States entscheiden WANN gewechselt wird (z.B. "run" -> "jump"),
// der Controller kuemmert sich nur um WIE der Wechsel und das Frame-Update ablaeuft.
class AnimationController {
public:
    void addAnimation(const std::string& name, std::unique_ptr<Animation> animation);

    // Wechselt nur die Animation, wenn sich der Name aendert (verhindert Reset-Flackern).
    void play(const std::string& name, bool forceRestart = false);

    void update(float deltaTime);
    void applyTo(sf::Sprite& sprite) const;

    const std::string& getCurrentAnimationName() const noexcept { return m_currentAnimationName; }

private:
    std::unordered_map<std::string, std::unique_ptr<Animation>> m_animations;
    std::string m_currentAnimationName;
};
