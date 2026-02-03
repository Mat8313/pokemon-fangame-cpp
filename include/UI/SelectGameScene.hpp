#pragma once 

#include "UI/Scene.hpp"
#include "UI/Button.hpp"
#include "Utils/SaveManager.hpp"
#include <memory>
#include <vector>

class SelectGameScene : public Scene {
private:
    std::vector<Button> slotButtons;
    std::vector<GameState> saves;
    sf::Font font;
    sf::Text title;
    Button backButton;

    int selectedIndex; 
    void updateButtonSelection();
    
    SceneType nextScene;
    bool transitioning;
    SaveManager saveManager;
    
    void onSlotSelected(int slot);
    void onBack();
    
public:
    SelectGameScene();
    
    void handleInput(sf::Event& event) override;
    void render(sf::RenderWindow& window) override;
    
    SceneType getNextScene() const override { return nextScene; }
    bool shouldTransition() const override { return transitioning; }

    void update(float deltaTime);
};
