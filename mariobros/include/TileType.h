#pragma once
#include <cstdint>

// Der Typ eines einzelnen Tiles. Bewusst eine geschlossene enum class statt
// Bitmasken - die Typen schliessen sich in Etappe 2 gegenseitig aus
// (ein Tile ist entweder solide ODER Schaden ODER Muenzblock).
enum class TileType : std::uint8_t {
    Empty = 0,
    Solid,
    Damage,
    CoinBlock,
};

// Zentrale Stelle, die definiert, WELCHES Verhalten zu welchem Tile-Typ gehoert.
// Neue Eigenschaften (z.B. "bounces", "breakable") werden hier ergaenzt,
// nicht verstreut im Kollisions- oder Rendercode.
struct TileProperties {
    static bool isSolid(TileType type) noexcept {
        return type == TileType::Solid || type == TileType::CoinBlock;
    }
    static bool isDamaging(TileType type) noexcept {
        return type == TileType::Damage;
    }
};
