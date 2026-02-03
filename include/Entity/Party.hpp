#pragma once 

#include <vector>
#include "Entity/Pokemon.hpp"


class Party {
private : 
    std::vector<Pokemon*> pokemons; 
public : 
    std::vector<Pokemon*> getPokemons();
    void setPokemons(Pokemon* pokemon, int index);
};