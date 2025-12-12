#ifndef ENTITY_HPP
#define ENTITY_HPP

#include <SFML/Graphics.hpp>


class Entity{
private: 
    float positionX, positionY;
    sf::Sprite sprite;  
public : 
    Entity();
    Entity(float , float y, const sf::Texture texture);

    virtual void update()=0; 

    virtual void draw(sf::RenderWindow& window);

    float getPositionX();
    float getPositionY(); 
    
    sf::Sprite getSprite(); 

    void setPositionX(float x);
    void setPositionY(float y); 
    
    void setSprite(sf::Sprite sprite);

};

#endif 