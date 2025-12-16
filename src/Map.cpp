#include "Map.hpp"

Map::Map(int width, int height, float tileSize)
    : mapWidth(width), mapHeight(height), tileSize(tileSize),
        mapData(height, std::vector<Type>(width, Type::Grass)) {}

Type Map::getTileType(int x, int y) const {
    return mapData[y][x];
}   
void Map::setTileType(int x, int y, Type type) {
    mapData[y][x] = type;
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
    for (auto& row : mapData) {
        row.resize(width, Type::Grass);
    }
}   

