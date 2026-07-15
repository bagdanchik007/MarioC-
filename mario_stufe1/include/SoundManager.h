#pragma once
#include <SFML/Audio.hpp>
#include <unordered_map>
#include <vector>
#include <string>
#include <memory>

// Zentrale Verwaltung von Soundeffekten + Hintergrundmusik. Sounds werden per
// Namen registriert (registerSound) und ueber diesen Namen abgespielt
// (play("jump")) - Aufrufer muessen keine sf::SoundBuffer/sf::Sound-Details kennen.
//
// WICHTIG: Fehlt eine Audiodatei (z.B. weil noch keine Assets vorhanden sind),
// wird das sauber ignoriert statt zu werfen/abzustuerzen - registerSound()
// gibt dann false zurueck, und play() auf einen nie geladenen Namen ist ein
// No-op. Das Spiel bleibt so vollstaendig lauffaehig, auch komplett ohne
// Audio-Assets (aktuell der Fall - siehe Game::Game()).
class SoundManager {
public:
    bool registerSound(const std::string& name, const std::string& filepath);
    void play(const std::string& name, float volume = 100.f) const;

    bool playMusic(const std::string& filepath, bool loop = true);
    void stopMusic();
    void setMusicVolume(float volume);

private:
    std::unordered_map<std::string, sf::SoundBuffer> m_buffers;

    // Pool statt EINEM sf::Sound: mehrere Soundeffekte muessen ueberlappend
    // spielen koennen (z.B. zwei Muenzen kurz hintereinander). mutable, da
    // play() konzeptionell "const" ist (aendert keinen sichtbaren Zustand des
    // SoundManagers), aber SFML intern den Wiedergabe-Status verwaltet.
    mutable std::vector<sf::Sound> m_activeSounds;

    std::unique_ptr<sf::Music> m_music;
};
