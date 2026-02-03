#include "Entity/Character.hpp"
using namespace std; 

Character::Character() : Entity(), direction(Direction::South), moveSpeed(1.0) {}

Direction Character::getDirection() {
    return this->direction;
}
double Character::getMoveSpeed() {
    return this->moveSpeed;
}

void Character::setDirection(Direction direction){
    this->direction = direction; 
}
void Character::setMoveSpeed(double speed){
    this->moveSpeed = speed; 
}

