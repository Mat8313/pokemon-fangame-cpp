# Pokémon Development Roadmap - Implementation Plan

## Phase 1: Data Models & Core Logic (High Priority)
- [ ] **1.1 Types & Constants**
    - Create `PokemonType` enum (15 types Gen 1).
    - Define `Stat` enum (HP, Atk, Def, SpAtk, SpDef, Spe).
    - Define `StatusCondition` and `DamageCategory`.
- [ ] **1.2 Base Structures**
    - Implement `BaseStats` struct.
    - Implement `Species` class (dex number, name, types, stats, growth rate, learnset).
- [ ] **1.3 Move Model**
    - Create `Move` class (id, name, type, category, power, accuracy, PP).
- [ ] **1.4 Pokémon Instance**
    - Create `PokemonInstance` class.
    - Implement IVs/EVs structs.
    - Implement stat calculation formulas (official).

## Phase 2: Data Management (JSON Implementation)
- [ ] **2.1 Resource Manager**
    - Create `PokemonDatabase` to store global data.
- [ ] **2.2 JSON Loaders**
    - Implement `species.json` and `moves.json` parsing.
    - Validate data loading in debug mode.

## Phase 3: Systems Integration
- [ ] **3.1 Party & Storage**
    - Create `Party` class (max 6 slots).
    - Implement `PCStorage` for overflow Pokémon.
- [ ] **3.2 Save System Hook**
    - Integrate `PokemonInstance` serialization into existing JSON save system.

## Phase 4: World Interaction
- [ ] **4.1 Wild Encounters**
    - Add encounter tables to maps.
    - Implement `generateWildPokemon()` logic (random level/IVs).
- [ ] **4.2 Basic Battle HUD (Debug)**
    - Create a minimal overlay to display team stats and HP.

## Phase 5: Battle Engine (Future)
- [ ] **5.1 Damage Calculator**
    - Implement official damage formula (with STAB & Type multipliers).
- [ ] **5.2 Turn Loop**
    - Create basic 1v1 battle state logic.
