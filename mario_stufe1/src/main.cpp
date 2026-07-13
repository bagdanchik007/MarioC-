#include "../include/Game.h"

// Absichtlich minimal: main() delegiert sofort an Game.
// Keine Spiellogik hier - so bleibt main.cpp stabil, auch wenn Game
// spaeter um Level, Camera, StateManager etc. waechst.
int main() {
    Game game;
    game.run();
    return 0;
}
