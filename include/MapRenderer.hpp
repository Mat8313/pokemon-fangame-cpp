#ifndef MAPRENDERER_HPP
#define MAPRENDERER_HPP

#include <SFML/Graphics.hpp>
#include "Map.hpp"



class MapRenderer{ 
private : 
    sf::Texture tilesetTexture;
    bool textureLoaded = false; 
public : 
    void render(sf::RenderWindow& window, const Map& map);  
};

#endif 