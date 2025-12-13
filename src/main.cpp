#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include "Tile.hpp"
#include "Map.hpp"
#include "LoadMap.hpp" 
#include "MapRenderer.hpp"

using namespace std;

int main() {
    sf::RenderWindow window(sf::VideoMode(800, 576), "Test SFML - Pokemon Fangame");
    cout << "Fenetre SFML creee avec succes !" << endl;

    Map map(0, 0, 32.f);
    LoadMap loader;
    try {
        loader.loadFromFileTxt("../assets/map/map1.txt", map);
        cout << "Map loaded: " << map.getWidth() << "x" << map.getHeight() << endl;
    } catch (const std::exception& e) {
        cerr << "Error loading map: " << e.what() << endl;
        return -1;
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

        
        MapRenderer renderer;
        renderer.render(window, map);

        window.display();
    }

    cout << "Fenetre fermee." << endl;
    return 0;
}
