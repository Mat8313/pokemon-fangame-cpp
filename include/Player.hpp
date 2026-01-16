#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <string>
#include <iostream>
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
    int currentFrame;
    float animationTime;
    sf::Clock animationClock;  // Pour un deltaTime précis
    sf::Texture playerTexture;  // Texture du sprite sheet
    const float frameTime = 0.30f;
    const int framesPerDirection = 2;
    const int spriteWidth = 32;
    const int spriteHeight = 32;
    bool leftFootNext; 

    Direction nextDirection;
    bool hasNextMove;
public : 
    Player();

    void setParty(Party* party);
    void setBag(Bag* bag); 
    void setName(std::string name);

    void loadPlayerTexture();

    std::string getName();
    Party* getParty(); 
    Bag* getBag(); 

    void handleInput(sf::Event& event);
    void update(float detlaTime) override;
    bool canMove(float newX, float newY);
    void setMap(Map* map); 

    void updateSpriteRect();
    void startMovement(Direction dir);
    
};
#endif 
