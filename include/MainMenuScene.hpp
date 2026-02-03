#ifndef MAINMENUSCENE_HPP
#define MAINMENUSCENE_HPP

#include "Scene.hpp"
#include "Button.hpp"
#include <memory>

class MainMenuScene : public Scene {
private:
    std::vector<Button> buttons;
    sf::Font font;
    sf::Text title;
    int selectedIndex;

    SceneType nextScene;
    bool transitioning;

    void updateButtonSelection();
    void update(float deltaTime);
    
    // Callbacks
    void onNewGame();
    void onLoadGame();
    void onSettings();
    void onQuit();
    
public:
    MainMenuScene();
    
    void handleInput(sf::Event& event) override;
    void render(sf::RenderWindow& window) override;
    
    SceneType getNextScene() const override { return nextScene; }
    bool shouldTransition() const override { return transitioning; }
};

#endif
