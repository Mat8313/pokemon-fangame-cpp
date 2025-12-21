#include "TileSetManager.hpp"
using namespace std;
using namespace sf;

void TilesetManager::initializeTileCoordinates(){
    const int cols = tilesetTexture.getSize().x / tileSize;
     auto rectAt = [&](int col, int row) {
        return IntRect(col * tileSize, row * tileSize, tileSize, tileSize);
    };
    tileCoordinates[TileType::GRASS] = rectAt(0,6);
    tileCoordinates[TileType::TALL_GRASS] = rectAt(0,7);
    tileCoordinates[TileType::PATH] = rectAt(0,0);
    tileCoordinates[TileType::WATER] = rectAt(6,17);
    tileCoordinates[TileType::SAND] = rectAt(27,0);
    tileCoordinates[TileType::ROAD] = rectAt(24,0);
    tileCoordinates[TileType::WALL] = rectAt(16,19);
    //tileCoordinates[TileType::TREE] = rectAt();
    tileCoordinates[TileType::SMALLTREE] = rectAt(0,8);
    tileCoordinates[TileType::ROCK] = rectAt(16,7);
    tileCoordinates[TileType::LEDGE_UP] = rectAt(4,7);
    tileCoordinates[TileType::LEDGE_LEFT] = rectAt(6,7);
    tileCoordinates[TileType::LEDGE_RIGHT] = rectAt(5,7);
    //tileCoordinates[TileType::DOOR] = rectAt();
    tileCoordinates[TileType::ICE] = rectAt(22,16);
}

TilesetManager::TilesetManager(){}

bool TilesetManager::loadTileset(const std::string& filepath) {
    // Chargement de la texture du megatileset
    if (!tilesetTexture.loadFromFile(filepath)) {
        return false; // échec de chargement
    }

    // Maintenant que la texture est connue, on définit les rectangles
    initializeTileCoordinates();

    return true;
}

IntRect TilesetManager::getTileRect(TileType type)const {
    auto it = tileCoordinates.find(type);
    if (it != tileCoordinates.end()) {
        return it->second;
    }
    // fallback : première tuile (par exemple GRASS)
    return sf::IntRect(0, 0, tileSize, tileSize);
}

const Texture& TilesetManager::getTexture()const {
    return tilesetTexture;
} 

int TilesetManager::getTileSize()const{
    return tileSize;
}
