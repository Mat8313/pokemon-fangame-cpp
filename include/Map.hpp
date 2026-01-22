#ifndef MAP_HPP
#define MAP_HPP

#include "Tile.hpp"
#include <string>
#include <vector>

class MapLayer {
public:
    std::vector<int> gids; 
};

struct Warp {
    int fromX;
    int fromY;
    std::string targetMap;
    int targetX;
    int targetY;
    std::string dir;
};

struct TilesetInfo {
    int firstgid;
    int columns;
    int tileWidth;
    int tileHeight;
    int margin;
    int spacing;
    sf::Texture texture;
};

class Map {
private :
    int mapWidth, mapHeight;
    float tileSize;
    std::vector<std::vector<Tile>> mapData; 
    std::vector<MapLayer> layers;
    std::vector<Tile> tiles;

    std::vector<Warp> m_warps;

    std::vector<TilesetInfo> tilesets;

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

    void addWarp(const Warp& w);

    const Warp* getWarpAt(int x, int y) const;

    void clearWarps();

    void clearTilesets() ;
    void addTileset(const TilesetInfo& ts);
    const std::vector<TilesetInfo>& getTilesets() const;


};

#endif 