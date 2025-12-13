#include "LoadMap.hpp"
#include "Tile.hpp"


Type CharToType(char c) {
    switch (c) {
        case 'G': return Type::Grass;
        case 'P': return Type::Path;
        case 'W': return Type::Wall;
        case 'A': return Type::Water;
        default: throw std::runtime_error("Unknown tile type character");
    }
}

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
            map.setTileType(x, y, CharToType(typeChar));
        }
    }
}



