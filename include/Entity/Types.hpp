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

// Statuts alternatifs persistants
enum class Status {
    None,       // Aucun statut
    Poisoned,
    Sleep,
    Paralyzed,
    Burnt,
    Frozen
};

// Catégorie de dégâts d'une attaque
enum class DamageCategory {
    Physical,
    Special,
    Status     // Attaque sans dégâts directs
};

} // namespace Pokemon
