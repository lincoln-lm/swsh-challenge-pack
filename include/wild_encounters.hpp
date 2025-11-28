#pragma once

#include "hk/hook/Trampoline.h"
#include "hook/InlineHook.hpp"
#include "orion/field/FieldObject.hpp"
#include "orion/field/encounter/OverworldEncounterManager.hpp"
#include "rng/RngManager.hpp"
#include "save/SaveFile.hpp"
#include <format>

inline auto randomizeGimmickEncounters = hook::inlineHook([](hook::CpuState* state) {
    auto gimmick_encount_spawner = pun<orion::field::GimmickEncountSpawner*>(state->X[19]);
    auto overworld_encounter_manager = pun<orion::field::encounter::OverworldEncounterManager*>(state->X[0]);
    auto gimmick_spec = pun<orion::field::encounter::GimmickSpec*>(state->X[1]);
    auto overworld_spec_out = pun<orion::field::encounter::OverworldSpec*>(state->X[2]);
    
    if (save::gSaveFile.randomizeWildEncounters) {
        auto weather = orion::field::encounter::GetCurrentWeather();
        u64 gimmick_spawner_hash = gimmick_encount_spawner->uniqueHash;
        const std::string seed = std::format(
            "gimmick_spawner_{}_{}",
            gimmick_spawner_hash,
            (s32)weather
        );
        MersenneTwister rng;
        if (save::gSaveFile.liveRandomizeWildEncounters) {
            rng = RngManager::NewRandomGenerator();
        } else {
            rng = RngManager::NewRandomGenerator(seed);
        }
        auto [species, form] = rng.RandSpeciesAndForm();
        gimmick_spec->species = species;
        gimmick_spec->form = form;
        // TODO: level boosts
        // if (save::gSaveFile.wildLevelBoost) {
        //     gimmick_spec->level = level_boost(gimmick_spec->level);
        // }
        gimmick_spec->ability = rng.RandMax(3);
        gimmick_spec->heldItem = rng.RandHeldItem();
        std::array<s16, 4> moves;
        rng.RandMoves(std::span(moves));
        std::copy(
            moves.begin(),
            moves.end(),
            gimmick_spec->moves
        );
    }

    // original instruction
    state->X[0] = overworld_encounter_manager->GenerateGimmick(gimmick_spec, overworld_spec_out);
});

inline auto randomizeSymbolEncounters = hook::inlineHook([](hook::CpuState* state) {
    auto data = pun<orion::field::encounter::SymbolEncounterAreaTables*>(state->X[19]);
    // original instruction
    data->isValid = true;
    if (!save::gSaveFile.randomizeWildEncounters) {
        return;
    }
    const std::string seed = std::format("symbol_encounter_table_{}", data->hash);
    auto rng = RngManager::NewRandomGenerator(seed);
    for (int weather = 0; weather < 9; weather++) {
        // TODO: level boosts
        // if (save::gSaveFile.wildLevelBoost) {
        //     encounter_tables[weather].minimum_level = level_boost(encounter_tables[weather].minimum_level);
        //     encounter_tables[weather].maximum_level = level_boost(encounter_tables[weather].maximum_level);
        // }
        for (int i = 0; i < 10; i++) {
            auto slot = &(data->tables[weather].slots[i]);
            slot->rate = 10;
            auto [species, form] = rng.RandSpeciesAndForm();
            slot->species = species;
            slot->form = form;
        }
    }
});

inline auto randomizeHiddenEncounters = hook::inlineHook([](hook::CpuState* state) {
    auto data = pun<orion::field::encounter::HiddenEncounterAreaTables*>(state->X[19]);
    // original instruction
    data->isValid = true;
    if (!save::gSaveFile.randomizeWildEncounters) {
        return;
    }
    const std::string seed = std::format("hidden_encounter_table_{}", data->hash);
    auto rng = RngManager::NewRandomGenerator(seed);
    for (int weather = 0; weather < 11; weather++) {
        // TODO: level boosts
        // if (save::gSaveFile.wildLevelBoost) {
        //     encounter_tables[weather].minimum_level = level_boost(encounter_tables[weather].minimum_level);
        //     encounter_tables[weather].maximum_level = level_boost(encounter_tables[weather].maximum_level);
        // }
        for (int i = 0; i < 10; i++) {
            auto slot = &(data->tables[weather].slots[i]);
            slot->rate = 10;
            auto [species, form] = rng.RandSpeciesAndForm();
            slot->species = species;
            slot->form = form;
        }
    }
});

inline HkTrampoline<void, orion::field::encounter::EncounterGenerator*, orion::field::encounter::OverworldSpec*, orion::field::encounter::EncounterSlot*, s32, s32, void*> liveRandomizeSlotSpawns = hk::hook::trampoline([](orion::field::encounter::EncounterGenerator* this_, orion::field::encounter::OverworldSpec* spec, orion::field::encounter::EncounterSlot* slot, s32 minLevel, s32 maxLevel, void* flags) {
    if (save::gSaveFile.liveRandomizeWildEncounters) {
        auto rng = RngManager::NewRandomGenerator();
        auto [species, form] = rng.RandSpeciesAndForm();
        // TODO: level boosts
        // if (save::gSaveFile.wildLevelBoost) {
        //     minimum_level = level_boost(minimum_level);
        //     maximum_level = level_boost(maximum_level);
        // }
        slot->species = species;
        slot->form = form;
    }
    liveRandomizeSlotSpawns.orig(this_, spec, slot, minLevel, maxLevel, flags);
});

inline void installWildEncountersHooks() {
    randomizeGimmickEncounters.installAtPtrOffset(pun<ptr>(&orion::field::GimmickEncountSpawner::Spawn), 0x318);
    randomizeSymbolEncounters.installAtPtrOffset(pun<ptr>(&orion::field::encounter::OverworldEncounterManager::FetchSymbolEncounterTable), 0x114);
    randomizeHiddenEncounters.installAtPtrOffset(pun<ptr>(&orion::field::encounter::OverworldEncounterManager::FetchHiddenEncounterTable), 0x1f8);
    liveRandomizeSlotSpawns.installAtPtr(pun<void*>(&orion::field::encounter::EncounterGenerator::GenerateBasicSpec));
}