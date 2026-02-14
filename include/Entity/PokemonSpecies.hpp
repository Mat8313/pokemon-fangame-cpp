#pragma once

#include "Types.hpp"
#include <string>
#include <array>

namespace Pokemon {

// Espèce de Pokémon (données immuables partagées par tous les individus)
struct PokemonSpecies {
    int id;                                    // Numéro National Dex
    std::string name;                          // Nom de l'espèce (ex: "Bulbasaur")
    std::array<Type, 2> types;                 // Type primaire et secondaire (Normal si mono-type)
    std::array<int, 6> baseStats;              // Stats de base indexées par Stat enum
    
    int catchRate;                             // Taux de capture (0-255, plus c'est haut plus c'est facile)
    int baseExpYield;                          // XP donnée quand vaincu
    
    // TODO: Courbe d'XP (Erratic, Fast, Medium Fast, etc.)
    // TODO: Liste des attaques apprises par niveau
    // TODO: Conditions et cibles d'évolution
    // TODO: Talents (abilities)
    
    // Vérifie si l'espèce possède un type donné
    bool isType(Type t) const {
        return types[0] == t || types[1] == t;
    }
};

} // namespace Pokemon
