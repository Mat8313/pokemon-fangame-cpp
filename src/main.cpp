#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include "Tile.hpp"

using namespace std;

int main() {
    sf::RenderWindow window(sf::VideoMode(800, 576), "Test SFML - Pokemon Fangame");
    cout << "Fenetre SFML creee avec succes !" << endl;

    const int MAP_WIDTH  = 25;
    const int MAP_HEIGHT = 18;
    const float TILE_SIZE = 32.f;

    // 1) Structure de map logique
    Type mapData[MAP_HEIGHT][MAP_WIDTH];

    // Remplir la map avec des Types
    for (int y = 0; y < MAP_HEIGHT; ++y) {
        for (int x = 0; x < MAP_WIDTH; ++x) {
            Type type = Type::Grass;

            // Chemin horizontal au milieu
            if (y == MAP_HEIGHT / 2)
                type = Type::Path;

            // Eau sur les 3 dernières lignes
            if (y >= MAP_HEIGHT - 3)
                type = Type::Water;

            mapData[y][x] = type;
        }
    }

    // 2) Générer les tiles à partir de mapData
    std::vector<Tile> tiles;
    tiles.reserve(MAP_WIDTH * MAP_HEIGHT);

    for (int y = 0; y < MAP_HEIGHT; ++y) {
        for (int x = 0; x < MAP_WIDTH; ++x) {
            float posX = x * TILE_SIZE;
            float posY = y * TILE_SIZE;
            tiles.emplace_back(posX, posY, mapData[y][x]);
        }
    }

    // 3) Boucle de jeu
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
                window.close();
        }

        window.clear(sf::Color::Black);

        for (auto& tile : tiles) {
            tile.draw(window);
        }

        window.display();
    }

    cout << "Fenetre fermee." << endl;
    return 0;
}
