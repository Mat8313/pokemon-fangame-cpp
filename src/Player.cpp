#include "Player.hpp"

using namespace std;
using namespace sf;

Player::Player() 
    : Character(), name("Player"), party(nullptr), bag(nullptr), 
      isMoving(false), targetX(0.0f), targetY(0.0f), moveProgress(0.0f),
      currentFrame(0), animationTime(0.0f), leftFootNext(false), hasNextMove(false)
{
    loadPlayerTexture();
}

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

bool Player::canMove(float newX, float newY) {
    if (!currentMap) return false;

    float tileSize = currentMap->getTileSize();

    // Centre du joueur (ou bas du sprite) selon ta convention
    float centerX = newX + tileSize / 2.0f;
    float centerY = newY + tileSize;        // bas du sprite

    int tileX = static_cast<int>(centerX / tileSize);
    int tileY = static_cast<int>(centerY / tileSize);

    // Limites de la map
    if (tileX < 0 || tileY < 0 ||
        tileX >= currentMap->getWidth() ||
        tileY >= currentMap->getHeight()) {
        return false;
    }

    const Tile& tile = currentMap->getTile(tileX, tileY);
    if (tile.getIsObstacle()) {
        return false;
    }

    return true;
}


void Player::startMovement(Direction dir) {
    float currentX = getPositionX();
    float currentY = getPositionY();
    float tileSize = currentMap->getTileSize();
    
    // Si on change de direction, on se tourne SANS bouger
    if (getDirection() != dir) {
        setDirection(dir);
        updateSpriteRect();
        return;
    }
    
    float newTargetX = currentX;
    float newTargetY = currentY;
    
    // Si on regarde déjà dans cette direction, on avance
    switch (dir) {
        case Direction::North:
            newTargetY = currentY - tileSize;
            break;
        case Direction::South:
            newTargetY = currentY + tileSize;
            break;
        case Direction::West:
            newTargetX = currentX - tileSize;
            break;
        case Direction::East:
            newTargetX = currentX + tileSize;
            break;
    }
    
    if (canMove(newTargetX, newTargetY)) {
        targetX = newTargetX;
        targetY = newTargetY;
        isMoving = true;
        moveProgress = 0.0f;
        currentFrame = 1;
        animationTime = 0.0f;
        updateSpriteRect();
    } else {
        updateSpriteRect();
    }
}

void Player::handleInput(sf::Event& event) {
    if (event.type == sf::Event::KeyPressed) {
        Direction requestedDirection;
        bool validInput = false;
        
        switch (event.key.code) {
            case sf::Keyboard::Up:
            case sf::Keyboard::Z:
                requestedDirection = Direction::North;
                validInput = true;
                break;
            case sf::Keyboard::Down:
            case sf::Keyboard::S:
                requestedDirection = Direction::South;
                validInput = true;
                break;
            case sf::Keyboard::Left:
            case sf::Keyboard::Q:
                requestedDirection = Direction::West;
                validInput = true;
                break;
            case sf::Keyboard::Right:
            case sf::Keyboard::D:
                requestedDirection = Direction::East;
                validInput = true;
                break;
            default:
                return;
        }
        
        if (!validInput) return;
        
        // Si déjà en mouvement, enregistre le prochain mouvement
        if (isMoving) {
            nextDirection = requestedDirection;
            hasNextMove = true;
        } 
        // Sinon, démarre le mouvement immédiatement
        else {
            startMovement(requestedDirection);
        }
    }
}

void Player::update(float deltaTime) {
    if (isMoving) {
        // Progression basée sur le TEMPS réel
        float pixelsPerSecond = getMoveSpeed();
        float tileSize = currentMap->getTileSize();
        float progression = (pixelsPerSecond * deltaTime) / tileSize;
        moveProgress += progression;
        
        // Clamp pour ne pas dépasser
        if (moveProgress > 1.0f) {
            moveProgress = 1.0f;
        }
        
        // Animation en 3 phases
        if (moveProgress < 0.33f) {
            currentFrame = 1;  // Statique
        }
        else if (moveProgress < 0.66f) {
            currentFrame = leftFootNext ? 0 : 2;  // Pied levé
        }
        else {
            currentFrame = 1;  // Retour statique
        }
        
        updateSpriteRect();
        
        if (moveProgress >= 1.0f) {
            // Déplacement terminé
            setPositionX(targetX);
            setPositionY(targetY);
            isMoving = false;
            moveProgress = 0.0f;
            currentFrame = 1;
            leftFootNext = !leftFootNext;
            updateSpriteRect();

            // Enchaîne le prochain mouvement
            if (hasNextMove) {
                hasNextMove = false;
                startMovement(nextDirection);
            }

            // --- Détection d'un warp sur la case où on vient d'arriver ---
            if (currentMap) {
                float tileSize = currentMap->getTileSize();
                int tileX = static_cast<int>(getPositionX() / tileSize);
                int tileY = static_cast<int>(getPositionY() / tileSize);

                std::cout << "Player tile: " << tileX << "," << tileY << std::endl;

                const Warp* w = currentMap->getWarpAt(tileX, tileY);
                if (w) {
                    pendingWarp = *w;
                    hasPendingWarp = true;
                    std::cout << "Warp trouvé vers " << w->targetMap
                              << " (" << w->targetX << "," << w->targetY << ")\n";
                }
            }
        }

        else {
            // Interpolation linéaire
            float startX, startY;
            
            switch (getDirection()) {
                case Direction::North:
                    startX = targetX;
                    startY = targetY + tileSize;
                    break;
                case Direction::South:
                    startX = targetX;
                    startY = targetY - tileSize;
                    break;
                case Direction::West:
                    startX = targetX + tileSize;
                    startY = targetY;
                    break;
                case Direction::East:
                    startX = targetX - tileSize;
                    startY = targetY;
                    break;
            }
            
            float currentX = startX + (targetX - startX) * moveProgress;
            float currentY = startY + (targetY - startY) * moveProgress;
            
            setPositionX(currentX);
            setPositionY(currentY);

            
        }
    }
}

void Player::setMap(Map* map) {
    this->currentMap = map;
}

void Player::updateSpriteRect() {
    int row = 0;
    
    switch (getDirection()) {
        case Direction::South: row = 0; break;
        case Direction::North: row = 1; break;
        case Direction::West: row = 2; break;
        case Direction::East: row = 3; break;
    }
    
    int col = isMoving ? currentFrame : 1;
    sf::IntRect rect(
        col * spriteWidth, 
        row * spriteHeight, 
        spriteWidth, 
        spriteHeight
    );
    
    getSpriteRef().setTextureRect(rect);
}

void Player::loadPlayerTexture() {
    if (!playerTexture.loadFromFile("../assets/sprites/player.png")) {
        std::cerr << "ERROR: Cannot load player sprite sheet!" << std::endl;
        return;
    }
    
    getSpriteRef().setTexture(playerTexture);
    getSpriteRef().setTextureRect(sf::IntRect(spriteWidth, 0, spriteWidth, spriteHeight));
}

bool Player::hasWarpRequest() const { return hasPendingWarp; }

Warp Player::consumeWarpRequest() {
    hasPendingWarp = false;
    return pendingWarp;
}

//gerer le offset de (1;1)



