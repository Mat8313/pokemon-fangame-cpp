#include "MapRenderer.hpp"
using namespace sf;

#include "MapRenderer.hpp"
using namespace sf;

 void MapRenderer::render(sf::RenderWindow& window, const Map& map) {
    // Chargement textures
    if (!tilesetTexture1Loaded) {
        tilesetTexture1.loadFromFile("../assets/sprites/tiles.png");
        tilesetTexture1.setSmooth(false);
        tilesetTexture1Loaded = true;
    }

    if (!tilesetTexture2Loaded) {
        tilesetTexture2.loadFromFile("../assets/sprites/test.png");
        tilesetTexture2.setSmooth(false);
        tilesetTexture2Loaded = true;
    }

    int tileSize = map.getTileSize();

    // Tileset 1 (pokemon-red)
    int columns1 = 28;
    int spacing1 = 1;
    int margin1  = 1;
    int firstgid1 = 1;

    // Tileset 2 (test.tsx) -> adapter au firstgid réel du TMX
    int columns2 = 68;
    int spacing2 = 0;
    int margin2  = 0;
    int firstgid2 = 1317;

    int width  = map.getWidth();
    int height = map.getHeight();

    // Dessine layer par layer, en ordre
    for (int layerIndex = 0; layerIndex < map.getLayerCount(); ++layerIndex) {
        const MapLayer& layer = map.getLayer(layerIndex);

        for (int y = 0; y < height; ++y) {
            for (int x = 0; x < width; ++x) {
                int idx = y * width + x;
                int gid = layer.gids[idx];
                if (gid == 0) continue; // transparent

                sf::Sprite sprite;
                sprite.setPosition(static_cast<float>(x * tileSize),
                                   static_cast<float>(y * tileSize));

                // Tileset 1
                if (gid >= firstgid1 && gid < firstgid2) {
                    int localId = gid - firstgid1;
                    int tsX = margin1 + (localId % columns1) * (tileSize + spacing1);
                    int tsY = margin1 + (localId / columns1) * (tileSize + spacing1);
                    sprite.setTexture(tilesetTexture1);
                    sprite.setTextureRect(sf::IntRect(tsX, tsY, tileSize, tileSize));
                }
                // Tileset 2
                else if (gid >= firstgid2) {
                    int localId = gid - firstgid2;
                    int tsX = margin2 + (localId % columns2) * (tileSize + spacing2);
                    int tsY = margin2 + (localId / columns2) * (tileSize + spacing2);
                    sprite.setTexture(tilesetTexture2);
                    sprite.setTextureRect(sf::IntRect(tsX, tsY, tileSize, tileSize));
                }

                window.draw(sprite);
            }
        }
    }
}