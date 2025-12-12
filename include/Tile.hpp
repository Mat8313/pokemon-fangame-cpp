#ifndef TILE_HPP
#define TILE_HPP

#include <SFML/Graphics.hpp>

enum class Type {
    Grass, 
    Path, 
    Wall, 
    Water, 
};

class Tile{
private : 
    float positionX, positionY; 
    Type type;
    sf::RectangleShape shape;

public : 
    Tile();
    Tile(float x, float y, Type type); 

    void draw(sf::RenderWindow& window);

    float getPositionX() ;
    float getPositionY() ; 
    Type getType() ;
    sf::RectangleShape getShape();

    void setPositionX(float x);
    void setPositionY(float y);
    void setType(Type type);
    void setShape(const sf::RectangleShape& shape);


};



#endif 