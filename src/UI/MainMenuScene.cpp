#include "UI/MainMenuScene.hpp"
#include <iostream>

MainMenuScene::MainMenuScene()
    : nextScene(SceneType::MainMenu), transitioning(false)
{
    // Charger la font (utilise une font standard ou crée-la)
    if (!font.loadFromFile("../assets/fonts/Pokemon.ttf")) {
        std::cerr << "Warning: Font not found" << std::endl;
        // Continue même sans font
    }
    
    // Titre
    title.setString("POKEMON FANGAME");
    title.setFont(font);
    title.setCharacterSize(32);
    title.setFillColor(sf::Color::Yellow);
    title.setPosition(150, 50);
    
    // Boutons
    buttons.push_back(Button("NEW GAME", 250, 200, 300, 60, font));
    buttons.push_back(Button("LOAD GAME", 250, 290, 300, 60, font));
    buttons.push_back(Button("SETTINGS", 250, 380, 300, 60, font));
    buttons.push_back(Button("QUIT", 250, 470, 300, 60, font));
    
    // Callbacks
    buttons[0].setCallback([this]() { onNewGame(); });
    buttons[1].setCallback([this]() { onLoadGame(); });
    buttons[2].setCallback([this]() { onSettings(); });
    buttons[3].setCallback([this]() { onQuit(); });

    buttons[0].setSelected(true);
}

void MainMenuScene::onNewGame() {
    std::cout << "New Game selected" << std::endl;
    nextScene = SceneType::NewGameMenu;
    transitioning = true;
}

void MainMenuScene::onLoadGame() {
    std::cout << "Load Game selected" << std::endl;
    nextScene = SceneType::SelectGame;
    transitioning = true;
}

void MainMenuScene::onSettings() {
    std::cout << "Settings selected" << std::endl;
}

void MainMenuScene::onQuit() {
    std::cout << "Quit selected" << std::endl;
    nextScene = SceneType::Quit; 
    transitioning = true;
}

void MainMenuScene::handleInput(sf::Event& event) {
    if (event.type == sf::Event::KeyPressed) {
        if (event.key.code == sf::Keyboard::Up || event.key.code == sf::Keyboard::Z) {
            selectedIndex = (selectedIndex - 1 + buttons.size()) % buttons.size();
            updateButtonSelection();
        }
        else if (event.key.code == sf::Keyboard::Down || event.key.code == sf::Keyboard::S) {
            selectedIndex = (selectedIndex + 1) % buttons.size();
            updateButtonSelection();
        }
        else if (event.key.code == sf::Keyboard::Space || event.key.code == sf::Keyboard::Return) {
            buttons[selectedIndex].click();
        }
    }
}

void MainMenuScene::updateButtonSelection() {
    for (size_t i = 0; i < buttons.size(); ++i) {
        buttons[i].setSelected(i == static_cast<size_t>(selectedIndex));
    }
}


void MainMenuScene::render(sf::RenderWindow& window) {
    window.clear(sf::Color::Black);
    
    window.draw(title);
    
    for (auto& button : buttons) {
        button.render(window);
    }
    
    window.display();
}

void MainMenuScene::update(float deltaTime) {
    updateButtonSelection();
}
