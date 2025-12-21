#include "LoadMap.hpp"
#include "Tile.hpp"



void LoadMap::loadFromFileTxt(const std::string& filename, Map& map) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Could not open map file: " + filename);
    }

    int width , height;
    file >> width >> height;
    map.setDimensions(width, height);

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            char typeChar;
            file >> typeChar;
            map.setTileType(x, y, TileTypeUtils::charToType(typeChar));
        }
    }
}



