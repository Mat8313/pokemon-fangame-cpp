#include "MapRenderer.hpp"
using namespace sf;


void MapRenderer::render(RenderWindow& window, const Map& map){
    for (int y=0; y<map.getHeight(); y++){
        for (int x=0; x<map.getWidth() ;x++){
            float posX = x * map.getTileSize();
            float posY = y * map.getTileSize();
            Type type = map.getTileType(x, y);
            Tile tile(posX, posY, type, false);
            if (tile.getType() == Type::Wall || tile.getType() == Type::Water){
                tile.setIsObstacle(true);
            }
            tile.draw(window);
        }
    }
}