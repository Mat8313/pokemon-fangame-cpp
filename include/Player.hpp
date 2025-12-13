#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <string>
#include <SFML/Graphics.hpp>
#include "Character.hpp"
//#include "Party.hpp"
//#include "Bag.hpp"
class Party; //forward declaration 
class Bag;

class Player : public Character{
private : 
    std::string name;
    Party* party; 
    Bag* bag; 
    bool isMoving;
    float targetX, targetY;
    float moveProgress;
public : 
    Player();

    void setParty(Party* party);
    void setBag(Bag* bag); 
    void setName(std::string name);

    std::string getName();
    Party* getParty(); 
    Bag* getBag(); 

    void handleInput(sf::Event& event);
    void update() override;
    
};
#endif 
