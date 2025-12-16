#include "Player.hpp"

using namespace std;
using namespace sf;

Player::Player() 
    : Character(), name("Player"), party(nullptr), bag(nullptr), 
      isMoving(false), targetX(0.0f), targetY(0.0f), moveProgress(0.0f) 
{}
void Player::setParty(Party* party) {
    this->party = party;
}
void Player::setBag(Bag* bag) {
    this->bag = bag;
}
void Player::setName(string name) {
    this->name = name;
}
string Player::getName() {
    return this->name;
}
Party* Player::getParty() {
    return this->party;
}
Bag* Player::getBag() {
    return this->bag;
}

void Player::handleInput(sf::Event& event) {
    if (event.type == sf::Event::KeyPressed && !isMoving) {
        float currentX = getPositionX();
        float currentY = getPositionY();
        float tileSize = 32.0f; // Taille d'une tile
        
        isMoving = true;
        moveProgress = 0.0f;
        
        switch (event.key.code) {
            case sf::Keyboard::Up:
                targetX = currentX;
                targetY = currentY - tileSize;
                setDirection(Direction::North);
                break;
            case sf::Keyboard::Z:
                targetX = currentX;
                targetY = currentY - tileSize;
                setDirection(Direction::North);
                break;
            case sf::Keyboard::Down:
                targetX = currentX;
                targetY = currentY + tileSize;
                setDirection(Direction::South);
                break;
            case sf::Keyboard::S:
                targetX = currentX;
                targetY = currentY + tileSize;
                setDirection(Direction::South);
                break;
            case sf::Keyboard::Left:
                targetX = currentX - tileSize;
                targetY = currentY;
                setDirection(Direction::West);
                break;
            case sf::Keyboard::Q:
                targetX = currentX - tileSize;
                targetY = currentY;
                setDirection(Direction::West);
                break;
            case sf::Keyboard::Right:
                targetX = currentX + tileSize;
                targetY = currentY;
                setDirection(Direction::East);
                break;
            case sf::Keyboard::D:
                targetX = currentX + tileSize;
                targetY = currentY;
                setDirection(Direction::East);
                break;
        }
    }
}

void Player::update() {
    if (isMoving) {
        moveProgress += getMoveSpeed() / 32.0f; // Vitesse normalisée
        
        if (moveProgress >= 1.0f) {
            // Déplacement terminé 
            setPositionX(targetX);
            setPositionY(targetY);
            isMoving = false;
            moveProgress = 0.0f;
        }
        else if (moveProgress < 1.0f) {
            // Interpolation linéaire entre position actuelle et cible
                float startX = targetX - (targetX - getPositionX());
                float startY = targetY - (targetY - getPositionY());
            
                setPositionX(startX + (targetX - startX) * moveProgress);
                setPositionY(startY + (targetY - startY) * moveProgress);
            }
    }
}