#include "Game.hpp"
#include <iostream>
#include <cmath>

using namespace std;

Game::Game()
: window(sf::VideoMode(800, 576), "Pokemon Fangame"),
  zoomFactor(2.0f),
  map(0, 0, 16.f),
  currentMapName("Bourg-Palette")
{
    window.setFramerateLimit(60);
    cout << "Fenetre SFML creee avec succes !" << endl;

    loadMap(currentMapName);

    // Init joueur
    player.setMap(&map);
    player.setPositionX(345.f);
    player.setPositionY(336.f);
    player.setMoveSpeed(64.0);
    cout << "Player created" << endl;
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

void Game::run() {
    while (window.isOpen()) {
        float deltaTime = clock.restart().asSeconds();
        processEvents();
        update(deltaTime);
        render();
    }
}

void Game::processEvents() {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed)
            window.close();
        if (event.type == sf::Event::KeyPressed &&
            event.key.code == sf::Keyboard::Escape)
            window.close();

        player.handleInput(event);
    }
}

void Game::update(float deltaTime) {
    player.update(deltaTime);
    handleWarp();

    // Caméra
    sf::View view = window.getDefaultView();
    view.setCenter(
        std::round(player.getPositionX()),
        std::round(player.getPositionY())
    );
    view.zoom(1.0f / zoomFactor);
    window.setView(view);
}

void Game::handleWarp() {
    if (player.hasWarpRequest()) {
        Warp w = player.consumeWarpRequest();
    
        currentMapName = w.targetMap; // "playerHouse"
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

void Game::render() {
    window.clear(sf::Color::Black);
    renderer.render(window, map);
    player.draw(window);
    window.display();
}
