#include "../include/ParticleSystem.h"
#include <algorithm>
#include <random>
#include <cmath>

namespace {
// Thread-lokaler Zufallsgenerator, einmalig mit echtem Entropy-Seed
// initialisiert - kein Bezug zu Gameplay-Determinismus noetig, das ist rein
// kosmetisch (im Gegensatz zu Etappe 6, wo ein deterministischer Seed fuer
// Level-Generierung sinnvoll waere).
std::mt19937& rng() {
    static std::mt19937 generator(std::random_device{}());
    return generator;
}

float randomRange(float min, float max) {
    std::uniform_real_distribution<float> dist(min, max);
    return dist(rng());
}
}

void ParticleSystem::emitBurst(sf::Vector2f position, sf::Color color, int count) {
    for (int i = 0; i < count; ++i) {
        const float angle = randomRange(0.f, 6.2831853f); // 0 bis 2*Pi
        const float speed = randomRange(40.f, 140.f);

        Particle particle;
        particle.position = position;
        particle.velocity = sf::Vector2f(std::cos(angle) * speed, std::sin(angle) * speed);
        particle.maxLifetime = randomRange(0.3f, 0.6f);
        particle.lifetime = particle.maxLifetime;
        particle.size = randomRange(2.f, 4.f);
        particle.color = color;

        m_particles.push_back(particle);
    }
}

void ParticleSystem::update(float deltaTime) {
    for (auto& particle : m_particles) {
        particle.position += particle.velocity * deltaTime;
        particle.lifetime -= deltaTime;
    }

    // Erase-Remove Idiom: abgelaufene Partikel entfernen.
    m_particles.erase(
        std::remove_if(m_particles.begin(), m_particles.end(),
                        [](const Particle& p) { return p.lifetime <= 0.f; }),
        m_particles.end());
}

void ParticleSystem::render(sf::RenderWindow& window) const {
    if (m_particles.empty()) {
        return;
    }

    sf::VertexArray quads(sf::Quads, m_particles.size() * 4);

    for (std::size_t i = 0; i < m_particles.size(); ++i) {
        const Particle& p = m_particles[i];
        const float alphaRatio = std::max(0.f, p.lifetime / p.maxLifetime);
        sf::Color color = p.color;
        color.a = static_cast<sf::Uint8>(255.f * alphaRatio);

        const float halfSize = p.size / 2.f;
        const std::size_t base = i * 4;
        quads[base + 0].position = p.position + sf::Vector2f(-halfSize, -halfSize);
        quads[base + 1].position = p.position + sf::Vector2f(halfSize, -halfSize);
        quads[base + 2].position = p.position + sf::Vector2f(halfSize, halfSize);
        quads[base + 3].position = p.position + sf::Vector2f(-halfSize, halfSize);

        quads[base + 0].color = color;
        quads[base + 1].color = color;
        quads[base + 2].color = color;
        quads[base + 3].color = color;
    }

    window.draw(quads);
}
