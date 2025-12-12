#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <string>
#include "Character.hpp"
#include "Party.hpp"
#include "Bag.hpp"

class Player : public Character{
private : 
    std::string name;
    Party* party; 
    Bag* bag; 
public : 
    void setParty(Party* party);
    void setBag(Bag* bag); 
    void setName(std::string name);

    std::string getName();
    Party* getParty(); 
    Bag* getBag(); 
    
};
#endif 
