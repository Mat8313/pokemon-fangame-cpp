#ifndef MAP_HPP
#define MAP_HPP

#include "Tile.hpp"
#include <string>
#include <vector>

class Map {
private :
    int mapWidth, mapHeight;
    float tileSize;
    std::vector<std::vector<Tile>> mapData; 
public : 
    Map(int width, int height, float tileSize);

    
    int getWidth() const;
    int getHeight() const;

    float getTileSize() const;
    void setTileSize(float size);

    const Tile& getTile(int tileX, int tileY)const; // modif interdite 
    Tile& getTile(int tileX, int tileY);            // modif autorisée 

    void setDimensions(int width, int height);

};

#endif 