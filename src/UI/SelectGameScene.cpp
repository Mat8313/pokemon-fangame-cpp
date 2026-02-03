#include "UI/SelectGameScene.hpp"
#include <iostream>

SelectGameScene::SelectGameScene()
    : backButton("BACK", 300, 450, 200, 50, {}),
      nextScene(SceneType::MainMenu), transitioning(false), selectedIndex(0)
{
    // Charge font
    if (!font.loadFromFile("../assets/fonts/Pokemon.ttf")) {
        std::cerr << "Warning: Font not found" << std::endl;
    }
    
    // Titre
    title.setString("SELECT GAME SLOT");
    title.setFont(font);
    title.setCharacterSize(32);
    title.setFillColor(sf::Color::Yellow);
    title.setPosition(150, 50);
    
    // Charger les sauvegardes existantes
    saveManager.getAllSaves(saves);
    
    // Créer les boutons pour chaque slot
    for (int i = 0; i < 3; ++i) {
        std::string label = "SLOT " + std::to_string(i + 1);
        if (i < static_cast<int>(saves.size())) {
            label += "\n" + saveManager.getLastSaveInfo(i + 1);
        } else {
            label += "\n(Empty)";
        }
        
        slotButtons.push_back(Button(label, 150, 150 + i * 80, 500, 70, font, 14));
        int slot = i + 1;
        slotButtons[i].setCallback([this, slot]() { onSlotSelected(slot); });
    }
    
    // Bouton Back
    backButton = Button("BACK", 300, 450, 200, 50, font);
    backButton.setCallback([this]() { onBack(); });
    slotButtons[0].setSelected(true);
}

void SelectGameScene::onSlotSelected(int slot) {
    std::cout << "Loading from slot " << slot << std::endl;
    nextScene = SceneType::Gameplay;
    transitioning = true;
}

void SelectGameScene::onBack() {
    std::cout << "Back to menu" << std::endl;
    nextScene = SceneType::MainMenu;
    transitioning = true;
}

void SelectGameScene::handleInput(sf::Event& event) {
    if (event.type == sf::Event::KeyPressed) {
        // HAUT : naviguer vers le haut
        if (event.key.code == sf::Keyboard::Up || event.key.code == sf::Keyboard::Z) {
            selectedIndex = (selectedIndex - 1 + slotButtons.size()) % slotButtons.size();
            updateButtonSelection();
        }
        // BAS : naviguer vers le bas
        else if (event.key.code == sf::Keyboard::Down || event.key.code == sf::Keyboard::S) {
            selectedIndex = (selectedIndex + 1) % slotButtons.size();
            updateButtonSelection();
        }
        // A (ou SPACE/ENTER) : valider la sauvegarde
        else if (event.key.code == sf::Keyboard::Space || event.key.code == sf::Keyboard::Return) {
            slotButtons[selectedIndex].click();
        }
        // B (ou ESC) : retour
        else if (event.key.code == sf::Keyboard::E) {
            onBack();
        }
    }
}



void SelectGameScene::render(sf::RenderWindow& window) {
    window.clear(sf::Color::Black);
    
    window.draw(title);
    
    for (auto& button : slotButtons) {
        button.render(window);
    }
    
    backButton.render(window);
    
    window.display();
}


void SelectGameScene::update(float deltaTime) {
    // no Implementation.
}

 void SelectGameScene::updateButtonSelection() {
    // no Implementation.
}
