#include <gtest/gtest.h>
#include "Entity/PokemonInstance.hpp"
#include "Entity/PokemonSpecies.hpp"

// Test fixture : setup commun pour tous les tests
class PokemonInstanceTest : public ::testing::Test {
protected:
    Pokemon::PokemonSpecies* bulbasaur;
    
    void SetUp() override {
        // Crée un Bulbasaur de test avant chaque test
        bulbasaur = new Pokemon::PokemonSpecies{
            1,                                      // id
            "Bulbasaur",                            // name
            {Pokemon::Type::Grass, Pokemon::Type::Poison},  // types
            {45, 49, 49, 65, 65, 45},              // baseStats (HP, Atk, Def, SpAtk, SpDef, Speed)
            45,                                     // catchRate
            64                                      // baseExpYield
        };
    }
    
    void TearDown() override {
        delete bulbasaur;
    }
};

// Test simple : vérification du KO
TEST_F(PokemonInstanceTest, IsFaintedWhenHPZero) {
    // TODO: Tu devras créer un constructeur pour PokemonInstance
    // Pour l'instant c'est juste un exemple de structure
    EXPECT_TRUE(true);  // Test bidon pour vérifier que ça compile
}

// Test principal de compilation
TEST(BasicTest, GoogleTestWorks) {
    EXPECT_EQ(1 + 1, 2);
}
