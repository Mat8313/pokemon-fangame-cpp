#ifndef SCENE_HPP
#define SCENE_HPP

#include <SFML/Graphics.hpp>

enum class SceneType {
    MainMenu,
    SelectGame,
    NewGameMenu,
    Gameplay,
    BattleScene,
    PauseMenu,
    Quit
};

class Scene {
public:
    virtual ~Scene() = default;
    
    // Cycle de vie de la scène
    virtual void handleInput(sf::Event& event) = 0;
    virtual void update(float deltaTime) = 0;
    virtual void render(sf::RenderWindow& window) = 0;
    
    // Gestion des transitions
    virtual SceneType getNextScene() const { return SceneType::MainMenu; }
    virtual bool shouldTransition() const { return false; }
};

#endif
