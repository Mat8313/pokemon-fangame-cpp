#pragma once

#include <SFML/Graphics.hpp>
#include <string>
#include "Map.hpp"
#include "TiledMapLoader.hpp"
#include "MapRenderer.hpp"
#include "Player.hpp"
#include "SaveManager.hpp"
#include "MainMenuScene.hpp"
#include "SelectGameScene.hpp"
#include <iostream>

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
    SaveManager saveManager;
    GameState currentGameState;
    float totalPlaytime;

    void resetViewToDefault();
    
    sf::RenderWindow window;
    float zoomFactor;

    Map map;
    TiledMapLoader tiledLoader;
    MapRenderer renderer;
    Player player;

    std::string currentMapName;
    sf::Clock clock;

    void saveGame();
    void loadGame();

    void transitionScene();
    std::unique_ptr<Scene> currentScene;
    SceneType currentSceneType;
    bool isRunning;

};
