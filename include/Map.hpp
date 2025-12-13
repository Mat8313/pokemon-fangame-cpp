#ifndef MAP_HPP
#define MAP_HPP

#include "Tile.hpp"
#include <vector>

class Map {
private :
    int mapWidth, mapHeight;
    float tileSize;
    std::vector<std::vector<Type>> mapData; 
public : 
    Map(int width, int height, float tileSize);
    
    Type getTileType(int x, int y) const;
    void setTileType(int x, int y, Type type);
    int getWidth() const;
    int getHeight() const;
    float getTileSize() const;
    void setTileSize(float size);
    void setDimensions(int width, int height);

};

#endif 