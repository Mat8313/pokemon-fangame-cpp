#include "MapRenderer.hpp"
using namespace sf;


void MapRenderer::render(RenderWindow& window, const Map& map) {
    for (int y = 0; y < map.getHeight(); y++) {
        for (int x = 0; x < map.getWidth(); x++) {
            const Tile& tile = map.getTile(x, y); // Pas de copie !
            tile.draw(window);
        }
    }
}
