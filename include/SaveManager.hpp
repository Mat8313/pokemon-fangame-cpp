#ifndef SAVEMANAGER_HPP
#define SAVEMANAGER_HPP

#include "GameState.hpp"
#include <string>
#include <filesystem>
#include <vector>

class SaveManager {
private:
    static constexpr const char* SAVE_DIR = "../assets/saves/";
    static constexpr const char* SAVE_EXTENSION = ".sav";
    
    std::string getSavePath(int slot) const;
    bool ensureSaveDirectory() const;
    
public:
    SaveManager();
    
    // Sauvegarde
    bool saveGame(const GameState& state, int slot = 1);
    
    // Chargement
    bool loadGame(GameState& state, int slot = 1);
    
    // Vérification
    bool saveExists(int slot) const;
    std::string getLastSaveInfo(int slot) const;
    
    // Suppression
    bool deleteSave(int slot);
    
    // Slots multiples
    bool getAllSaves(std::vector<GameState>& states);
};

#endif
