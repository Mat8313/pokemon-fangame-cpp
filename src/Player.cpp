#include "Player.hpp"

using namespace std;
using namespace sf;

Player::Player() 
    : Character(), name("Player"), party(nullptr), bag(nullptr), 
      isMoving(false), targetX(0.0f), targetY(0.0f), moveProgress(0.0f),
      currentFrame(0),
      animationTime(0.0f)
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
    // Calculer le centre du joueur 
    float checkX = newX + (currentMap->getTileSize());  // Centre horizontal
    float checkY = newY + (currentMap->getTileSize());  // centre vertical 

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
    else return true; 
    
    // Vérifier la direction pour les rebords
    //int dirX = (newX > getPositionX()) ? 1 : (newX < getPositionX()) ? -1 : 0;
    //int dirY = (newY > getPositionY()) ? 1 : (newY < getPositionY()) ? -1 : 0;
    
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
                return; // Touche non gérée
        }
        
        // Vérifier si le déplacement est possible
        if (canMove(newTargetX, newTargetY)) {
            targetX = newTargetX;
            targetY = newTargetY;
            isMoving = true;
            moveProgress = 0.0f;
            currentFrame = 0;  // Reset l'animation au début
            animationTime = 0.0f;
        }
        
        // Met à jour le sprite rect même si le mouvement n'est pas possible
        // (pour que le joueur se tourne vers la direction voulue)
        updateSpriteRect();
    }
}



void Player::update() {
    if (isMoving) {
        // Progression du déplacement
        moveProgress += getMoveSpeed() / currentMap->getTileSize(); // Vitesse normalisée
        
        // Animation des frames
        animationTime += 1.0f / 60.0f;  // Assume 60 FPS (ajuste selon ton deltaTime si tu en as un)
        
        if (animationTime >= frameTime) {
            animationTime = 0.0f;
            currentFrame = (currentFrame + 1) % framesPerDirection;
        }
        
        // Mise à jour du sprite rect selon la direction et la frame
        updateSpriteRect();
        
        if (moveProgress >= 1.0f) {
            // Déplacement terminé 
            setPositionX(targetX);
            setPositionY(targetY);
            isMoving = false;
            moveProgress = 0.0f;
            currentFrame = 0;  // Reset à la frame de repos
            updateSpriteRect();  // Applique la frame de repos
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

void Player::updateSpriteRect() {
    int row = 0;
    
    // Détermine la ligne selon la direction
    switch (getDirection()) {
        case Direction::South: row = 0; break;
        case Direction::North: row = 1; break;
        case Direction::West:  row = 2; break;
        case Direction::East:  row = 3; break;
    }
    
    // Calcule le rectangle pour la frame actuelle
    int col = isMoving ? currentFrame : 1;  // Frame du milieu quand statique
    sf::IntRect rect(
        col * spriteWidth, 
        row * spriteHeight, 
        spriteWidth, 
        spriteHeight
    );
    
    getSprite().setTextureRect(rect);
}


