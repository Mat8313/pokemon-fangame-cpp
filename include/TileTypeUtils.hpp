#ifndef TILETYPEUTILS_HPP
#define TILETYPEUTILS_HPP

#include <string>

enum class TileType {
    GRASS,          // Herbe normale
    TALL_GRASS,     // Hautes herbes (combats sauvages)
    PATH,           // Chemin/piéton
    WATER,          // Eau (nécessite Surf)
    SAND,           // Sable
    ROAD,           // Route
    WALL,           // Mur/Obstacle solide
    TREE,           // Grand arbre
    SMALLTREE,      // Arbuste (nécessite Coupe)
    ROCK,           // Rocher (nécessite Force/Éclate-Roc)
    LEDGE_UP,       // Rebord saut vers le haut
    LEDGE_DOWN,     // Rebord saut vers le bas
    LEDGE_LEFT,     // Rebord saut vers la gauche
    LEDGE_RIGHT,    // Rebord saut vers la droite
    DOOR,           // Porte (point d'entrée bâtiment)
    ICE             // Glace (glisse)
};

class TileTypeUtils {
public:
    // Vérifier si une tuile est un obstacle
    static bool isObstacle(TileType type);
    
    // Vérifier si une tuile déclenche des combats
    static bool triggersBattle(TileType type);
    
    // Vérifier si une tuile nécessite une capacité spéciale
    static bool requiresHM(TileType type);
    
    // Obtenir le nom de la capacité requise
    static std::string getRequiredHM(TileType type);
    
    // Obtenir la vitesse de déplacement sur une tuile
    static float getSpeedModifier(TileType type);
    
    // Vérifier si on peut passer en direction spécifique (pour les rebords)
    static bool canPassDirection(TileType type, int dirX, int dirY);
    
    // Convertir un char (depuis fichier map) en TileType
    static TileType charToType(char c);
    
    // Convertir un TileType en string (pour debug)
    static std::string toString(TileType type);
};

#endif