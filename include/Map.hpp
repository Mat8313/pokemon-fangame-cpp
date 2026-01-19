#ifndef MAP_HPP
#define MAP_HPP

#include "Tile.hpp"
#include <string>
#include <vector>

class MapLayer {
public:
    std::vector<int> gids; 
};

class Map {
private :
    int mapWidth, mapHeight;
    float tileSize;
    std::vector<std::vector<Tile>> mapData; 
    std::vector<MapLayer> layers;
    std::vector<Tile> tiles;
public : 
    Map(int width, int height, float tileSize);

    
    int getWidth() const;
    int getHeight() const;

    float getTileSize() const;
    void setTileSize(float size);


    //void initTiles();
    const Tile& getTile(int tileX, int tileY)const; // modif interdite 
    Tile& getTile(int tileX, int tileY);            // modif autorisée 

    void setDimensions(int width, int height);

    void addLayer(const std::vector<int>& data);
    int getLayerCount() const;
    const MapLayer& getLayer(int index) const;



};

#endif 