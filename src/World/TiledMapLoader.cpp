#include "World/TiledMapLoader.hpp"
#include <fstream>
#include <sstream>
#include <iostream>
#include <filesystem>

void TiledMapLoader::loadFromTiledTmx(const std::string& filepath, Map& map) {
    // --- lecture du fichier TMX entier en mémoire ---
    std::ifstream file(filepath);
    if (!file.is_open()) {
        throw std::runtime_error("Cannot open Tiled TMX file: " + filepath);
    }

    std::string content((std::istreambuf_iterator<char>(file)),
                        std::istreambuf_iterator<char>());
    file.close();

    // --- attributs de la map ---
    int width     = extractIntAttribute(content, "map", "width");
    int height    = extractIntAttribute(content, "map", "height");
    int tilewidth = extractIntAttribute(content, "map", "tilewidth");
    int tileheight= extractIntAttribute(content, "map", "tileheight");

    std::cout << "Loading TMX map: " << width << "x" << height << std::endl;

    map.setDimensions(width, height);
    map.setTileSize(tilewidth); // carré dans ton projet

    // --- tilesets : infos graphiques + collisions ---
    std::map<int,bool> obstacleMap;
    map.clearTilesets();

    size_t tsPos = 0;
    while ((tsPos = content.find("<tileset", tsPos)) != std::string::npos) {
        size_t tsEnd = content.find(">", tsPos);
        if (tsEnd == std::string::npos) break;
        std::string tsTag = content.substr(tsPos, tsEnd - tsPos + 1);

        auto getIntAttr = [&](const std::string& key) -> int {
            std::string k = key + "=\"";
            size_t p = tsTag.find(k);
            if (p == std::string::npos) return 0;
            p += k.size();
            size_t e = tsTag.find("\"", p);
            return std::stoi(tsTag.substr(p, e - p));
        };

        int firstgid = getIntAttr("firstgid");

        // source="sprites/pokemon-red.tsx"
        std::string source;
        {
            std::string k = "source=\"";
            size_t p = tsTag.find(k);
            if (p != std::string::npos) {
                p += k.size();
                size_t e = tsTag.find("\"", p);
                source = tsTag.substr(p, e - p);
            }
        }

        if (firstgid == 0 || source.empty()) {
            tsPos = tsEnd;
            continue;
        }

        std::string tsxPath = "../assets/map/" + source;

        // --- lecture du TSX ---
        std::ifstream tsxFile(tsxPath);
        if (!tsxFile.is_open()) {
            std::cerr << "Warning: cannot open TSX: " << tsxPath << "\n";
            tsPos = tsEnd;
            continue;
        }
        std::string tsxContent((std::istreambuf_iterator<char>(tsxFile)),
                               std::istreambuf_iterator<char>());
        tsxFile.close();

        // <tileset ...> dans le TSX
        size_t rootPos = tsxContent.find("<tileset");
        size_t rootEnd = tsxContent.find(">", rootPos);
        std::string rootTag = tsxContent.substr(rootPos, rootEnd - rootPos + 1);

        auto getIntAttrRoot = [&](const std::string& key) -> int {
            std::string k = key + "=\"";
            size_t p = rootTag.find(k);
            if (p == std::string::npos) return 0;
            p += k.size();
            size_t e = rootTag.find("\"", p);
            return std::stoi(rootTag.substr(p, e - p));
        };

        int tw      = getIntAttrRoot("tilewidth");
        int th      = getIntAttrRoot("tileheight");
        int spacing = getIntAttrRoot("spacing");
        int margin  = getIntAttrRoot("margin");

        // <image ...>
        size_t imgPos = tsxContent.find("<image", rootEnd);
        size_t imgEnd = tsxContent.find(">", imgPos);
        std::string imgTag = tsxContent.substr(imgPos, imgEnd - imgPos + 1);

        auto getStrAttrImg = [&](const std::string& key) -> std::string {
            std::string k = key + "=\"";
            size_t p = imgTag.find(k);
            if (p == std::string::npos) return "";
            p += k.size();
            size_t e = imgTag.find("\"", p);
            return imgTag.substr(p, e - p);
        };

        std::string imageSource = getStrAttrImg("source");

        int imgWidth = 0;
        {
            std::string k = "width=\"";
            size_t p = imgTag.find(k);
            if (p != std::string::npos) {
                p += k.size();
                size_t e = imgTag.find("\"", p);
                imgWidth = std::stoi(imgTag.substr(p, e - p));
            }
        }

        int columns = (tw + spacing) > 0
            ? (imgWidth - 2 * margin + spacing) / (tw + spacing)
            : 0;

        TilesetInfo info;
        info.firstgid   = firstgid;
        info.columns    = columns;
        info.tileWidth  = tw;
        info.tileHeight = th;
        info.margin     = margin;
        info.spacing    = spacing;

        // imageSource vient directement du TSX, ex: "../sprites/tiles.png"
        std::string imagePath;
            
        // Si le chemin est déjà relatif à ../assets (commence par "../")
        if (imageSource.rfind("../", 0) == 0) {
            imagePath = "../assets/map/" + imageSource.substr(3); 
            // -> "../assets/sprites/tiles.png" si imageSource="../sprites/tiles.png"
        } else {
            // Chemin déjà correct par rapport à ../assets/map/
            imagePath = "../assets/map/" + imageSource;
        }
        
        if (!info.texture.loadFromFile(imagePath)) {
            std::cerr << "Warning: cannot load tileset image: " << imagePath << "\n";
        }
        

        map.addTileset(info);

        // --- collisions pour ce tileset ---
        std::map<int,bool> localObstacles = loadTilesetObstacles(tsxPath);
        for (const auto& [localId, isObs] : localObstacles) {
            int gid = firstgid + localId;
            obstacleMap[gid] = isObs;
        }

        std::cout << "Loading tileset: " << tsxPath
                  << " (firstgid=" << firstgid << ")\n";
        std::cout << "Loaded " << localObstacles.size()
                  << " tile properties\n";

        tsPos = tsEnd;
    }

    // --- parsing des layers (CSV) ---
    size_t layerPos  = 0;
    int    layerCount= 0;

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

        if ((int)tileData.size() < width * height) {
            tileData.resize(width * height, 0);
        }

        map.addLayer(tileData);

        // mise à jour des tiles logiques (collision)
        for (int y = 0; y < height; ++y) {
            for (int x = 0; x < width; ++x) {
                int idx = y * width + x;
                int gid = tileData[idx];
                if (gid == 0) continue;

                Tile& tile = map.getTile(x, y);
                tile.setGid(gid);

                auto it = obstacleMap.find(gid);
                if (it != obstacleMap.end() && it->second) {
                    tile.setIsObstacle(true);
                }
            }
        }

        layerCount++;
        layerPos = dataEnd;
    }

    std::cout << "Map layers in Map: " << map.getLayerCount() << std::endl;
    std::cout << "Loaded " << layerCount << " layers successfully!" << std::endl;

    // --- parsing des warps ---
    map.clearWarps();

    size_t ogPos = 0;
    while ((ogPos = content.find("<objectgroup", ogPos)) != std::string::npos) {
        size_t namePos = content.find("name=\"", ogPos);
        if (namePos == std::string::npos) break;
        namePos += 6;
        size_t nameEnd = content.find("\"", namePos);
        std::string ogName = content.substr(namePos, nameEnd - namePos);

        size_t ogEnd = content.find("</objectgroup>", ogPos);
        if (ogEnd == std::string::npos) break;

        if (ogName == "warps") {
            size_t objPos = ogPos;
            while ((objPos = content.find("<object", objPos)) != std::string::npos
                   && objPos < ogEnd) {

                size_t objEnd = content.find(">", objPos);
                if (objEnd == std::string::npos) break;
                std::string objTag = content.substr(objPos, objEnd - objPos + 1);

                auto getFloatAttr = [&](const std::string& attr) -> float {
                    std::string key = attr + "=\"";
                    size_t aPos = objTag.find(key);
                    if (aPos == std::string::npos) return 0.f;
                    aPos += key.size();
                    size_t aEnd = objTag.find("\"", aPos);
                    return std::stof(objTag.substr(aPos, aEnd - aPos));
                };

                float xPix = getFloatAttr("x");
                float yPix = getFloatAttr("y");
                int fromX = static_cast<int>(xPix) / tilewidth;
                int fromY = static_cast<int>(yPix) / tileheight;

                size_t objClose = content.find("</object>", objEnd);
                if (objClose == std::string::npos) break;
                std::string objContent = content.substr(objEnd, objClose - objEnd);

                auto getProp = [&](const std::string& propName) -> std::string {
                    std::string key = "name=\"" + propName + "\"";
                    size_t pPos = objContent.find(key);
                    if (pPos == std::string::npos) return "";
                    size_t vPos = objContent.find("value=\"", pPos);
                    if (vPos == std::string::npos) return "";
                    vPos += 7;
                    size_t vEnd = objContent.find("\"", vPos);
                    return objContent.substr(vPos, vEnd - vPos);
                };

                std::string type       = getProp("Type");
                if (type != "warp") {
                    objPos = objClose;
                    continue;
                }

                std::string targetMap  = getProp("target-map");
                std::string targetXStr = getProp("targetX");
                std::string targetYStr = getProp("targetY");
                std::string dir        = getProp("dir");

                if (!targetMap.empty()) {
                    Warp w;
                    w.fromX     = fromX;
                    w.fromY     = fromY;
                    w.targetMap = targetMap;
                    w.targetX   = targetXStr.empty() ? fromX : std::stoi(targetXStr);
                    w.targetY   = targetYStr.empty() ? fromY : std::stoi(targetYStr);
                    w.dir       = dir;
                    map.addWarp(w);

                    std::cout << "Warp chargé: from(" << w.fromX << "," << w.fromY
                              << ") -> " << w.targetMap
                              << " (" << w.targetX << "," << w.targetY << ")\n";
                }

                objPos = objClose;
            }
        }

        ogPos = ogEnd;
    }
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

