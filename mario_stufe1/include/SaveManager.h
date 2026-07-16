#pragma once
#include <string>
#include "SaveData.h"

// Speichert/laedt einen Spielstand als JSON-Datei. Statische Klasse ohne
// State, analog zu LevelLoader - reine Serialisierungs-Funktionen.
class SaveManager {
public:
    static bool save(const std::string& filepath, const SaveData& data);

    // Gibt false zurueck (statt zu werfen), wenn die Datei fehlt oder
    // ungueltig ist - Aufrufer behandelt das als "kein Spielstand vorhanden".
    static bool load(const std::string& filepath, SaveData& outData);
};
