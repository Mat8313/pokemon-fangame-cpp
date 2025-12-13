#include "Entity.hpp"

Entity::Entity() : positionX(0), positionY(0), sprite() {}
Entity::Entity(float x, float y, const sf::Texture texture) 
    : positionX(x), positionY(y), sprite(texture) {}

void Entity::draw(sf::RenderWindow& window) {
    sprite.setPosition(positionX, positionY);
    window.draw(sprite);
}
float Entity::getPositionX() {
    return positionX;
}
float Entity::getPositionY() {
    return positionY;
}
int Entity::getPositionTileX() {
    return positionTileX;
}
int Entity::getPositionTileY() {
    return positionTileY;   
}
sf::Sprite Entity::getSprite() {
    return sprite;
}

void Entity::setPositionX(float x) {
    positionX = x;
}
void Entity::setPositionY(float y) {
    positionY = y;
}
void Entity::setPositionTileX(int x) {
    positionTileX = x;
}
void Entity::setPositionTileY(int y) {
    positionTileY = y;
}
void Entity::setSprite(sf::Sprite spr) {
    sprite = spr;
}
