#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <string>
#include <SFML/Graphics.hpp>
#include "Character.hpp"
#include "Tile.hpp"
#include "Map.hpp"
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
    Map* currentMap; // référence à la map actuel. 
    
    // Animation
    int currentFrame;           // Frame actuelle (0, 1, 2...)
    float animationTime;        // Temps écoulé pour changer de frame
    const float frameTime = 0.15f;  // Durée d'une frame en secondes
    const int framesPerDirection = 3;  // Nombre de frames par direction
    const int spriteWidth = 32;        // Largeur d'une frame
    const int spriteHeight = 32;       // Hauteur d'une frame

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
    bool canMove(float newX, float newY);
    void setMap(Map* map); 

    void updateSpriteRect();
    
};
#endif 
