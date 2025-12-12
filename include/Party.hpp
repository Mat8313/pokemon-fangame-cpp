#ifndef PARTY_HPP
#define PARTY_HPP

#include <vector>
#include "Pokemon.hpp"


class Party {
private : 
    std::vector<Pokemon*> pokemons; 
public : 
    std::vector<Pokemon*> getPokemons();
    void setPokemons(Pokemon* pokemon, int index);
};

#endif 