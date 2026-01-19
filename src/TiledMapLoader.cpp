#include "TiledMapLoader.hpp"
#include <fstream>
#include <sstream>
#include <iostream>
#include <filesystem>

void TiledMapLoader::loadFromTiledTmx(const std::string& filepath, Map& map) {
    std::ifstream file(filepath);
    if (!file.is_open()) {
        throw std::runtime_error("Cannot open Tiled TMX file: " + filepath);
    }

    std::string content((std::istreambuf_iterator<char>(file)),
                        std::istreambuf_iterator<char>());
    file.close();

    // Attributs de la map
    int width      = extractIntAttribute(content, "map", "width");
    int height     = extractIntAttribute(content, "map", "height");
    int tilewidth  = extractIntAttribute(content, "map", "tilewidth");
    int tileheight = extractIntAttribute(content, "map", "tileheight");

    std::cout << "Loading TMX map: " << width << "x" << height << std::endl;

    map.setDimensions(width, height);
    map.setTileSize(static_cast<int>(tilewidth));

    // Tilesets (adapter les chemins / firstgid à ton TMX actuel)
    std::map<int, bool> obstacleMap;
    std::vector<std::pair<int, std::string>> tilesets = {
        {1,    "../assets/map/sprites/pokemon-red.tsx"},
        {6213, "../assets/map/sprites/test.tsx"}
    };

    for (const auto& [firstgid, tsxPath] : tilesets) {
        std::cout << "Loading tileset: " << tsxPath
                  << " (firstgid=" << firstgid << ")" << std::endl;

        std::map<int, bool> localObstacles = loadTilesetObstacles(tsxPath);
        for (const auto& [localId, isObstacle] : localObstacles) {
            int gid = firstgid + localId;
            obstacleMap[gid] = isObstacle;
        }

        std::cout << "Loaded " << localObstacles.size()
                  << " tile properties" << std::endl;
    }

    // Parse des layers (chacun devient une MapLayer dans Map)
    size_t layerPos = 0;
    int layerCount = 0;

    while ((layerPos = content.find("<layer", layerPos)) != std::string::npos) {
        size_t dataStart = content.find("<data", layerPos);
        if (dataStart == std::string::npos) break;
        dataStart = content.find(">", dataStart);
        if (dataStart == std::string::npos) break;
        dataStart += 1;

        size_t dataEnd = content.find("</data>", dataStart);
        if (dataEnd == std::string::npos) break;

        std::string csvData = content.substr(dataStart, dataEnd - dataStart);
        std::vector<int> tileData = parseCSV(csvData);

        // On s'assure que la taille correspond
        if ((int)tileData.size() < width * height) {
            tileData.resize(width * height, 0);
        }

        // Ajoute la layer au Map
        map.addLayer(tileData);

        // Remplit en plus les infos d'obstacles sur la « couche logique » Tile
        for (int y = 0; y < height; ++y) {
            for (int x = 0; x < width; ++x) {
                int idx = y * width + x;
                int gid = tileData[idx];
                if (gid == 0) continue;

                Tile& tile = map.getTile(x, y);
                // garde le plus haut GID pour la logique si tu veux
                tile.setGid(gid);

                auto it = obstacleMap.find(gid);
                if (it != obstacleMap.end()) {
                    tile.setIsObstacle(it->second);
                }
            }
        }

        layerCount++;
        layerPos = dataEnd;
    }
    std::cout << "Map layers in Map: " << map.getLayerCount() << std::endl;

    std::cout << "Loaded " << layerCount << " layers successfully!" << std::endl;
}





std::map<int, bool> TiledMapLoader::loadTilesetObstacles(const std::string& tsxPath) {
    std::map<int, bool> obstacles;
    
    std::ifstream file(tsxPath);
    if (!file.is_open()) {
        std::cerr << "Warning: Cannot open tileset file: " << tsxPath << std::endl;
        return obstacles;
    }
    
    std::string content((std::istreambuf_iterator<char>(file)),
                        std::istreambuf_iterator<char>());
    file.close();
    
    // Parse chaque <tile id="X">
    size_t tilePos = 0;
    
    while ((tilePos = content.find("<tile id=\"", tilePos)) != std::string::npos) {
        // Extrait l'ID de la tile
        size_t idStart = tilePos + 10;
        size_t idEnd = content.find("\"", idStart);
        int tileId = std::stoi(content.substr(idStart, idEnd - idStart));
        
        // Trouve la fin de cette tile
        size_t tileEndPos = content.find("</tile>", tilePos);
        size_t searchEnd = (tileEndPos != std::string::npos) ? tileEndPos : content.size();
        
        // Extrait le contenu de cette tile
        std::string tileContent = content.substr(tilePos, searchEnd - tilePos + 7);
        
        // Cherche isObstacle
        size_t propStart = tileContent.find("name=\"isObstacle\"");
        
        if (propStart != std::string::npos) {
            // Trouve value=" après isObstacle
            size_t valueStart = tileContent.find("value=\"", propStart);
            
            if (valueStart != std::string::npos) {
                valueStart += 7;
                size_t valueEnd = tileContent.find("\"", valueStart);
                std::string value = tileContent.substr(valueStart, valueEnd - valueStart);
                
                obstacles[tileId] = (value == "true");
            }
        }
        
        tilePos = searchEnd;
    }
    
    return obstacles;
}




int TiledMapLoader::extractIntAttribute(const std::string& xml, const std::string& tag, const std::string& attr) {
    size_t tagPos = xml.find("<" + tag);
    if (tagPos == std::string::npos) return 0;
    
    size_t attrPos = xml.find(attr + "=\"", tagPos);
    if (attrPos == std::string::npos) return 0;
    
    attrPos += attr.length() + 2;
    size_t endPos = xml.find("\"", attrPos);
    
    std::string valueStr = xml.substr(attrPos, endPos - attrPos);
    return std::stoi(valueStr);
}

std::string TiledMapLoader::extractStringAttribute(const std::string& xml, const std::string& tag, const std::string& attr) {
    size_t tagPos = xml.find("<" + tag);
    if (tagPos == std::string::npos) return "";
    
    size_t attrPos = xml.find(attr + "=\"", tagPos);
    if (attrPos == std::string::npos) return "";
    
    attrPos += attr.length() + 2;
    size_t endPos = xml.find("\"", attrPos);
    
    return xml.substr(attrPos, endPos - attrPos);
}

std::vector<int> TiledMapLoader::parseCSV(const std::string& csv) {
    std::vector<int> result;
    std::stringstream ss(csv);
    std::string token;
    
    while (std::getline(ss, token, ',')) {
        token.erase(0, token.find_first_not_of(" \n\r\t"));
        token.erase(token.find_last_not_of(" \n\r\t") + 1);
        
        if (!token.empty()) {
            result.push_back(std::stoi(token));
        }
    }
    
    return result;
}

