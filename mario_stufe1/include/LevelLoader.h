#pragma once
#include <string>
#include "LevelData.h"

// Laedt ein Level aus einer JSON-Datei in ein LevelData-Struct.
// Statische Klasse ohne State - reine Parsing-Funktion, analog zu
// CollisionSystem/InteractionSystem.
//
// Erwartetes JSON-Format (siehe assets/levels/level1.json):
// {
//   "tileSize": 32,
//   "tiles": ["....", "####", ...],           // 'P' markiert den Spawnpunkt
//   "enemies": [ { "type": "goomba", "x": 15, "y": 17 } ],   // x/y in TILES, nicht Pixel
//   "items":   [ { "type": "coin", "x": 30, "y": 12 } ]
// }
class LevelLoader {
public:
    // Gibt false zurueck (statt zu werfen/abzustuerzen), wenn die Datei fehlt
    // oder ungueltiges JSON enthaelt - der Aufrufer (Game) faellt dann auf ein
    // eingebautes Fallback-Level zurueck. Gleiche "graceful degradation" wie
    // bei fehlenden Grafik-/Audio-Assets in Etappe 1/5.
    static bool loadFromFile(const std::string& filepath, LevelData& outData);
};
