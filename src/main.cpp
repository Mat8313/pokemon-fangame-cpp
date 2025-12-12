#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include "Tile.hpp"
using namespace std ; 

int main() {
    // Créer une fenêtre
    sf::RenderWindow window(sf::VideoMode(800, 600), "Test SFML - Pokemon Fangame");
    
    cout << "Fenêtre SFML créée avec succès !" << endl;
    
    const int MAP_WIDTH = 25;
    const int MAP_HEIGHT = 18;
    const float TILE_SIZE = 32.f;

    Type mapData[MAP_HEIGHT][MAP_WIDTH] = {
        { Type::Grass, Type::Grass, Type::Path,  Type::Path,  Type::Grass },
        { Type::Grass, Type::Wall,  Type::Wall,  Type::Path,  Type::Grass },
        { Type::Grass, Type::Grass, Type::Path,  Type::Path,  Type::Grass },
        { Type::Water, Type::Water, Type::Water, Type::Grass, Type::Grass },
        { Type::Grass, Type::Grass, Type::Grass, Type::Grass, Type::Grass },
    };

    vector<Tile> tiles;
    tiles.reserve(MAP_WIDTH * MAP_HEIGHT);

    for (int y = 0; y < MAP_HEIGHT; ++y) {
        for (int x = 0; x < MAP_WIDTH; ++x) {
            Type type = Type::Grass;

            // Exemple : faire un chemin horizontal au milieu
            if (y == MAP_HEIGHT / 2)
                type = Type::Path;

            // Exemple : de l'eau en bas
            if (y >= MAP_HEIGHT - 3)
                type = Type::Water;

            float posX = x * TILE_SIZE;
            float posY = y * TILE_SIZE;
            tiles.emplace_back(posX, posY, type);
        }
    }



    // Boucle de jeu
    while (window.isOpen()) {
        // Gérer les événements
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
            
            // Fermer avec ESC
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
                window.close();

        }

        // Effacer l'écran en noir
        window.clear(sf::Color::Black);
        
        
        for (auto& tile : tiles) {
        tile.draw(window);
        }


        // Afficher
        window.display();
    }
    
    cout << "Fenêtre fermée." << endl;
    return 0;
}
