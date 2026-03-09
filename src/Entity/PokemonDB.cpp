#include "Entity/PokemonDB.hpp"
#include <fstream>
#include <stdexcept>

using json = nlohmann::json;
using namespace Pokemon;

// --- helpers de mapping ---

static Type typeFromString(const std::string& t) {
    if (t == "normal")   return Type::Normal;
    if (t == "fire")     return Type::Fire;
    if (t == "water")    return Type::Water;
    if (t == "grass")    return Type::Grass;
    if (t == "electric") return Type::Electric;
    if (t == "ice")      return Type::Ice;
    if (t == "fighting") return Type::Fighting;
    if (t == "poison")   return Type::Poison;
    if (t == "ground")   return Type::Ground;
    if (t == "flying")   return Type::Flying;
    if (t == "psychic")  return Type::Psychic;
    if (t == "bug")      return Type::Bug;
    if (t == "rock")     return Type::Rock;
    if (t == "ghost")    return Type::Ghost;
    if (t == "dragon")   return Type::Dragon;
    if (t == "dark")     return Type::Dark;
    if (t == "steel")    return Type::Steel;
    if (t == "fairy")    return Type::Fairy;
    return Type::Normal; // fallback
}

// --- impl ---

void PokemonDB::loadFromFile(const std::string& path) {
    std::ifstream in(path);
    if (!in) {
        throw std::runtime_error("PokemonDB::loadFromFile: impossible d'ouvrir " + path);
    }

    json j;
    in >> j;
    in.close();

    if (!j.is_array()) {
        throw std::runtime_error("PokemonDB::loadFromFile: le JSON racine doit être un tableau");
    }

    speciesById.clear();
    idByName.clear();

    for (const auto& e : j) {
        PokemonSpecies s{};

        // id + name
        s.id = e.at("id").get<int>();
        s.name = e.at("name").get<std::string>();

        // Types (array<Type,2>)
        s.types = { Type::Normal, Type::Normal };
        if (e.contains("types") && e["types"].is_array() && !e["types"].empty()) {
            const auto& arr = e["types"];
            s.types[0] = typeFromString(arr[0].get<std::string>());
            if (arr.size() > 1) {
                s.types[1] = typeFromString(arr[1].get<std::string>());
            }
        }

        // Stats de base (array<int,6> indexé par Stat)
        // Ordre de l'enum: HP, Atk, Def, SpAtk, SpDef, Speed
        s.baseStats = {0, 0, 0, 0, 0, 0};
        if (e.contains("stats") && e["stats"].is_object()) {
            const auto& st = e["stats"];
            s.baseStats[static_cast<int>(Stat::HP)]    = st.value("hp", 0);
            s.baseStats[static_cast<int>(Stat::Atk)]   = st.value("attack", 0);
            s.baseStats[static_cast<int>(Stat::Def)]   = st.value("defense", 0);
            s.baseStats[static_cast<int>(Stat::SpAtk)] = st.value("special-attack", 0);
            s.baseStats[static_cast<int>(Stat::SpDef)] = st.value("special-defense", 0);
            s.baseStats[static_cast<int>(Stat::Speed)] = st.value("speed", 0);
        }

        // Pour l’instant, JSON slim ne contient pas catchRate ni baseExpYield,
        // donc on met 0 ou une valeur par défaut.
        s.catchRate = 0;
        s.baseExpYield = 0;

        // Insertion
        speciesById.emplace(s.id, s);
        idByName.emplace(s.name, s.id);

        // Aussi indexer par name_fr
        if (e.contains("name_fr") && e["name_fr"].is_string()) {
            std::string fr = e["name_fr"].get<std::string>();
            if (!fr.empty()) {
                idByName.emplace(fr, s.id);
            }
        }
    }
}

const Pokemon::PokemonSpecies& PokemonDB::getSpecies(int id) const {
    auto it = speciesById.find(id);
    if (it == speciesById.end()) {
        throw std::out_of_range("PokemonDB::getSpecies: id inconnu");
    }
    return it->second;
}

const Pokemon::PokemonSpecies& PokemonDB::getSpeciesByName(const std::string& name) const {
    auto itId = idByName.find(name);
    if (itId == idByName.end()) {
        throw std::out_of_range("PokemonDB::getSpeciesByName: nom inconnu");
    }
    return getSpecies(itId->second);
}

bool PokemonDB::hasSpecies(int id) const {
    return speciesById.find(id) != speciesById.end();
}

size_t PokemonDB::size() const {
    return speciesById.size();
}
