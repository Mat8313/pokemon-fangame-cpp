#pragma once

namespace Pokemon {

// Types élémentaires (Gen 1-6, tous inclus pour future-proof)
enum class Type {
    Normal,
    Fire,
    Water,
    Grass,
    Electric,
    Ice,
    Fighting,
    Poison,
    Ground,
    Flying,
    Psychic,
    Bug,
    Rock,
    Ghost,
    Dragon,
    Dark,      // Gen 2
    Steel,     // Gen 2
    Fairy      // Gen 6
};

// Statistiques de base d'un Pokémon
enum class Stat {
    HP,
    Atk,
    Def,
    SpAtk,
    SpDef,
    Speed
};

// Statuts persistants
enum class Status {
    None,       // Aucun statut
    Poisoned,
    Sleep,
    Paralyzed,
    Burnt,
    Frozen
};

// Status volatiles 
struct StatusVolatile {
    bool confused = false;
    int confusionTurns = 0;
    // TODO: autres conditions volatiles
};

// Catégorie de dégâts d'une attaque
enum class DamageCategory {
    Physical,
    Special,
    Status     // Attaque sans dégâts directs
};

// Nature du pokemon  
enum class Nature {
    Hardy, Lonely, Brave, Adamant, Naughty,    // Atk-focused
    Bold, Docile, Relaxed, Impish, Lax,        // Def-focused
    Timid, Hasty, Serious, Jolly, Naive,       // Speed-focused
    Modest, Mild, Quiet, Bashful, Rash,        // SpAtk-focused
    Calm, Gentle, Sassy, Careful, Quirky       // SpDef-focused
};

} // namespace Pokemon
