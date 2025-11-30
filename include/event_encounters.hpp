#pragma once

#include "hk/hook/Trampoline.h"
#include "mod_hooks.hpp"
#include "orion/field/FieldObject.hpp"
#include "orion/field/encounter/EventEncounterManager.hpp"
#include "rng/RngManager.hpp"
#include <span>

inline bool sIsGimmickSpawnerInit = false;

inline HkTrampoline<bool, orion::field::GimmickEncountSpawner*, void*, void*, void*, void*, void*, void*> logGimmick = hk::hook::trampoline([](orion::field::GimmickEncountSpawner* this_, void* p1, void* p2, void* p3, void* p4, void* p5, void* p6) {
    sIsGimmickSpawnerInit = true;
    bool result = logGimmick.orig(this_, p1, p2, p3, p4, p5, p6);
    sIsGimmickSpawnerInit = false;
    return result;
});

inline HkTrampoline<orion::field::encounter::EventEncounter*, orion::field::encounter::EventEncounterManager*, u64*> randomizeEventEncounters = hk::hook::trampoline([](orion::field::encounter::EventEncounterManager* this_, u64* hashPtr) {
    auto original_encounter = randomizeEventEncounters.orig(this_, hashPtr);
    // don't randomize gimmick spawns (here)
    
    if (sIsGimmickSpawnerInit || !sHooksEnabled || !save::gSaveFile.randomizeEventEncounters) {
        return original_encounter;
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
    return original_encounter;
});

inline void installEventEncountersHooks() {
    randomizeEventEncounters.installAtPtr(pun<void*>(&orion::field::encounter::EventEncounterManager::GetEvent));
}