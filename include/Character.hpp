#ifndef CHARACTER_HPP
#define CHARACTER_HPP

#include <string>


class Character{
private :  
    Direction direction;
    double moveSpeed;  
public : 
    Direction getDirection(); 
    double getMoveSpeed(); 

    void setDirection(std::string direction);    
    void setMoveSpeed(double speed);        
};


enum class Direction {
    North, 
    South, 
    West,  
    East   
};

#endif 