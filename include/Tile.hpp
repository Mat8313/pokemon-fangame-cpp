#ifndef TILE_HPP
#define TILE_HPP

#include "TileTypeUtils.hpp"

#include <SFML/Graphics.hpp>


class Tile{
private : 
    int positionX, positionY; 
    TileType type;
    //mutable sf::RectangleShape shape; old version 
    mutable sf::Sprite sprite;
    bool isObstacle;
    int gid; 

public : 
    Tile();
    Tile(int x, int y, TileType type, bool obstacle); 

    void draw(sf::RenderWindow& window)const;

    int getPositionX() ;
    int getPositionY() ; 
    TileType getType() const;
    //sf::RectangleShape getShape(); // old version 

    void setPositionX(int x);
    void setPositionY(int y);
    void setType(TileType type);
    //void setShape(const sf::RectangleShape& shape); //old version 
    bool getIsObstacle()const;
    void setIsObstacle(bool o);
    void setTexture(const sf::Texture& texture, const sf::IntRect& rect);

    void setGid(int g);
    int getGid() const;

};




#endif 