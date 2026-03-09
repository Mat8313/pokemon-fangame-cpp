#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <set>

#include "../include/nlohmann/json.hpp" // nlohmann::json : https://github.com/nlohmann/json

using json = nlohmann::json;

// ----- Helpers pour les infos FR -----

std::string get_french_name(const json& species) {
    if (!species.contains("names")) return "";
    for (const auto& n : species["names"]) {
        if (n.contains("language") && n["language"]["name"] == "fr") {
            return n["name"].get<std::string>();
        }
    }
    return "";
}

std::string get_french_flavor(const json& species) {
    if (!species.contains("flavor_text_entries")) return "";
    std::string last_fr = "";
    for (const auto& ft : species["flavor_text_entries"]) {
        if (ft.contains("language") && ft["language"]["name"] == "fr") {
            last_fr = ft["flavor_text"].get<std::string>();
        }
    }
    return last_fr;
}

// ----- Helpers pour types / abilities / stats / moves / sprites -----

json build_stats(const json& pokemon) {
    json stats = json::object();
    if (!pokemon.contains("stats")) return stats;

    for (const auto& st : pokemon["stats"]) {
        std::string stat_name = st["stat"]["name"].get<std::string>();
        int base_stat = st["base_stat"].get<int>();
        stats[stat_name] = base_stat;
    }
    return stats;
}

json build_types(const json& pokemon) {
    json types = json::array();
    if (!pokemon.contains("types")) return types;

    for (const auto& t : pokemon["types"]) {
        if (t.contains("type") && t["type"].contains("name")) {
            types.push_back(t["type"]["name"]);
        }
    }
    return types;
}

json build_abilities(const json& pokemon) {
    json abilities = json::array();
    if (!pokemon.contains("abilities")) return abilities;

    for (const auto& ab : pokemon["abilities"]) {
        json a;
        a["name"] = ab["ability"]["name"];
        a["is_hidden"] = ab["is_hidden"];
        a["slot"] = ab["slot"];
        abilities.push_back(a);
    }
    return abilities;
}

// Agrégation des moves par nom, avec min_level_learned + liste de méthodes
json build_moves(const json& pokemon) {
    json moves = json::array();
    if (!pokemon.contains("moves")) return moves;

    struct MoveInfo {
        int min_level = 999;
        std::set<std::string> methods;
    };

    std::map<std::string, MoveInfo> agg;

    for (const auto& mv : pokemon["moves"]) {
        std::string move_name = mv["move"]["name"];

        if (!mv.contains("version_group_details")) continue;
        for (const auto& vgd : mv["version_group_details"]) {
            int lvl = vgd["level_learned_at"].get<int>();
            std::string method = vgd["move_learn_method"]["name"];

            auto& info = agg[move_name];
            if (lvl < info.min_level) {
                info.min_level = lvl;
            }
            info.methods.insert(method);
        }
    }

    for (const auto& [name, info] : agg) {
        json m;
        m["name"] = name;
        m["min_level_learned"] = (info.min_level == 999 ? 0 : info.min_level);

        json methods = json::array();
        for (const auto& method : info.methods) {
            methods.push_back(method);
        }
        m["methods"] = methods;

        moves.push_back(m);
    }

    return moves;
}

std::string get_sprite(const json& pokemon) {
    if (pokemon.contains("sprites") &&
        pokemon["sprites"].contains("front_default") &&
        !pokemon["sprites"]["front_default"].is_null()) {

        return pokemon["sprites"]["front_default"].get<std::string>();
    }
    return "";
}

std::string get_artwork(const json& pokemon) {
    if (pokemon.contains("sprites") &&
        pokemon["sprites"].contains("other") &&
        pokemon["sprites"]["other"].contains("official-artwork") &&
        pokemon["sprites"]["other"]["official-artwork"].contains("front_default") &&
        !pokemon["sprites"]["other"]["official-artwork"]["front_default"].is_null()) {

        return pokemon["sprites"]["other"]["official-artwork"]["front_default"].get<std::string>();
    }
    return "";
}

// ----- main -----

int main() {
    // 1. Lire le gros fichier
    std::ifstream in("pokedex_gen1_8_full.json");
    if (!in) {
        std::cerr << "Impossible d'ouvrir pokedex_gen1_8_full.json\n";
        return 1;
    }

    json big;
    in >> big;
    in.close();

    if (!big.is_array()) {
        std::cerr << "Le fichier n'est pas un tableau JSON.\n";
        return 1;
    }

    json slim = json::array();

    // 2. Parcourir toutes les entrées et construire le slim
    for (const auto& entry : big) {
        if (!entry.contains("full_pokemon") || !entry.contains("full_species")) {
            continue;
        }

        const json& p = entry["full_pokemon"];
        const json& s = entry["full_species"];

        json out = json::object();

        // ----- ORDRE DES CHAMPS -----

        // 1. Infos de base
        out["id"] = p.value("id", 0);
        out["name"] = p.value("name", "");
        out["name_fr"] = get_french_name(s);
        out["base_species"] = entry.value("base_species", out["name"]);
        out["generation"] = entry.value("generation", 0);

        // 2. Types, abilities, stats
        out["types"] = build_types(p);
        out["abilities"] = build_abilities(p);
        out["stats"] = build_stats(p);

        // 3. Description + visuels
        out["pokedex_entry_fr"] = get_french_flavor(s);
        out["sprite"] = get_sprite(p);
        out["artwork"] = get_artwork(p);

        // 4. Moves en dernier
        out["moves"] = build_moves(p);

        slim.push_back(out);
    }

    // 3. Écrire le slim dans un nouveau fichier
    std::ofstream outFile("pokedex_slim.json");
    if (!outFile) {
        std::cerr << "Impossible de créer pokedex_slim.json\n";
        return 1;
    }
    outFile << slim.dump(2);
    outFile.close();

    std::cout << "pokedex_slim.json généré avec " << slim.size() << " entrées.\n";
    return 0;
}
