#pragma once

// Aktuell nur Goomba implementiert. Neue Gegnertypen werden hier als weiterer
// Enum-Wert ergaenzt + ein Eintrag in den Tuning-Tabellen in Enemy.cpp
// (Geschwindigkeit, Groesse, Punktwert) - keine neue Klasse noetig, siehe
// Architektur-Kommentar in Enemy.h.
enum class EnemyType {
    Goomba,
};
