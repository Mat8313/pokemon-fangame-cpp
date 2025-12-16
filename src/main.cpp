#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include "Tile.hpp"
#include "Map.hpp"
#include "LoadMap.hpp" 
#include "MapRenderer.hpp"
#include "Player.hpp"

using namespace std;

int main() {
    // 1) Création de la fenêtre
    sf::RenderWindow window(sf::VideoMode(800, 576), "Test SFML - Pokemon Fangame");
    window.setFramerateLimit(60); // Limite à 60 FPS
    cout << "Fenetre SFML creee avec succes !" << endl;
    
    // 2) Chargement de la map
    Map map(0, 0, 32.f);
    LoadMap loader;
    try {
        loader.loadFromFileTxt("../assets/map/map1.txt", map);
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
    player.setPositionX(100.f);  // Position initiale X (en pixels)
    player.setPositionY(100.f);  // Position initiale Y (en pixels)
    player.setSprite(sf::Sprite(playerTexture));
    player.setMoveSpeed(2.0);    // Vitesse de déplacement (pixels par frame)
    cout << "Player created at position (" << player.getPositionX() 
         << ", " << player.getPositionY() << ")" << endl;
    
    // 4) Boucle de jeu
    while (window.isOpen()) {
        // A) Gestion des événements
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
                
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
                window.close();
            
            // Gestion des inputs du joueur
            player.handleInput(event);
        }
        
        // B) Update (logique du jeu)
        player.update(); // Met à jour la position du joueur
        
        // C) Rendu (affichage)
        window.clear(sf::Color::Black);
        
        // Affichage de la map
        MapRenderer renderer;
        renderer.render(window, map);
        
        // Affichage du joueur
        player.draw(window);
        
        window.display();
    }
    
    cout << "Fenetre fermee." << endl;
    return 0;
}
