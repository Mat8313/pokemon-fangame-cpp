#pragma once

#include <SFML/Graphics.hpp>
#include <string>
#include "Map.hpp"
#include "TiledMapLoader.hpp"
#include "MapRenderer.hpp"
#include "Player.hpp"

class Game {
public:
    Game();
    void run(); // boucle principale

private:
    void processEvents();
    void update(float deltaTime);
    void render();
    void loadMap(const std::string& mapName);
    void handleWarp(); // gestion des warps

private:
    sf::RenderWindow window;
    float zoomFactor;

    Map map;
    TiledMapLoader tiledLoader;
    MapRenderer renderer;
    Player player;

    std::string currentMapName;
    sf::Clock clock;
};
