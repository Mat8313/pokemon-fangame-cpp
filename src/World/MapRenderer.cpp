#include "World/MapRenderer.hpp"
#include <iostream>

using namespace sf;

void MapRenderer::render(sf::RenderWindow& window, const Map& map) {
    int tileSize = map.getTileSize();
    int width    = map.getWidth();
    int height   = map.getHeight();

    const auto& tilesets = map.getTilesets();
    if (tilesets.empty()) return;

    // on suppose tous les tilesets en 16x16 (comme tileSize),
    // sinon il faudrait gérer les écarts.

    for (int layerIndex = 0; layerIndex < map.getLayerCount(); ++layerIndex) {
        const MapLayer& layer = map.getLayer(layerIndex);

        for (int y = 0; y < height; ++y) {
            for (int x = 0; x < width; ++x) {
                int idx = y * width + x;
                int gid = layer.gids[idx];
                if (gid == 0) continue;

                // Trouver le tileset avec plus grand firstgid <= gid
                const TilesetInfo* usedTs = nullptr;
                for (const auto& ts : tilesets) {
                    if (ts.firstgid <= gid) {
                        if (!usedTs || ts.firstgid > usedTs->firstgid)
                            usedTs = &ts;
                    }
                }
                if (!usedTs) continue;

                int localId = gid - usedTs->firstgid;
                int cols    = usedTs->columns;
                int margin  = usedTs->margin;
                int spacing = usedTs->spacing;
                int tw      = usedTs->tileWidth;
                int th      = usedTs->tileHeight;

                int tsX = margin + (localId % cols) * (tw + spacing);
                int tsY = margin + (localId / cols) * (th + spacing);

                Sprite sprite;
                sprite.setTexture(usedTs->texture);
                sprite.setTextureRect(IntRect(tsX, tsY, tw, th));
                sprite.setPosition(static_cast<float>(x * tileSize),
                                   static_cast<float>(y * tileSize));

                window.draw(sprite);
            }
        }
    }
}
