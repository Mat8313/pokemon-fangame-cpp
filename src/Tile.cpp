#include "Tile.hpp"
#include <SFML/Graphics.hpp>
using namespace sf; 

Tile::Tile(float x, float y, TileType type, bool obstacle) : positionX(x), positionY(y), type(type), isObstacle(obstacle) {
    shape.setSize(Vector2f(32.f,32.f));

    switch(type){ 
        case TileType::GRASS : 
            shape.setFillColor(Color::Green);
            break; 
        case TileType::PATH : 
            shape.setFillColor(Color(150, 75, 0));
            break;
        case TileType::WALL : 
            shape.setFillColor(Color(64, 64, 64));
            break; 
        case TileType::WATER : 
            shape.setFillColor(Color::Cyan);
            break; 
    }
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
RectangleShape Tile::getShape()  {
    return this->shape;
}       
void Tile::setPositionX(float x) {
    this->positionX = x; 
}
void Tile::setPositionY(float y) {
    this->positionY = y; 
}
void Tile::setType(TileType t) {
    this->type = t;
    
    // Mettre à jour la couleur du shape selon le nouveau type
    switch(t) { 
        case TileType::GRASS : 
            shape.setFillColor(Color::Green);
            break; 
        case TileType::PATH : 
            shape.setFillColor(Color(150, 75, 0));
            break;
        case TileType::WALL : 
            shape.setFillColor(Color(64, 64, 64));
            break; 
        case TileType::WATER : 
            shape.setFillColor(Color::Cyan);
            break; 
    }
}
void Tile::setShape(const RectangleShape& s) {
    this->shape = s;
}  

void Tile::draw(RenderWindow& window)const {
    shape.setPosition(positionX, positionY);
    window.draw(shape);
}

bool Tile::getIsObstacle()const{
    return isObstacle;
}
void Tile::setIsObstacle(bool o){
    isObstacle = o; 
}