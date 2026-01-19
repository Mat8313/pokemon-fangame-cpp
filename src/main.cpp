#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include "Tile.hpp"
#include "Map.hpp"
#include "TiledMapLoader.hpp"  
#include "MapRenderer.hpp"
#include "Player.hpp"
#include <cmath>

using namespace std;

int main() {
    // Création de la fenêtre
    int windowWidth = 800;   
    int windowHeight = 576;  
    float zoomFactor = 2.0f; // Zoom ×2 (ou 3.0f pour ×3)
    sf::RenderWindow window(sf::VideoMode(windowWidth, windowHeight), "Pokemon Fangame");

    window.setFramerateLimit(60);
    cout << "Fenetre SFML creee avec succes !" << endl;
    
    // Chargement de la map
    Map map(0, 0, 16.f);
    TiledMapLoader tiledLoader;
    try {
        tiledLoader.loadFromTiledTmx("../assets/map/Bourg-Palette.tmx", map);
        cout << "Map loaded: " << map.getWidth() << "x" << map.getHeight() << endl;
    } catch (const std::exception& e) {
        cerr << "Error loading map: " << e.what() << endl;
        return -1;
    }
    
    // Création du joueur
    Player player;
    player.setMap(&map);
    player.setPositionX(345.f);
    player.setPositionY(336.f);
    player.setMoveSpeed(64.0);  // 64 pixels par seconde
    cout << "Player created" << endl;
    
    sf::Clock clock;  // ← AJOUTE ÇA
    
    // Boucle de jeu 
    while (window.isOpen()) {
        float deltaTime = clock.restart().asSeconds();  // ← AJOUTE ÇA
        
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
            
            if (event.type == sf::Event::KeyPressed && 
                event.key.code == sf::Keyboard::Escape)
                window.close();
            
            player.handleInput(event);
        }
        
        sf::View view = window.getDefaultView();
        view.setCenter(round(player.getPositionX()), 
                      round(player.getPositionY()));
        view.zoom(1.0f / zoomFactor);
        window.setView(view);
        
        player.update(deltaTime);  // ← PASSE deltaTime
        
        window.clear(sf::Color::Black);
        
        MapRenderer renderer;
        renderer.render(window, map);
        
        player.draw(window);
        
        window.display();
    }
    
    cout << "Fenetre fermee." << endl;
    return 0;
}

