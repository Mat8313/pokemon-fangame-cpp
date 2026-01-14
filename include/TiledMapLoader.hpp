#ifndef TILEDMAPLOADER_HPP
#define TILEDMAPLOADER_HPP

#include "Map.hpp"
#include <string>
#include <vector>
#include <map>

class TiledMapLoader {
public:
    // Charge une map Tiled TMX (XML) dans ton objet Map
    void loadFromTiledTmx(const std::string& filepath, Map& map);
    
private:
    // Parse le tileset externe .tsx
    std::map<int, bool> loadTilesetObstacles(const std::string& tsxPath);
    
    // Helpers pour parser le XML
    int extractIntAttribute(const std::string& xml, const std::string& tag, const std::string& attr);
    
    std::string extractStringAttribute(const std::string& xml, const std::string& tag, const std::string& attr);
    
    std::vector<int> parseCSV(const std::string& csv);
     
};

#endif
