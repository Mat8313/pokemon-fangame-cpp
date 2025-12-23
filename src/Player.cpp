#include "Player.hpp"

using namespace std;
using namespace sf;

Player::Player() 
    : Character(), name("Player"), party(nullptr), bag(nullptr), 
      isMoving(false), targetX(0.0f), targetY(0.0f), moveProgress(0.0f) 
{}
void Player::setParty(Party* party) {
    this->party = party;
}
void Player::setBag(Bag* bag) {
    this->bag = bag;
}
void Player::setName(string name) {
    this->name = name;
}
string Player::getName() {
    return this->name;
}
Party* Player::getParty() {
    return this->party;
}
Bag* Player::getBag() {
    return this->bag;
}
bool Player::canMove(float newX, float newY){
    if (!currentMap) return false;
    
    // Convertir les coordonnées pixel en indices de tuile
    // Calculer le centre du joueur (ou le point de collision, généralement les pieds)
    float checkX = newX + (currentMap->getTileSize() / 2.0f);  // Centre horizontal
    float checkY = newY + currentMap->getTileSize() - 1;       // Bas du sprite (pieds)

    int tileX = static_cast<int>(checkX / currentMap->getTileSize());
    int tileY = static_cast<int>(checkY / currentMap->getTileSize());

    
    // Vérifier les limites de la map
    if (tileX < 0 || tileY < 0 || 
        tileX >= currentMap->getWidth() || 
        tileY >= currentMap->getHeight()) {
        return false;
    }
    
    // Récupérer la tuile de destination
    const Tile& tile = currentMap->getTile(tileX, tileY);
    
    // Vérifier si c'est un obstacle
    if (tile.getIsObstacle()) {
        return false;
    }
    
    // Vérifier la direction pour les rebords
    int dirX = (newX > getPositionX()) ? 1 : (newX < getPositionX()) ? -1 : 0;
    int dirY = (newY > getPositionY()) ? 1 : (newY < getPositionY()) ? -1 : 0;
    
    return TileTypeUtils::canPassDirection(tile.getType(), dirX, dirY);
}

void Player::handleInput(sf::Event& event) {
    if (event.type == sf::Event::KeyPressed && !isMoving) {
        float currentX = getPositionX();
        float currentY = getPositionY();
        float tileSize = currentMap->getTileSize(); // Taille d'une tile
        
        float newTargetX = currentX;
        float newTargetY = currentY;
        
        switch (event.key.code) {
            case sf::Keyboard::Up:
            case sf::Keyboard::Z:
                newTargetY = currentY - tileSize;
                setDirection(Direction::North);
                break;
            case sf::Keyboard::Down:
            case sf::Keyboard::S:
                newTargetY = currentY + tileSize;
                setDirection(Direction::South);
                break;
            case sf::Keyboard::Left:
            case sf::Keyboard::Q:
                newTargetX = currentX - tileSize;
                setDirection(Direction::West);
                break;
            case sf::Keyboard::Right:
            case sf::Keyboard::D:
                newTargetX = currentX + tileSize;
                setDirection(Direction::East);
                break;
            default:
                return;  // Touche non gérée
        }
        
        // Vérifier si le déplacement est possible
        if (canMove(newTargetX, newTargetY)) {
            targetX = newTargetX;
            targetY = newTargetY;
            isMoving = true;
            moveProgress = 0.0f;
        }
        // Sinon, ne rien faire (le joueur reste en place)
    }
}


void Player::update() {
    if (isMoving) {
        moveProgress += getMoveSpeed() / currentMap->getTileSize(); // Vitesse normalisée
        
        if (moveProgress >= 1.0f) {
            // Déplacement terminé 
            setPositionX(targetX);
            setPositionY(targetY);
            isMoving = false;
            moveProgress = 0.0f;
        }
        else if (moveProgress < 1.0f) {
            // Interpolation linéaire entre position actuelle et cible
                float startX = targetX - (targetX - getPositionX());
                float startY = targetY - (targetY - getPositionY());
                
                setPositionX(startX + (targetX - startX) * moveProgress);
                setPositionY(startY + (targetY - startY) * moveProgress);
            }
    }
}

void Player::setMap(Map* map) {
    this->currentMap = map;
}


