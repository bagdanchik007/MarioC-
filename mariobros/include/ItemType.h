#pragma once

// Aktuell drei Item-Typen. Wie bei EnemyType: neuer Typ = neuer Enum-Wert +
// Tabellenzeile in Item.cpp, keine neue Klasse.
enum class ItemType {
    Coin,
    Mushroom,
    FireFlower,
};
