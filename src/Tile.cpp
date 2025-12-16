#include "Tile.hpp"
#include <SFML/Graphics.hpp>
using namespace sf; 

Tile::Tile(float x, float y, Type type, bool obstacle):positionX(x), positionY(y), type(type), isObstacle(obstacle) {
    shape.setSize(Vector2f(32.f,32.f));

    switch(type){ 
        case Type::Grass : 
            shape.setFillColor(Color::Green);
            break; 
        case Type::Path : 
            shape.setFillColor(Color(150, 75, 0));
            break;
        case Type::Wall : 
            shape.setFillColor(Color(64, 64, 64));
            break; 
        case Type::Water : 
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
Type Tile::getType()  {
    return this->type;
}
bool Tile::getIsObstacle() {
    return this->isObstacle;
}
void Tile::setIsObstacle(bool isObs) {
    this->isObstacle = isObs;
}

sf::RectangleShape Tile::getShape()  {
    return this->shape;
}       

void Tile::setPositionX(float x) {
    this->positionX = x; 
}
void Tile::setPositionY(float y) {
    this->positionY = y; 
}
void Tile::setType(Type t) {
    this->type = t;
}
void Tile::setShape(const sf::RectangleShape& s) {
    this->shape = s;
}  

void Tile::draw(sf::RenderWindow& window) {
    shape.setPosition(positionX, positionY);
    window.draw(shape);
}