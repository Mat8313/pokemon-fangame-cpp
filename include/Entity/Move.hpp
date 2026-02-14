#pragma once

#include "Types.hpp"
#include <string>

namespace Pokemon {

// Représente une attaque Pokémon avec toutes ses caractéristiques
struct Move {
    int id;                          // ID unique de l'attaque
    std::string name;                // Nom de l'attaque (ex: "Flamethrower")
    Type type;                       // Type élémentaire de l'attaque
    DamageCategory category;         // Physical, Special ou Status
    
    int power;                       // Puissance de base (0 si attaque de statut)
    int accuracy;                    // Précision (0-100), ou -1 si ne rate jamais
    int maxPP;                       // Points de Pouvoir maximum
    
    int priority;                    // Priorité d'exécution (0 = normal, +1/+2 = rapide, -1/-7 = lent)
    int effectChance;                // Chance de déclencher l'effet secondaire (0-100)
    
    std::string description;         // Description de l'effet de l'attaque
    
    // TODO: Effet secondaire (baisse stats, statut, flinch, etc.)
    // TODO: Target (un ennemi, tous les ennemis, soi-même, allié, etc.)
    // TODO: Flags spéciaux (contact, sound, punch, bite, etc.)
    
    // Vérifie si l'attaque inflige des dégâts
    bool isDamaging() const {
        return power > 0;
    }
    
    // Vérifie si l'attaque rate parfois
    bool canMiss() const {
        return accuracy >= 0 && accuracy < 100;
    }
};

} // namespace Pokemon
