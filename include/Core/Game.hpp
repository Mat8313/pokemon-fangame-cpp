#pragma once

#include <SFML/Graphics.hpp>
#include <string>
#include "World/Map.hpp"
#include "World/TiledMapLoader.hpp"
#include "World/MapRenderer.hpp"
#include "Entity/Player.hpp"
#include "Utils/SaveManager.hpp"
#include "UI/MainMenuScene.hpp"
#include "UI/SelectGameScene.hpp"
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
    float totalPlaytime;

};
