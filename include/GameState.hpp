#ifndef GAMESTATE_HPP
#define GAMESTATE_HPP

#include <string>

struct Vec2f {
    float x, y;
};

struct PlayerState {
    std::string name;
    Vec2f position;
    std::string currentMap;
    int playtime;  // en secondes
};

struct GameState {
    PlayerState player;
    int playtime;  // Temps total de jeu
    std::string lastSaveTime;
    int slotNumber;  // Pour sauvegarde multiple
    
    GameState() = default;
    GameState(const PlayerState& p) : player(p), playtime(0), slotNumber(1) {}
};

#endif
