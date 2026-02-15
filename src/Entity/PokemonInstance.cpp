#pragma once 

#include "PokemonInstance.hpp"
#include <cmath>

using namespace Pokemon;





int PokemonInstance::calculateStat(Stat stat){
    if (stat == Stat::HP){
        return floor(((2 * species->baseStats[static_cast<int>(stat)] + getIV(stat) + floor(getEV(stat) / 4)) * lvl) / 100) + lvl + 10 ;
    }
    else {
        return floor((floor(((2 * species->baseStats[static_cast<int>(stat)] + getIV(stat) + floor(getEV(stat) / 4)) * lvl) / 100) + 5) * getNatureMultiplier(stat));
    }
}

int PokemonInstance::getMaxHP() {
    return calculateStat(Stat::HP);
}

void PokemonInstance::takeDamage(int dmg){
    int newHp = hp -dmg;
    if (newHp <= 0){
        hp = 0;
    }
    else {
        hp = newHp;
    }
}

void PokemonInstance::heal(int heal) {
    int newHp = hp + heal;
    if (newHp > getMaxHP()){
        hp = getMaxHP();
    }
    else {
        hp = newHp;
    }
}

bool PokemonInstance::isFainted(){
    if (!hp) return true;
}

void PokemonInstance::gainExp(int xpGained){
    xp += xpGained;
}

float PokemonInstance::getNatureMultiplier(Stat stat) const {
    // Les 25 natures et leurs effets sur les stats
    switch (nature) {
        // Natures neutres (pas de modificateur)
        case Pokemon::Nature::Hardy:
        case Pokemon::Nature::Docile:
        case Pokemon::Nature::Serious:
        case Pokemon::Nature::Bashful:
        case Pokemon::Nature::Quirky:
            return 1.0f;
        
        // +Atk -Def
        case Pokemon::Nature::Lonely:
            if (stat == Pokemon::Stat::Atk) return 1.1f;
            if (stat == Pokemon::Stat::Def) return 0.9f;
            return 1.0f;
        
        // +Atk -SpAtk
        case Pokemon::Nature::Adamant:
            if (stat == Pokemon::Stat::Atk) return 1.1f;
            if (stat == Pokemon::Stat::SpAtk) return 0.9f;
            return 1.0f;
        
        // +Atk -SpDef
        case Pokemon::Nature::Naughty:
            if (stat == Pokemon::Stat::Atk) return 1.1f;
            if (stat == Pokemon::Stat::SpDef) return 0.9f;
            return 1.0f;
        
        // +Atk -Speed
        case Pokemon::Nature::Brave:
            if (stat == Pokemon::Stat::Atk) return 1.1f;
            if (stat == Pokemon::Stat::Speed) return 0.9f;
            return 1.0f;
        
        // +Def -Atk
        case Pokemon::Nature::Bold:
            if (stat == Pokemon::Stat::Def) return 1.1f;
            if (stat == Pokemon::Stat::Atk) return 0.9f;
            return 1.0f;
        
        // +Def -SpAtk
        case Pokemon::Nature::Impish:
            if (stat == Pokemon::Stat::Def) return 1.1f;
            if (stat == Pokemon::Stat::SpAtk) return 0.9f;
            return 1.0f;
        
        // +Def -SpDef
        case Pokemon::Nature::Lax:
            if (stat == Pokemon::Stat::Def) return 1.1f;
            if (stat == Pokemon::Stat::SpDef) return 0.9f;
            return 1.0f;
        
        // +Def -Speed
        case Pokemon::Nature::Relaxed:
            if (stat == Pokemon::Stat::Def) return 1.1f;
            if (stat == Pokemon::Stat::Speed) return 0.9f;
            return 1.0f;
        
        // +SpAtk -Atk
        case Pokemon::Nature::Modest:
            if (stat == Pokemon::Stat::SpAtk) return 1.1f;
            if (stat == Pokemon::Stat::Atk) return 0.9f;
            return 1.0f;
        
        // +SpAtk -Def
        case Pokemon::Nature::Mild:
            if (stat == Pokemon::Stat::SpAtk) return 1.1f;
            if (stat == Pokemon::Stat::Def) return 0.9f;
            return 1.0f;
        
        // +SpAtk -SpDef
        case Pokemon::Nature::Rash:
            if (stat == Pokemon::Stat::SpAtk) return 1.1f;
            if (stat == Pokemon::Stat::SpDef) return 0.9f;
            return 1.0f;
        
        // +SpAtk -Speed
        case Pokemon::Nature::Quiet:
            if (stat == Pokemon::Stat::SpAtk) return 1.1f;
            if (stat == Pokemon::Stat::Speed) return 0.9f;
            return 1.0f;
        
        // +SpDef -Atk
        case Pokemon::Nature::Calm:
            if (stat == Pokemon::Stat::SpDef) return 1.1f;
            if (stat == Pokemon::Stat::Atk) return 0.9f;
            return 1.0f;
        
        // +SpDef -Def
        case Pokemon::Nature::Gentle:
            if (stat == Pokemon::Stat::SpDef) return 1.1f;
            if (stat == Pokemon::Stat::Def) return 0.9f;
            return 1.0f;
        
        // +SpDef -SpAtk
        case Pokemon::Nature::Careful:
            if (stat == Pokemon::Stat::SpDef) return 1.1f;
            if (stat == Pokemon::Stat::SpAtk) return 0.9f;
            return 1.0f;
        
        // +SpDef -Speed
        case Pokemon::Nature::Sassy:
            if (stat == Pokemon::Stat::SpDef) return 1.1f;
            if (stat == Pokemon::Stat::Speed) return 0.9f;
            return 1.0f;
        
        // +Speed -Atk
        case Pokemon::Nature::Timid:
            if (stat == Pokemon::Stat::Speed) return 1.1f;
            if (stat == Pokemon::Stat::Atk) return 0.9f;
            return 1.0f;
        
        // +Speed -Def
        case Pokemon::Nature::Hasty:
            if (stat == Pokemon::Stat::Speed) return 1.1f;
            if (stat == Pokemon::Stat::Def) return 0.9f;
            return 1.0f;
        
        // +Speed -SpAtk
        case Pokemon::Nature::Jolly:
            if (stat == Pokemon::Stat::Speed) return 1.1f;
            if (stat == Pokemon::Stat::SpAtk) return 0.9f;
            return 1.0f;
        
        // +Speed -SpDef
        case Pokemon::Nature::Naive:
            if (stat == Pokemon::Stat::Speed) return 1.1f;
            if (stat == Pokemon::Stat::SpDef) return 0.9f;
            return 1.0f;
        
        default:
            return 1.0f;
    }
}

int PokemonInstance::getEV(Stat stat) const {
    return evs[static_cast<int>(stat)];
}

int PokemonInstance::getIV(Stat stat) const {
    return ivs[static_cast<int>(stat)];
}

int PokemonInstance::getHp() const {
    return hp; 
}

int PokemonInstance::getExp() const {
    return xp;
}

int PokemonInstance::getLvl() const {
    return lvl;
}

Nature PokemonInstance::getNature() const {
    return nature;
}

Status PokemonInstance::getStatus() const {
    return status;
}

StatusVolatile PokemonInstance::getStatusVolatile() const {
    return statusVolatile;
}

LearnedMove PokemonInstance::getLearnedMove(int index) const {
    return moves[index];
}