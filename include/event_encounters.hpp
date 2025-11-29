#pragma once

#include "hook/InlineHook.hpp"
#include "orion/field/encounter/EventEncounterManager.hpp"
#include "rng/RngManager.hpp"
#include <span>
inline auto randomizeEventEncounters = hook::inlineHook([](hook::CpuState* state) {
    // original instruction
    state->X[8] = *pun<u32*>(state->X[20]);
    if (!save::gSaveFile.randomizeEventEncounters) {
        return;
    }
    auto original_encounter = pun<orion::field::encounter::EventEncounter*>(state->X[0]);

    // don't randomize gimmick spawns (here)
    bool is_gimmick = original_encounter->backgroundNearTypeId == 0xD83D0EED33AB2E05 && original_encounter->encounterScenario != orion::field::encounter::EventEncounterScenario::MOTOSTOKE_ENCOUNTER;
    if (is_gimmick) {
        return;
    }

    auto rng = RngManager::NewRandomGenerator(original_encounter->hash);
    auto [species, form] = rng.RandSpeciesAndForm();
    std::array<s16, 4> moves;
    rng.RandMoves(std::span(moves));
    original_encounter->species = species;
    original_encounter->form = form;
    original_encounter->shinyLock = 0;
    original_encounter->nature = 25;
    original_encounter->ability = rng.RandMax(3);
    original_encounter->heldItem = rng.RandHeldItem();
    std::copy(
        moves.begin(),
        moves.end(),
        original_encounter->moves
    );
});

inline void installEventEncountersHooks() {
    randomizeEventEncounters.installAtPtrOffset(pun<ptr>(&orion::field::encounter::EventEncounterManager::UnpackEventEncounterArchive), 0x8E8);
}