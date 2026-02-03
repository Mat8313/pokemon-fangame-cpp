#include "Core/Game.hpp"
#include "UI/MainMenuScene.hpp"
#include "UI/SelectGameScene.hpp"
#include <iostream>
#include <cmath>
#include <memory>

using namespace std;

Game::Game()
    : window(sf::VideoMode(800, 576), "Pokemon Fangame"),
      zoomFactor(2.0f),
      map(0, 0, 16.f),
      currentMapName("Bourg-Palette"),
      totalPlaytime(0.0f),
      currentSceneType(SceneType::MainMenu),
      isRunning(true)
{
    window.setFramerateLimit(60);
    cout << "Fenetre SFML creee avec succes !" << endl;
    
    // Initialiser la première scène (Menu principal)
    currentScene = std::make_unique<MainMenuScene>();
    
    cout << "Game initialized" << endl;
}

void Game::saveGame() {
    currentGameState.player.name = player.getName();
    currentGameState.player.position.x = player.getPositionX();
    currentGameState.player.position.y = player.getPositionY();
    currentGameState.player.currentMap = currentMapName;
    currentGameState.player.playtime = static_cast<int>(totalPlaytime);
    currentGameState.playtime = static_cast<int>(totalPlaytime);
    
    if (saveManager.saveGame(currentGameState, 1)) {
        std::cout << "Game saved successfully!" << std::endl;
    }
}

void Game::run() {
    sf::Clock fpsTimer;
    
    while (window.isOpen() && isRunning) {
        float deltaTime = clock.restart().asSeconds();
        
        
        // Pendant le gameplay, tracker le temps
        if (currentSceneType == SceneType::Gameplay) {
            totalPlaytime += deltaTime;
        }
        
        processEvents();
        update(deltaTime);
        render();
    }
}

void Game::resetViewToDefault() {
    sf::View defaultView(sf::FloatRect(0, 0, 800, 576));
    window.setView(defaultView);
}

void Game::processEvents() {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            window.close();
            isRunning = false;
        }
        
        if (event.type == sf::Event::KeyPressed) {
            if (event.key.code == sf::Keyboard::Escape) {
                // Retour au menu depuis le gameplay
                if (currentSceneType == SceneType::Gameplay) {
                    currentSceneType = SceneType::MainMenu;
                    currentScene = std::make_unique<MainMenuScene>();
                    resetViewToDefault();
                } else {
                    window.close();
                    isRunning = false;
                }
            }
            
            // F5 pour sauvegarder
            if (event.key.code == sf::Keyboard::F5 && 
                currentSceneType == SceneType::Gameplay) {
                saveGame();
            }
        }
        
        // Passer l'événement à la scène courante ou au joueur
        if (currentSceneType == SceneType::Gameplay) {
            player.handleInput(event);  // ✅ Les inputs vont au joueur
        } else {
            if (currentScene) {
                currentScene->handleInput(event);  // Les inputs vont au menu
            }
        }
    }
}


void Game::update(float deltaTime) {
    if (!currentScene) return;
    
    currentScene->update(deltaTime);
    
    // Ne fais la transition qu'UNE FOIS
    static bool alreadyTransitioned = false;
    if (currentScene->shouldTransition() && !alreadyTransitioned) {
        transitionScene();
        alreadyTransitioned = true;
    }
    
    // Réinitialise le flag si on change de scène
    if (currentSceneType != SceneType::Gameplay) {
        alreadyTransitioned = false;
    }
    
    // UPDATE DU GAMEPLAY
    if (currentSceneType == SceneType::Gameplay) {
        player.update(deltaTime);
        handleWarp();
        
        sf::View view = window.getDefaultView();
        view.setCenter(
            std::round(player.getPositionX()),
            std::round(player.getPositionY())
        );
        view.zoom(1.0f / zoomFactor);
        window.setView(view);
    }
}
void Game::render() {
    if (currentSceneType == SceneType::Gameplay) {
        window.clear(sf::Color::Black);
        renderer.render(window, map);
        player.draw(window);
        window.display();
    } else {
        // Les menus gèrent leur propre rendu
        if (currentScene) {
            currentScene->render(window);
        }
    }
}

