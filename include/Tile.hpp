#ifndef TILE_HPP
#define TILE_HPP

#include "TileTypeUtils.hpp"

#include <SFML/Graphics.hpp>


class Tile{
private : 
    float positionX, positionY; 
    TileType type;
    sf::RectangleShape shape;
    bool isObstacle;

public : 
    Tile();
    Tile(float x, float y, TileType type, bool obstacle); 

    void draw(sf::RenderWindow& window);

    float getPositionX() ;
    float getPositionY() ; 
    TileType getType() const;
    sf::RectangleShape getShape();

    void setPositionX(float x);
    void setPositionY(float y);
    void setType(TileType type);
    void setShape(const sf::RectangleShape& shape);

    bool getIsObstacle()const;
    void setIsObstacle(bool o);


};




#endif 