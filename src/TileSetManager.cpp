#include "TileSetManager.hpp"
using namespace std;
using namespace sf;

void TilesetManager::initializeTileCoordinates(){
    const int cols = tilesetTexture.getSize().x / tileSize;
    auto rectAt = [&](int col, int row) {
        int spacing = 1;  // Espacement entre les tuiles
        int x = col * (tileSize + spacing);
        int y = row * (tileSize + spacing);
        return IntRect(x, y, tileSize+1, tileSize+1);
    };
    tileCoordinates[TileType::GRASS] = rectAt(6,0);
    tileCoordinates[TileType::TALL_GRASS] = rectAt(0,7);
    tileCoordinates[TileType::PATH] = rectAt(0,0);
    tileCoordinates[TileType::WATER] = rectAt(11,5);
    tileCoordinates[TileType::SAND] = rectAt(27,0);
    tileCoordinates[TileType::ROAD] = rectAt(24,0);
    tileCoordinates[TileType::WALL] = rectAt(21,19);
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

bool TilesetManager::loadTileset(const std::string& filepath, int tileSize_) {
    tileSize = tileSize_;
    // Chargement de la texture du megatileset
    if (!tilesetTexture.loadFromFile(filepath)) {
        return false; // échec de chargement
    }
    tilesetTexture.setSmooth(false);
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
