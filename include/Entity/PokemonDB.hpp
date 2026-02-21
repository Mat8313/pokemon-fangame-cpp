#pragma once

#include "Entity/PokemonSpecies.hpp"
#include "Entity/Types.hpp"
#include "Entity/PokemonInstance.hpp"
#include "nlohmann/json.hpp" 
#include <string>

class PokemonDB {
private : 
    std::unordered_map<int, Pokemon::PokemonSpecies&> speciesById; 
    std::unordered_map<std::string, int> idByName; 

public :
    // 1) Chargement
    void loadFromFile(const std::string& path);

    // 2) Accès par id / nom
    const Pokemon::PokemonSpecies& getSpecies(int id) const;
    const Pokemon::PokemonSpecies& getSpeciesByName(const std::string& name) const;

    bool hasSpecies(int id) const;
    size_t size() const;

};