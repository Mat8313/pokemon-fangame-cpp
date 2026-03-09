#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <stdexcept>

#include <curl/curl.h>
#include "../include/nlohmann/json.hpp" // nlohmann::json : https://github.com/nlohmann/json

using json = nlohmann::json;

// callback pour écrire la réponse HTTP dans une std::string
static size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp) {
    size_t totalSize = size * nmemb;
    std::string* s = static_cast<std::string*>(userp);
    s->append(static_cast<char*>(contents), totalSize);
    return totalSize;
}

// GET simple qui renvoie le body, et throw si code != 200
std::string http_get(const std::string& url) {
    CURL* curl = curl_easy_init();
    if (!curl) {
        throw std::runtime_error("curl_easy_init failed");
    }

    std::string response;
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
    curl_easy_setopt(curl, CURLOPT_USERAGENT, "pokecpp/1.0");

    CURLcode res = curl_easy_perform(curl);
    if (res != CURLE_OK) {
        std::string msg = curl_easy_strerror(res);
        curl_easy_cleanup(curl);
        throw std::runtime_error("curl_easy_perform failed: " + msg);
    }

    long code = 0;
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &code);
    curl_easy_cleanup(curl);

    if (code != 200) {
        throw std::runtime_error("HTTP error code: " + std::to_string(code) + " for URL " + url);
    }

    return response;
}

int main() {
    curl_global_init(CURL_GLOBAL_DEFAULT);
    std::vector<json> pokedex;

    try {
        // Générations 1 à 8
        for (int gen = 1; gen <= 8; ++gen) {
            std::cout << "Generation " << gen << "...\n";

            // Endpoint generation/{id} → liste des pokemon_species [web:15]
            std::string genUrl = "https://pokeapi.co/api/v2/generation/" + std::to_string(gen) + "/";
            std::string genBody = http_get(genUrl);
            json genJson = json::parse(genBody);

            for (const auto& species : genJson["pokemon_species"]) {
                std::string baseName   = species["name"];  // ex: "deoxys"
                std::string speciesUrl = species["url"];   // ex: .../pokemon-species/386/
                std::cout << "  Species -> " << baseName << std::endl;

                // 1) Récupérer la species complète (jamais 404) [web:55]
                std::string speciesBody = http_get(speciesUrl);
                json speciesJson = json::parse(speciesBody);
                int speciesId = speciesJson["id"];

                // 2) TENTER la forme "principale" /pokemon/{baseName}
                bool main_done = false;
                try {
                    std::string pokemonUrl = "https://pokeapi.co/api/v2/pokemon/" + baseName + "/";
                    std::string pokemonBody = http_get(pokemonUrl);
                    json pokemonJson = json::parse(pokemonBody);

                    json entry;
                    entry["id"] = speciesId;          // id de la species
                    entry["name"] = baseName;         // nom de base
                    entry["is_form"] = false;         // forme principale
                    entry["base_species"] = baseName; // pour homogénéité
                    entry["generation"] = gen;
                    entry["full_pokemon"] = pokemonJson;
                    entry["full_species"] = speciesJson;

                    pokedex.push_back(entry);
                    main_done = true;

                    std::cout << "    -> forme principale OK: " << baseName << std::endl;
                } catch (const std::exception& e) {
                    std::cout << "    -> /pokemon/" << baseName << " introuvable, pas de forme principale directe\n";
                }

                // 3) TOUTES les varieties (formes) depuis pokemon-species.varieties [web:45][web:53]
                for (const auto& variety : speciesJson["varieties"]) {
                    std::string varietyName = variety["pokemon"]["name"]; // ex: "deoxys-attack"
                    std::string varietyUrl  = variety["pokemon"]["url"];

                    // si déjà traitée comme forme principale, on peut la skipper
                    if (varietyName == baseName && main_done) {
                        continue;
                    }

                    try {
                        std::string varietyBody = http_get(varietyUrl);
                        json varietyJson = json::parse(varietyBody);

                        json entry;
                        entry["id"] = speciesId;            // même species id
                        entry["name"] = varietyName;        // nom complet de la forme
                        entry["is_form"] = (varietyName != baseName);
                        entry["base_species"] = baseName;   // toujours l'espèce d'origine
                        entry["generation"] = gen;
                        entry["full_pokemon"] = varietyJson;
                        entry["full_species"] = speciesJson;

                        pokedex.push_back(entry);
                        std::cout << "    -> forme ajoutee: " << varietyName << std::endl;
                    } catch (const std::exception& e) {
                        std::cout << "    -> forme " << varietyName << " introuvable, skip\n";
                    }
                }
            }
        }

        // 4) Écrire le fichier final
        json out = pokedex;
        std::ofstream file("pokedex_gen1_8_full.json");
        file << out.dump(2);  // indentation 2
        file.close();

        std::cout << "\n✅ Fichier pokedex_gen1_8_full.json genere: "
                  << pokedex.size() << " entrees (formes incluses)." << std::endl;

    } catch (const std::exception& e) {
        std::cerr << "Erreur fatale: " << e.what() << std::endl;
    }

    curl_global_cleanup();
    return 0;
}
