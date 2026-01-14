#include "TileTypeUtils.hpp"
#include <iostream>
using namespace std; 

bool TileTypeUtils::isObstacle(TileType type){
    switch (type) {
        case TileType::WATER : 
        case TileType::WALL : 
        case TileType::ROCK : 
        case TileType::SMALLTREE :
        case TileType::TREE : 
            return true;
        default : 
            return false; 
    }
}

bool TileTypeUtils::requiresHM(TileType type){
    switch(type){
        case TileType::WATER :
        case TileType::ROCK :
        case TileType::SMALLTREE :
            return true;
        default : 
            return false;
    }
}

string TileTypeUtils::getRequiredHM(TileType type){
    switch(type){
        case TileType::SMALLTREE :
            return "cut"; 
        case TileType::ROCK : 
            return "rock smash";
        case TileType::WATER : 
            return "surf";
    }
}

bool TileTypeUtils::triggersBattle(TileType type){
    //fonction à coder 
}
float TileTypeUtils::getSpeedModifier(TileType type){
    //fonction à coder
}
bool TileTypeUtils::canPassDirection(TileType type, int dirX, int dirY) {
    // Pour l'instant, autorise toutes les directions
    return true;
}

string TileTypeUtils::toString(TileType type){
    //fonction à coder 
}
TileType TileTypeUtils::charToType(char c){
switch (c) {
    case 'G': return TileType::GRASS;
    case 'P': return TileType::PATH;
    case 'W': return TileType::WALL;
    case 'A': return TileType::WATER;
    default: return TileType::NONE;
}

}


