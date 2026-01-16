#ifndef ENTITY_HPP
#define ENTITY_HPP

#include <SFML/Graphics.hpp>
#include "Tile.hpp"


class Entity{
private: 
    float positionX, positionY;
    int positionTileX, positionTileY;
    sf::Sprite sprite;  
public : 
    Entity();
    Entity(float , float y, const sf::Texture texture);

    virtual void update(float deltaTime); 

    virtual void draw(sf::RenderWindow& window);

    float getPositionX();
    float getPositionY(); 

    int getPositionTileX();
    int getPositionTileY();
    
    sf::Sprite getSprite(); 
    sf::Sprite& getSpriteRef();

    void setPositionX(float x);
    void setPositionY(float y); 
    
    void setPositionTileX(int x);
    void setPositionTileY(int y);

    void setSprite(sf::Sprite sprite);

};

#endif 