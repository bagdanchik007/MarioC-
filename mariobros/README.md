# Mario Clone (C++ / SFML)

Ein Super-Mario-Bros-Klon in modernem C++20, geschrieben als Lern- und
Portfolio-Projekt. Entwickelt in klar getrennten Etappen — von der
Grundarchitektur bis zu datengetriebenen Levels und einem Save/Load-System.

## Features

- **Player-Physik & State Machine**: Idle, Run, Jump, Fall, Attack — inkl.
  variabler Sprunghöhe (Taste kurz vs. lang gedrückt)
- **AABB-Kollisionssystem** gegen eine Tilemap (Separate-Axis-Sweep)
- **Kamera** mit weichem Follow-Verhalten und Level-Grenzen
- **Gegner-KI** (Patrol/Squish) über Komposition statt Vererbungshierarchie
- **Power-up-System**: Small → Big → Fire, inkl. Fireball-Projektil
- **Game State Machine**: Menü, Spielen, Pause, Game Over, Level geschafft
- **Score, Münzen, Leben, Timer**
- **Partikeleffekte, Event-System, Sound-Manager** (Etappe 5)
- **Datengetriebene Levels** über JSON + **Save/Load** (Etappe 6)
- **On-Screen-HUD** (echtes Textrendering statt Fenstertitel) mit
  DejaVu Sans Bold (frei lizenziert, liegt unter `assets/fonts/` bei)

**→ [Screenshots ansehen](SCREENSHOTS.md)**

## Voraussetzungen

- **Visual Studio 2022** (oder neuer) mit dem Workload
  *"Desktopentwicklung mit C++"*
- Internetverbindung beim ersten Öffnen (lädt SFML und nlohmann/json
  automatisch herunter — siehe unten)
- Git

Keine manuelle Bibliotheks-Installation nötig (kein vcpkg, kein manueller
SFML-Download) — das übernimmt CMake automatisch.

## Herunterladen & Öffnen

```bash
git clone <repository-url>
```

Oder das ZIP herunterladen und entpacken.

Dann in Visual Studio:

1. **Datei → Öffnen → Ordner...**
2. Den Projektordner (der mit `CMakeLists.txt`) auswählen
3. Visual Studio erkennt die `CMakeLists.txt` automatisch und beginnt zu
   konfigurieren (Statusleiste unten: *"CMake-Generierung läuft..."*). Beim
   ersten Mal werden SFML und nlohmann/json von GitHub geladen und lokal
   gebaut — das dauert ein paar Minuten. Jeder weitere Build ist schnell.

## Starten

1. Oben in der Symbolleiste sicherstellen, dass **`MarioClone.exe`** als
   Startobjekt ausgewählt ist (nicht "Alle CMake-Targets")
2. **F5** drücken

## Steuerung

| Taste          | Aktion                          |
|----------------|----------------------------------|
| ← / →          | Laufen                          |
| ↑              | Springen (länger halten = höher) |
| X              | Angriff / Fireball (bei Fire-Power) |
| Escape         | Pause / Fortsetzen               |
| F5             | Spielstand speichern             |
| F9             | Spielstand laden                 |
| Enter          | Menü bestätigen / Neustart nach Game Over |

## Projektstruktur

```
include/    Header-Dateien (eine Klasse = ein Header)
src/        Implementierungen (.cpp)
assets/
  levels/   Level-Definitionen als JSON
  (sfx/, music/ — noch keine echten Dateien vorhanden, siehe unten)
CMakeLists.txt
```

Jede Klasse ist bewusst klein und einzeln kommentiert — Architektur-Entscheidungen
(warum Komposition statt Vererbung bei Enemy, warum Separate-Axis-Kollision,
etc.) stehen direkt als Kommentar über der jeweiligen Klasse im Header.

## Bekannter Stand / was noch fehlt

- **Keine echten Grafiken/Sounds.** Player, Gegner und Items werden aktuell
  nur als einfarbige Platzhalter-Formen gerendert; Sound-Dateien unter
  `assets/sfx/` und `assets/music/` existieren noch nicht. Das
  `AnimationController`- und `SoundManager`-System sind aber vollständig
  fertig verdrahtet — sobald echte Spritesheets/`.wav`-Dateien unter den
  erwarteten Pfaden liegen, funktionieren sie ohne Codeänderung.
- **Ein Level** vorhanden (`assets/levels/level1.json`) — weitere Level
  lassen sich als zusätzliche JSON-Dateien nach demselben Schema anlegen.

## Neues Level anlegen

Levels sind JSON-Dateien unter `assets/levels/`. Koordinaten für Gegner/Items
sind in **Tiles**, nicht Pixel:

```json
{
  "tileSize": 32,
  "tiles": [
    "..................",
    "..................",
    "P.................",
    "##################"
  ],
  "enemies": [
    { "type": "goomba", "x": 10, "y": 2 }
  ],
  "items": [
    { "type": "coin", "x": 5, "y": 1 },
    { "type": "mushroom", "x": 8, "y": 1 },
    { "type": "fireflower", "x": 12, "y": 1 }
  ]
}
```

`P` im Tile-Raster markiert den Spawnpunkt. Gültige Tile-Zeichen: `.` (leer),
`#` (solide), `^` (Schaden), `?` (Münzblock). Gültige `enemies`-Typen:
`goomba`. Gültige `items`-Typen: `coin`, `mushroom`, `fireflower`.
