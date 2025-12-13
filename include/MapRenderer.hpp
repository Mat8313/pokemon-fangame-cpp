#ifndef MAPRENDERER_HPP
#define MAPRENDERER_HPP

#include <SFML/Graphics.hpp>
#include "Map.hpp"



class MapRenderer{ 
public : 
    void render(sf::RenderWindow& window, const Map& map);  
};

#endif 