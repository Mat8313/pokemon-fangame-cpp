#include "World/Tile.hpp"
#include <SFML/Graphics.hpp>
#include <iostream>
using namespace sf; 

Tile::Tile(int x, int y, bool obstacle) 
    : positionX(x), positionY(y), isObstacle(obstacle) {
    sprite.setPosition(positionX, positionY);
}

int Tile::getPositionX()  {
    return this->positionX; 
}
int Tile::getPositionY()  {
    return this->positionY; 
}
     
void Tile::setPositionX(int x) {
    this->positionX = x; 
}
void Tile::setPositionY(int y) {
    this->positionY = y; 
}


void Tile::setTexture(const sf::Texture& texture, const sf::IntRect& rect) {
    sprite.setTexture(texture);
    sprite.setTextureRect(rect);
}

void Tile::draw(RenderWindow& window) const {
    window.draw(sprite);
}


bool Tile::getIsObstacle()const{
    return isObstacle;
}
void Tile::setIsObstacle(bool o){
    isObstacle = o; 
}

void Tile::setGid(int g){this->gid = g;}

int Tile::getGid()const{return this->gid;}