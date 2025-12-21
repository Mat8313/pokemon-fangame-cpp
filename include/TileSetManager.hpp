#ifndef TILESETMANAGER_HPP
#define TILESETMANAGER_HPP

#include <SFML/Graphics.hpp>
#include <map>
#include <string>
#include "TileTypeUtils.hpp"

class TilesetManager {
private:
    sf::Texture tilesetTexture;
    std::map<TileType, sf::IntRect> tileCoordinates;
    int tileSize;
    
    // Initialise les coordonnées fixes de chaque TileType
    void initializeTileCoordinates();

public:
    TilesetManager();
    
    // Charge le megatileset depuis un fichier
    bool loadTileset(const std::string& filepath);
    
    // Récupère le IntRect pour un TileType donné
    sf::IntRect getTileRect(TileType type) const;
    
    // Récupère la texture complète du tileset
    const sf::Texture& getTexture() const;
    
    int getTileSize() const;
};

#endif 
