#include <gtest/gtest.h>

#include "../include/Entity/Move.hpp"
#include "../include/Entity/PokemonSpecies.hpp"
#include "../include/Entity/PokemonInstance.hpp"
#include "../include/Entity/Types.hpp"

// Helpers simples pour les stats
static Pokemon::PokemonSpecies makeTestSpecies() {
    Pokemon::PokemonSpecies s;
    s.name = "Testmon";
    s.types[0] = Pokemon::Type::Normal;
    s.types[1] = Pokemon::Type::Normal;

    // Base stats simples : 50 partout
    s.baseStats[static_cast<int>(Pokemon::Stat::HP)]    = 50;
    s.baseStats[static_cast<int>(Pokemon::Stat::Atk)]  = 50;
    s.baseStats[static_cast<int>(Pokemon::Stat::Def)] = 50;
    s.baseStats[static_cast<int>(Pokemon::Stat::SpAtk)] = 50;
    s.baseStats[static_cast<int>(Pokemon::Stat::SpDef)] = 50;
    s.baseStats[static_cast<int>(Pokemon::Stat::Speed)]    = 50;

    // Growth rate basique si tu as un champ pour ça (sinon ignore)
    // s.growthRate = Pokemon::GrowthRate::MediumFast;

    return s;
}

static Pokemon::Move makeTestMove() {
    Pokemon::Move m;
    m.id = 1;
    m.name = "Tackle";
    m.type = Pokemon::Type::Normal;
    m.category = Pokemon::DamageCategory::Physical;
    m.power = 40;
    m.accuracy = 100;
    m.maxPP = 35;
    m.priority = 0;
    m.effectChance = 0;
    m.description = "Basic physical attack.";
    return m;
}

// ---------- Tests sur Move / LearnedMove ----------

TEST(MoveTest, IsDamagingAndCanMiss) {
    Pokemon::Move dmgMove = makeTestMove();
    EXPECT_TRUE(dmgMove.isDamaging());
    EXPECT_FALSE(dmgMove.canMiss()); // 100% accuracy => ne "peut pas rater"

    Pokemon::Move statusMove = makeTestMove();
    statusMove.power = 0;
    EXPECT_FALSE(statusMove.isDamaging()); // 0 power => pas de dégâts

    Pokemon::Move lowAccMove = makeTestMove();
    lowAccMove.accuracy = 75;
    EXPECT_TRUE(lowAccMove.canMiss()); // < 100 => peut rater

    Pokemon::Move neverMissMove = makeTestMove();
    neverMissMove.accuracy = -1;
    EXPECT_FALSE(neverMissMove.canMiss()); // -1 => ne rate jamais
}

TEST(LearnedMoveTest, DefaultAndCustomConstructor) {
    Pokemon::Move baseMove = makeTestMove();

    Pokemon::LearnedMove lmDefault;
    EXPECT_EQ(lmDefault.currentPP, 0); // ctor par défaut

    Pokemon::LearnedMove lm(baseMove);
    EXPECT_EQ(lm.currentPP, baseMove.maxPP); // ctor avec Move
    EXPECT_EQ(lm.move.name, baseMove.name);
}

// ---------- Tests sur PokemonInstance ----------

TEST(PokemonInstanceTest, ConstructorAndGetters) {
    static Pokemon::PokemonSpecies species = makeTestSpecies();

    std::array<int, 6> ivs = {31, 30, 29, 28, 27, 26};
    std::array<int, 6> evs = {0, 10, 20, 30, 40, 50};

    PokemonInstance p(
        &species,
        10,                         // level
        Pokemon::Nature::Hardy,     // neutre
        ivs,
        evs
    );

    EXPECT_EQ(p.getSpecies(), &species);
    EXPECT_EQ(p.getLvl(), 10);
    EXPECT_EQ(p.getExp(), 0);      // à adapter si tu initialises autrement
    EXPECT_GT(p.getHp(), 0);       // doit avoir des PV positifs

    EXPECT_EQ(p.getIV(Pokemon::Stat::HP), 31);
    EXPECT_EQ(p.getEV(Pokemon::Stat::Speed), 50);
    EXPECT_EQ(p.getNature(), Pokemon::Nature::Hardy);
}

TEST(PokemonInstanceTest, TakeDamageAndHeal) {
    static Pokemon::PokemonSpecies species = makeTestSpecies();

    PokemonInstance p(
        &species,
        10,
        Pokemon::Nature::Hardy
    );

    int maxHp = p.getMaxHP();
    EXPECT_EQ(p.getHp(), maxHp);

    p.takeDamage(10);
    EXPECT_EQ(p.getHp(), maxHp - 10);
    EXPECT_FALSE(p.isFainted());

    p.heal(5);
    EXPECT_EQ(p.getHp(), maxHp - 5);

    // Heal au-dessus du max ne doit pas dépasser
    p.heal(999);
    EXPECT_EQ(p.getHp(), maxHp);

    // Dégâts létaux => KO mais pas négatif
    p.takeDamage(9999);
    EXPECT_EQ(p.getHp(), 0);
    EXPECT_TRUE(p.isFainted());
}

TEST(PokemonInstanceTest, CalculateStatsWithNature) {
    static Pokemon::PokemonSpecies species = makeTestSpecies();

    PokemonInstance neutral(
        &species,
        50,
        Pokemon::Nature::Hardy // neutre
    );

    int atkNeutral = neutral.calculateStat(Pokemon::Stat::Atk);
    int defNeutral = neutral.calculateStat(Pokemon::Stat::Def);

    PokemonInstance atkUpDefDown(
        &species,
        50,
        Pokemon::Nature::Adamant // +Atk -SpAtk normalement, adapte à ton implé
    );

    int atkAdamant = atkUpDefDown.calculateStat(Pokemon::Stat::Atk);
    int spAtkAdamant = atkUpDefDown.calculateStat(Pokemon::Stat::SpAtk);

    EXPECT_GT(atkAdamant, atkNeutral);     // nature +Atk => attaque plus haute
    EXPECT_LT(spAtkAdamant,
              neutral.calculateStat(Pokemon::Stat::SpAtk)); // -SpAtk
}

TEST(PokemonInstanceTest, GainExpAndLevelUp) {
    static Pokemon::PokemonSpecies species = makeTestSpecies();

    PokemonInstance p(
        &species,
        5,
        Pokemon::Nature::Hardy
    );

    int initialLevel = p.getLvl();
    int initialMaxHp = p.getMaxHP();

    int xpNext = p.getExpForNextLevel();
    EXPECT_GT(xpNext, 0);

    p.gainExp(xpNext); // devrait au moins passer 1 niveau

    EXPECT_GT(p.getLvl(), initialLevel);
    EXPECT_EQ(p.getHp(), p.getMaxHP()); // si tu soignes au level up
    EXPECT_GT(p.getMaxHP(), initialMaxHp); // stats augmentent avec le niveau
}

