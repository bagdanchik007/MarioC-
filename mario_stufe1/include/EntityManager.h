#pragma once
#include <vector>
#include <memory>
#include <algorithm>
#include <SFML/Graphics.hpp>

// Generischer Container fuer eine Menge gleichartiger Entities (Enemies,
// Coins, spaeter Projectiles/Items...). Header-only Template - eine
// .h/.cpp-Trennung wuerde hier nur Explicit-Instantiation-Boilerplate
// erzeugen, ohne echten Vorteil, da Templates zur Compile-Zeit instanziiert
// werden muessen.
//
// Bewusst KEIN einzelner polymorpher vector<unique_ptr<Entity>> fuer ALLE
// Objektarten zusammen: Player, Enemy und Coin brauchen unterschiedliche,
// typ-spezifische Interaktionslogik (Stomp nur bei Enemy, Einsammeln nur bei
// Coin). Ein generischer vector<Entity*> wuerde dynamic_cast/RTTI im
// InteractionSystem erzwingen. Getrennte, typisierte Manager sind hier
// einfacher und schneller - ein echtes ECS (Etappe 6, falls gewuenscht) waere
// die Alternative, wenn die Anzahl der Objekttypen stark waechst.
template <typename T>
class EntityManager {
public:
    T& add(std::unique_ptr<T> entity) {
        m_entities.push_back(std::move(entity));
        return *m_entities.back();
    }

    void update(float deltaTime) {
        for (auto& entity : m_entities) {
            entity->update(deltaTime);
        }
    }

    void render(sf::RenderWindow& window) {
        for (auto& entity : m_entities) {
            entity->render(window);
        }
    }

    // Erase-Remove Idiom: entfernt alle Entities, die sich selbst als "tot"
    // markiert haben (Enemy nach Squish-Animation, Coin nach Einsammeln).
    void removeDead() {
        m_entities.erase(
            std::remove_if(m_entities.begin(), m_entities.end(),
                            [](const std::unique_ptr<T>& e) { return !e->isAlive(); }),
            m_entities.end());
    }

    std::vector<std::unique_ptr<T>>& getAll() noexcept { return m_entities; }
    std::size_t count() const noexcept { return m_entities.size(); }

    // Wird von Game::resetGame() genutzt (GameOver/LevelComplete -> Neustart).
    void clear() noexcept { m_entities.clear(); }

private:
    std::vector<std::unique_ptr<T>> m_entities;
};
