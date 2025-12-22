#include "Tile.hpp"
#include <SFML/Graphics.hpp>
using namespace sf; 

Tile::Tile(float x, float y, TileType type, bool obstacle) 
    : positionX(x), positionY(y), type(type), isObstacle(obstacle) {
    sprite.setPosition(positionX, positionY);
}

float Tile::getPositionX()  {
    return this->positionX; 
}
float Tile::getPositionY()  {
    return this->positionY; 
}
TileType Tile::getType() const{
    return this->type;
}     
void Tile::setPositionX(float x) {
    this->positionX = x; 
}
void Tile::setPositionY(float y) {
    this->positionY = y; 
}
void Tile::setType(TileType t) {
    this->type = t;
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