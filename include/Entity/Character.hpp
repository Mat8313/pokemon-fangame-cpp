#pragma once 

#include <string>
#include "Entity.hpp"

enum class Direction {
    North, 
    South, 
    West,  
    East   
};

class Character : public Entity {
private :
    Direction direction;
    double moveSpeed;  
public : 
    Character();

    Direction getDirection(); 
    double getMoveSpeed(); 

    void setDirection(Direction direction);    
    void setMoveSpeed(double speed);        
};