#include "../include/SoundManager.h"
#include <algorithm>

bool SoundManager::registerSound(const std::string& name, const std::string& filepath) {
    sf::SoundBuffer buffer;
    if (!buffer.loadFromFile(filepath)) {
        return false; // Datei fehlt/ist ungueltig - bewusst kein Crash, kein throw.
    }
    m_buffers[name] = std::move(buffer);
    return true;
}

void SoundManager::play(const std::string& name, float volume) const {
    const auto it = m_buffers.find(name);
    if (it == m_buffers.end()) {
        return; // Sound war nie registriert (z.B. Datei fehlte) - stilles No-op.
    }

    // Abgelaufene Sounds aus dem Pool entfernen, bevor ein neuer dazukommt -
    // verhindert unbegrenztes Wachstum von m_activeSounds ueber die Spielzeit.
    m_activeSounds.erase(
        std::remove_if(m_activeSounds.begin(), m_activeSounds.end(),
                        [](const sf::Sound& s) { return s.getStatus() == sf::Sound::Stopped; }),
        m_activeSounds.end());

    sf::Sound sound;
    sound.setBuffer(it->second);
    sound.setVolume(volume);
    sound.play();
    m_activeSounds.push_back(std::move(sound));
}

bool SoundManager::playMusic(const std::string& filepath, bool loop) {
    auto music = std::make_unique<sf::Music>();
    if (!music->openFromFile(filepath)) {
        return false;
    }
    m_music = std::move(music);
    m_music->setLoop(loop);
    m_music->play();
    return true;
}

void SoundManager::stopMusic() {
    if (m_music) {
        m_music->stop();
    }
}

void SoundManager::setMusicVolume(float volume) {
    if (m_music) {
        m_music->setVolume(volume);
    }
}
