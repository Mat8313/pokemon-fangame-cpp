#include "TileTypeUtils.hpp"
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
