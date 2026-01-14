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
    
    // Parse les attributs de la map
    int width = extractIntAttribute(content, "map", "width");
    int height = extractIntAttribute(content, "map", "height");
    int tilewidth = extractIntAttribute(content, "map", "tilewidth");
    int tileheight = extractIntAttribute(content, "map", "tileheight");
    
    std::cout << "Loading TMX map: " << width << "x" << height << std::endl;
    
    // Configure la Map
    map.setDimensions(width, height);
    map.setTileSize(static_cast<float>(tilewidth));
    
    // Parse le tileset externe pour récupérer les propriétés isObstacle
    std::map<int, bool> obstacleMap;
    
    size_t tilesetPos = content.find("<tileset");
    if (tilesetPos != std::string::npos) {
        int firstgid = extractIntAttribute(content.substr(tilesetPos), "tileset", "firstgid");
        std::string tsxSource = extractStringAttribute(content.substr(tilesetPos), "tileset", "source");
        
        std::cout << "Found external tileset: " << tsxSource << " (firstgid=" << firstgid << ")" << std::endl;
        
        // Construit le chemin vers le .tsx
        std::filesystem::path mapPath(filepath);
        std::filesystem::path tsxPath = mapPath.parent_path() / tsxSource;
        
        std::cout << "Loading tileset from: " << tsxPath << std::endl;
        
        // Charge les propriétés isObstacle depuis le .tsx
        std::map<int, bool> localObstacles = loadTilesetObstacles(tsxPath.string());
        
        // Ajuste avec le firstgid
        for (const auto& [localId, isObstacle] : localObstacles) {
            int gid = firstgid + localId;
            obstacleMap[gid] = isObstacle;
            if (isObstacle) {
                std::cout << "  GID " << gid << " is obstacle" << std::endl;
            }
        }
    }
    
    // Parse les layers
    size_t layerPos = 0;
    int layerCount = 0;
    
    while ((layerPos = content.find("<layer", layerPos)) != std::string::npos) {
        layerPos++;
        
        size_t nameStart = content.find("name=\"", layerPos) + 6;
        size_t nameEnd = content.find("\"", nameStart);
        std::string layerName = content.substr(nameStart, nameEnd - nameStart);
        
        std::cout << "Processing layer: " << layerName << std::endl;
        
        size_t dataStart = content.find("<data encoding=\"csv\">", layerPos);
        if (dataStart == std::string::npos) continue;
        
        dataStart += 21;
        size_t dataEnd = content.find("</data>", dataStart);
        std::string csvData = content.substr(dataStart, dataEnd - dataStart);
        
        std::vector<int> tileData = parseCSV(csvData);
        
        // Applique les données à la Map
        for (int y = 0; y < height; ++y) {
            for (int x = 0; x < width; ++x) {
                int idx = y * width + x;
                if (idx >= tileData.size()) continue;
                
                int gid = tileData[idx];
                if (gid == 0) continue;
                
                // Récupère la tile actuelle
                Tile& tile = map.getTile(x, y);
                tile.setGid(gid);
                
                // Si c'est le premier layer (Ground) ou si la tile est vide, on définit le type
                if (layerName == "Ground" || tile.getType() == TileType::NONE) {
                    TileType type = gidToTileType(gid);
                    map.setTileType(x, y, type);
                }
                
                // Applique la propriété isObstacle depuis le .tsx
                if (obstacleMap.find(gid) != obstacleMap.end()) {
                    tile.setIsObstacle(obstacleMap[gid]);
                }
            }
        }
        
        layerCount++;
    }
    
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
    
    std::cout << "Parsing tileset properties..." << std::endl;
    
    // Parse chaque <tile id="X">
    size_t tilePos = 0;
    while ((tilePos = content.find("<tile id=\"", tilePos)) != std::string::npos) {
        // Extrait l'ID de la tile
        size_t idStart = tilePos + 10; // longueur de "<tile id=\""
        size_t idEnd = content.find("\"", idStart);
        int tileId = std::stoi(content.substr(idStart, idEnd - idStart));
        
        // Cherche la propriété isObstacle
        size_t propStart = content.find("<property name=\"isObstacle\"", tilePos);
        size_t nextTile = content.find("<tile", tilePos + 1);
        
        // Vérifie que la propriété est dans cette tile
        if (propStart != std::string::npos && (nextTile == std::string::npos || propStart < nextTile)) {
            size_t valueStart = content.find("value=\"", propStart) + 7;
            size_t valueEnd = content.find("\"", valueStart);
            std::string value = content.substr(valueStart, valueEnd - valueStart);
            
            obstacles[tileId] = (value == "true");
        }
        
        tilePos = idEnd;
    }
    
    std::cout << "Found " << obstacles.size() << " tiles with obstacle properties" << std::endl;
    
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

TileType TiledMapLoader::gidToTileType(int gid) {
    // ADAPTATION : Mappe tes GID vers tes TileType
    // Regarde ton enum TileType et ajuste
    
    // Exemples basés sur ton tileset
    if (gid >= 1 && gid <= 10) return TileType::GRASS;
    if (gid >= 206 && gid <= 265) return TileType::WATER;
    if (gid >= 952 && gid <= 1041) return TileType::TREE;
    
    // Par défaut
    return TileType::GRASS;
}
