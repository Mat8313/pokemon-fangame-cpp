#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include "Tile.hpp"
#include "Map.hpp"
#include "TiledMapLoader.hpp"   // ← NOUVEAU : Loader Tiled
#include "MapRenderer.hpp"
#include "Player.hpp"
#include <cmath>

using namespace std;

int main() {
    // 1) Création de la fenêtre
    sf::RenderWindow window(sf::VideoMode(400, 288), "Test SFML - Pokemon Fangame");
    window.setFramerateLimit(60);
    cout << "Fenetre SFML creee avec succes !" << endl;
    
    // 2) Chargement de la map avec Tiled
    Map map(0, 0, 16.f);
    
    
    TiledMapLoader tiledLoader;
    try {
        tiledLoader.loadFromTiledTmx("../assets/map/map-test.tmx", map);
        cout << "Map loaded: " << map.getWidth() << "x" << map.getHeight() << endl;
    } catch (const std::exception& e) {
        cerr << "Error loading map: " << e.what() << endl;
        return -1;
    }

    
    // 3) Création du joueur 
    sf::Texture playerTexture;
    if (!playerTexture.loadFromFile("../assets/sprites/player.png")) {
        cerr << "Error loading player texture!" << endl;
        return -1;
    }
    
    Player player;
    player.setMap(&map);
    player.setPositionX(57.f);
    player.setPositionY(64.f);
    player.setSprite(sf::Sprite(playerTexture));
    player.setMoveSpeed(2.0);
    cout << "Player created at position (" << player.getPositionX() 
         << ", " << player.getPositionY() << ")" << endl;
    
    // 4) Boucle de jeu 
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
            
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
                window.close();
            
            player.handleInput(event);
        }
        
        sf::View view = window.getDefaultView();
        view.setCenter(round(player.getPositionX()), round(player.getPositionY())
);
        window.setView(view);
        
        player.update();
        
        window.clear(sf::Color::Black);
        
        MapRenderer renderer;
        renderer.render(window, map);
        
        player.draw(window);
        
        window.display();
    }
    
    cout << "Fenetre fermee." << endl;
    return 0;
}
