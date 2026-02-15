#pragma once 

#include "PokemonSpecies.hpp"
#include "Types.hpp"
#include "Move.hpp"



class PokemonInstance {
private : 
    Pokemon::PokemonSpecies* species; 
    int lvl, xp, hp;
    std::array<int, 6> ivs;
    std::array<int, 6> evs; 
    Pokemon::Nature nature; 
    Pokemon::Status status; 
    Pokemon::StatusVolatile statusVolatile; 
    std::array<Pokemon::LearnedMove, 4> moves;
    void checkLevelUp();
    void levelUp();


public : 
    int calculateStat(Pokemon::Stat stat) const; 
    int getMaxHP();
    void takeDamage(int dmg);
    void heal(int heal);
    bool isFainted(); 
    void gainExp(int xpGained); 
    int getExpForNextLevel() const;


    // Getters
    Pokemon::PokemonSpecies* getSpecies() const;
    int getLvl() const;
    int getExp() const;
    int getHp() const;
    int getIV(Pokemon::Stat stat) const;
    int getEV(Pokemon::Stat stat) const;
    Pokemon::Nature getNature() const;
    Pokemon::Status getStatus() const;
    Pokemon::StatusVolatile getStatusVolatile() const;
    Pokemon::LearnedMove getLearnedMove(int index ) const;
    float getNatureMultiplier(Pokemon::Stat stat) const; 

    PokemonInstance(
        Pokemon::PokemonSpecies* speciesPtr,
        int level,
        Pokemon::Nature nat,
        const std::array<int, 6>& ivValues = {31, 31, 31, 31, 31, 31},
        const std::array<int, 6>& evValues = {0, 0, 0, 0, 0, 0}
    );
};