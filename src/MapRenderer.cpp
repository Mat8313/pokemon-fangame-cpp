#include "MapRenderer.hpp"
using namespace sf;


void MapRenderer::render(sf::RenderWindow& window, const Map& map) {
    // Charge la texture du tileset
    sf::Texture tilesetTexture;
    tilesetTexture.loadFromFile("../assets/sprites/tiles.png");
    
    int tileSize = 16;
    int columns = 28;  // Colonnes dans pokemon-red.tsx
    
    for (int y = 0; y < map.getHeight(); ++y) {
        for (int x = 0; x < map.getWidth(); ++x) {
            const Tile& tile = map.getTile(x, y);
            int gid = tile.getGid();
            
            if (gid == 0) continue;  // Pas de tile
            
            // ✅ CALCUL AUTOMATIQUE de la position dans le tileset
            int localId = gid - 1;  // GID commence à 1, pas 0
            int spacing = 1;
            int margin = 1;
            int tsX = margin + (localId % columns) * (tileSize + spacing);
            int tsY = margin + (localId / columns) * (tileSize + spacing);
            
            sf::Sprite sprite;
            sprite.setTexture(tilesetTexture);
            sprite.setTextureRect(sf::IntRect(tsX, tsY, tileSize, tileSize));
            sprite.setPosition(x * tileSize, y * tileSize);
            
            window.draw(sprite);
        }
    }
}

