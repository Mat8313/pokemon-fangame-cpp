#include "SaveManager.hpp"
#include "nlohmann/json.hpp"
#include <fstream>
#include <iostream>
#include <ctime>
#include <iomanip>

using json = nlohmann::json;
namespace fs = std::filesystem;

SaveManager::SaveManager() {
    ensureSaveDirectory();
}

std::string SaveManager::getSavePath(int slot) const {
    return std::string(SAVE_DIR) + "save_slot_" + std::to_string(slot) + SAVE_EXTENSION;
}

bool SaveManager::ensureSaveDirectory() const {
    try {
        fs::create_directories(SAVE_DIR);
        return true;
    } catch (const std::exception& e) {
        std::cerr << "Error creating save directory: " << e.what() << std::endl;
        return false;
    }
}

bool SaveManager::saveGame(const GameState& state, int slot) {
    try {
        json saveData;
        
        // Sérialiser les données du joueur
        saveData["player"]["name"] = state.player.name;
        saveData["player"]["positionX"] = state.player.position.x;
        saveData["player"]["positionY"] = state.player.position.y;
        saveData["player"]["currentMap"] = state.player.currentMap;
        saveData["player"]["playtime"] = state.player.playtime;
        
        // Métadonnées de sauvegarde
        saveData["playtime"] = state.playtime;
        saveData["slotNumber"] = slot;
        
        // Timestamp
        auto now = std::time(nullptr);
        auto tm = *std::localtime(&now);
        std::ostringstream oss;
        oss << std::put_time(&tm, "%d-%m-%Y %H:%M:%S");
        saveData["lastSaveTime"] = oss.str();
        
        // Écrire dans le fichier
        std::string savePath = getSavePath(slot);
        std::ofstream saveFile(savePath);
        if (!saveFile.is_open()) {
            std::cerr << "Cannot open save file: " << savePath << std::endl;
            return false;
        }
        
        saveFile << saveData.dump(4);  // Format indenté
        saveFile.close();
        
        std::cout << "Game saved to slot " << slot << std::endl;
        return true;
        
    } catch (const std::exception& e) {
        std::cerr << "Error saving game: " << e.what() << std::endl;
        return false;
    }
}

bool SaveManager::loadGame(GameState& state, int slot) {
    try {
        std::string savePath = getSavePath(slot);
        
        std::ifstream saveFile(savePath);
        if (!saveFile.is_open()) {
            std::cerr << "Save file not found: " << savePath << std::endl;
            return false;
        }
        
        json saveData;
        saveFile >> saveData;
        saveFile.close();
        
        // Désérialiser
        state.player.name = saveData["player"]["name"];
        state.player.position.x = saveData["player"]["positionX"];
        state.player.position.y = saveData["player"]["positionY"];
        state.player.currentMap = saveData["player"]["currentMap"];
        state.player.playtime = saveData["player"]["playtime"];
        
        state.playtime = saveData["playtime"];
        state.lastSaveTime = saveData["lastSaveTime"];
        state.slotNumber = slot;
        
        std::cout << "Game loaded from slot " << slot << std::endl;
        return true;
        
    } catch (const std::exception& e) {
        std::cerr << "Error loading game: " << e.what() << std::endl;
        return false;
    }
}

bool SaveManager::saveExists(int slot) const {
    try {
        return fs::exists(getSavePath(slot));
    } catch (const std::exception& e) {
        std::cerr << "Error checking save: " << e.what() << std::endl;
        return false;
    }
}

std::string SaveManager::getLastSaveInfo(int slot) const {
    try {
        std::string savePath = getSavePath(slot);
        if (!fs::exists(savePath)) {
            return "No save";
        }
        
        std::ifstream saveFile(savePath);
        json saveData;
        saveFile >> saveData;
        saveFile.close();
        
        std::string playerName = saveData["player"]["name"];
        std::string lastTime = saveData["lastSaveTime"];
        int playtime = saveData["playtime"];
        
        return playerName + " - " + lastTime + " (" + std::to_string(playtime/3600) + "h)";
        
    } catch (const std::exception& e) {
        return "Error reading save";
    }
}

bool SaveManager::deleteSave(int slot) {
    try {
        std::string savePath = getSavePath(slot);
        if (fs::exists(savePath)) {
            fs::remove(savePath);
            std::cout << "Save slot " << slot << " deleted" << std::endl;
            return true;
        }
        return false;
    } catch (const std::exception& e) {
        std::cerr << "Error deleting save: " << e.what() << std::endl;
        return false;
    }
}

bool SaveManager::getAllSaves(std::vector<GameState>& states) {
    try {
        for (int i = 1; i <= 3; ++i) {  // Supposons 3 slots max
            if (saveExists(i)) {
                GameState state;
                if (loadGame(state, i)) {
                    states.push_back(state);
                }
            }
        }
        return true;
    } catch (const std::exception& e) {
        std::cerr << "Error getting all saves: " << e.what() << std::endl;
        return false;
    }
}
