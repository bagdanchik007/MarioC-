#pragma once
#include <vector>
#include <SFML/Graphics.hpp>

// Einfaches Partikelsystem fuer kurzlebige visuelle Effekte (Stomp-Staub,
// Muenzen-Funkeln). Partikel sind reine Daten (kein Entity/Component) - sie
// nehmen nicht an Physik/Kollision teil und leben nur Sekundenbruchteile,
// eine Verwaltung ueber EntityManager/CollisionSystem waere hier Overkill.
//
// Rendering ueber ein pro-Frame neu aufgebautes sf::VertexArray (Quads) statt
// einem sf::CircleShape pro Partikel - ein einziger Draw-Call fuer alle
// Partikel ist deutlich guenstiger als N einzelne Draw-Calls.
class ParticleSystem {
public:
    // Erzeugt 'count' Partikel an 'position' mit zufaelliger Richtung/Geschwindigkeit,
    // die ueber ihre Lebenszeit ausblassen (Alpha-Fade).
    void emitBurst(sf::Vector2f position, sf::Color color, int count);

    void update(float deltaTime);
    void render(sf::RenderWindow& window) const;

private:
    struct Particle {
        sf::Vector2f position;
        sf::Vector2f velocity;
        float lifetime;
        float maxLifetime;
        float size;
        sf::Color color;
    };

    std::vector<Particle> m_particles;
};
