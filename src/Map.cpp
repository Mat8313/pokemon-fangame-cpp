#include "Map.hpp"
using namespace std;

Map::Map(int width, int height, float tileSize)
    : mapWidth(width), mapHeight(height), tileSize(tileSize)
    {
            mapData.resize(height);
    for (int y = 0; y < height; y++) {
        mapData[y].reserve(width);
        for (int x = 0; x < width; x++) {
            float posX = x * tileSize;
            float posY = y * tileSize;
            bool isObstacle = false; // Par défaut
            mapData[y].emplace_back(y, x, isObstacle);
         }
    }
    }

  


int Map::getWidth() const {
    return mapWidth;
}
int Map::getHeight() const {
    return mapHeight;
}
float Map::getTileSize() const {
    return tileSize;
}
void Map::setTileSize(float size) {
    tileSize = size;
}
void Map::setDimensions(int width, int height) {
    mapWidth = width;
    mapHeight = height;
    mapData.resize(height);
    for (int y = 0; y < height; y++) {
        mapData[y].clear();
        mapData[y].reserve(width);
        for (int x = 0; x < width; x++) {
            float posX = x * tileSize;
            float posY = y * tileSize;
            mapData[y].emplace_back(posX, posY, false);
        }
    }
}  

const Tile& Map::getTile(int tileX, int tileY)const{
    return mapData[tileY][tileX];
}
Tile& Map::getTile(int tileX, int tileY) {
    return mapData[tileY][tileX];
}

