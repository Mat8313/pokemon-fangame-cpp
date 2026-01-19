#ifndef MAPRENDERER_HPP
#define MAPRENDERER_HPP

#include <SFML/Graphics.hpp>
#include "Map.hpp"



class MapRenderer{ 
private : 
    sf::Texture tilesetTexture;
    bool textureLoaded = false; 
    sf::Texture tilesetTexture1;
    sf::Texture tilesetTexture2;
    bool tilesetTexture1Loaded = false;
    bool tilesetTexture2Loaded = false;
public : 
    void render(sf::RenderWindow& window, const Map& map);  
};

#endif 