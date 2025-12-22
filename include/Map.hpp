#ifndef MAP_HPP
#define MAP_HPP

#include "TileTypeUtils.hpp"
#include "Tile.hpp"
#include "TileSetManager.hpp"
#include <vector>

class Map {
private :
    int mapWidth, mapHeight;
    float tileSize;
    std::vector<std::vector<Tile>> mapData; 
    TilesetManager tilesetManager;
public : 
    Map(int width, int height, float tileSize);

    bool init();
    
    TileType getTileType(int x, int y) ;
    void setTileType(int x, int y, TileType type);

    int getWidth() const;
    int getHeight() const;

    float getTileSize() const;
    void setTileSize(float size);

    const Tile& getTile(int tileX, int tileY)const; // modif interdite 
    Tile& getTile(int tileX, int tileY);            // modif autorisée 

    void setDimensions(int width, int height);

};

#endif 