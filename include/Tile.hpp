#ifndef TILE_HPP
#define TILE_HPP

#include "TileTypeUtils.hpp"

#include <SFML/Graphics.hpp>


class Tile{
private : 
    float positionX, positionY; 
    TileType type;
    //mutable sf::RectangleShape shape; old version 
    mutable sf::Sprite sprite;
    bool isObstacle;

public : 
    Tile();
    Tile(float x, float y, TileType type, bool obstacle); 

    void draw(sf::RenderWindow& window)const;

    float getPositionX() ;
    float getPositionY() ; 
    TileType getType() const;
    //sf::RectangleShape getShape(); // old version 

    void setPositionX(float x);
    void setPositionY(float y);
    void setType(TileType type);
    //void setShape(const sf::RectangleShape& shape); //old version 
    bool getIsObstacle()const;
    void setIsObstacle(bool o);
    void setTexture(const sf::Texture& texture, const sf::IntRect& rect);


};




#endif 