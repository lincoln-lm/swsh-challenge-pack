#pragma once
#include "hk/hook/Trampoline.h"
#include "hk/types.h"
#include "orion/field/encounter/GiftEncounterManager.hpp"
#include "orion/field/encounter/GiftEncounter_flatbuffer.h"
#include "rng/RngManager.hpp"
#include "save/SaveFile.hpp"

inline HkTrampoline<orion::field::encounter::flatbuffers::GiftEncounter*, orion::field::encounter::GiftEncounterManager*, u64*> randomizeGiftEncounters = hk::hook::trampoline([](orion::field::encounter::GiftEncounterManager* this_, u64* hash_ptr) {
    auto original_encounter = randomizeGiftEncounters.orig(this_, hash_ptr);
    if (!save::gSaveFile.randomizeGiftEncounters) {
        return original_encounter;
    }
    auto rng = RngManager::NewRandomGenerator(*hash_ptr);
    auto [species, form] = rng.RandSpeciesAndForm();
    auto ball = rng.RandBall();
    auto item = rng.RandHeldItem();
    bool shiny_lock = false;
    auto ability = rng.RandRange(0, 3);
    auto gender = 0; // random
    auto nature = 25; // random
    auto special_move = rng.RandValidMoveId();

    original_encounter->mutate_species(species);
    original_encounter->mutate_form(form);
    original_encounter->mutate_ball(ball);
    original_encounter->mutate_held_item(item);
    original_encounter->mutate_shiny_lock(shiny_lock);
    original_encounter->mutate_ability(ability);
    original_encounter->mutate_gender(gender);
    original_encounter->mutate_nature(nature);
    original_encounter->mutate_special_move(special_move);

    return original_encounter;
});

inline void installGiftEncountersHooks() {
    randomizeGiftEncounters.installAtPtr(pun<void*>(&orion::field::encounter::GiftEncounterManager::GetGift));
}