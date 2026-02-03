#ifndef TILE_HPP
#define TILE_HPP


#include <SFML/Graphics.hpp>


class Tile{
private : 
    int positionX, positionY; 
    //mutable sf::RectangleShape shape; old version 
    mutable sf::Sprite sprite;
    bool isObstacle;
    int gid; 

public : 
    Tile();
    Tile(int x, int y, bool obstacle); 

    void draw(sf::RenderWindow& window)const;

    int getPositionX() ;
    int getPositionY() ; 
    //sf::RectangleShape getShape(); // old version 

    void setPositionX(int x);
    void setPositionY(int y);
    //void setShape(const sf::RectangleShape& shape); //old version 
    bool getIsObstacle()const;
    void setIsObstacle(bool o);
    void setTexture(const sf::Texture& texture, const sf::IntRect& rect);

    void setGid(int g);
    int getGid() const;

};




#endif 