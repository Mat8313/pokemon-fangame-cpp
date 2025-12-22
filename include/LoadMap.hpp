#ifndef LOADMAP_HPP
#define LOADMAP_HPP

#include <string>
#include <fstream>
#include "Map.hpp"
#include "TileSetManager.hpp"

class LoadMap {
public:
    void loadFromFileTxt(const std::string& filename, Map& map);
};  

#endif 