void Game::transitionScene() {
    SceneType nextScene = currentScene->getNextScene();
    
    cout << "Transitioning from " << static_cast<int>(currentSceneType) 
         << " to " << static_cast<int>(nextScene) << endl;
    
    switch (nextScene) {
        case SceneType::MainMenu:
            currentSceneType = SceneType::MainMenu;
            currentScene = std::make_unique<MainMenuScene>();
            resetViewToDefault();
            break;
            
        case SceneType::SelectGame:
            currentSceneType = SceneType::SelectGame;
            currentScene = std::make_unique<SelectGameScene>();
            break;
            
        case SceneType::NewGameMenu:
            currentSceneType = SceneType::Gameplay;
            loadMap(currentMapName);
            player.setMap(&map);
            player.setPositionX(19 * 16.f);
            player.setPositionY(22 * 16.f);
            player.setMoveSpeed(64.0);
            totalPlaytime = 0.0f;
            cout << "New Game started" << endl;
            break;
            
        case SceneType::Gameplay: {
            currentSceneType = SceneType::Gameplay;
            
            if (saveManager.loadGame(currentGameState, 1)) {
                loadMap(currentGameState.player.currentMap);
                player.setMap(&map);
                player.setPositionX(currentGameState.player.position.x);
                player.setPositionY(currentGameState.player.position.y);
                player.setMoveSpeed(64.0);
                totalPlaytime = currentGameState.playtime;
                cout << "Game loaded and started" << endl;
            } else {
                loadMap(currentMapName);
                player.setMap(&map);
                player.setPositionX(19 * 16.f);
                player.setPositionY(22 * 16.f);
                player.setMoveSpeed(64.0);
                totalPlaytime = 0.0f;
            }
            break;
        }
        case SceneType::Quit:
            window.close();
            isRunning = false;
            cout << "Game closed" << endl;
            break;
            
        default:
            break;
    }
    
    // Reset du flag après transition
    // (La nouvelle scène doit avoir shouldTransition() = false)
}


void Game::loadMap(const std::string& mapName) {
    std::string path = "../assets/map/" + mapName + ".tmx";
    try {
        tiledLoader.loadFromTiledTmx(path, map);
        cout << "Map loaded: " << map.getWidth()
             << "x" << map.getHeight()
             << " (" << mapName << ")" << endl;
    } catch (const std::exception& e) {
        cerr << "Error loading map " << mapName << ": " << e.what() << endl;
    }
}

void Game::handleWarp() {
    if (player.hasWarpRequest()) {
        Warp w = player.consumeWarpRequest();
        
        currentMapName = w.targetMap;
        std::string newMapPath = "../assets/map/" + currentMapName + ".tmx";
        
        try {
            tiledLoader.loadFromTiledTmx(newMapPath, map);
            std::cout << "Warp to map: " << currentMapName
                      << " (" << w.targetX << "," << w.targetY << ")\n";
        } catch (const std::exception& e) {
            std::cerr << "Error loading target map: " << e.what() << std::endl;
        }
        
        player.setMap(&map);
        float tileSize = map.getTileSize();
        player.setPositionX(w.targetX * tileSize);
        player.setPositionY(w.targetY * tileSize);
    }
}

void Game::loadGame() {
    if (saveManager.loadGame(currentGameState, 1)) {
        loadMap(currentGameState.player.currentMap);
        player.setMap(&map);
        player.setPositionX(currentGameState.player.position.x);
        player.setPositionY(currentGameState.player.position.y);
        totalPlaytime = currentGameState.playtime;
        std::cout << "Game loaded successfully!" << std::endl;
    }
}