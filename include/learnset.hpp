#pragma once

#include "hk/hook/Trampoline.h"
#include "orion/learn/Data.hpp"
#include "rng/RngManager.hpp"
#include "save/SaveFile.hpp"

inline HkTrampoline<void, orion::learn::Learnset*, s32, s32> randomizeLearnsets = hk::hook::trampoline([](orion::learn::Learnset* out, s32 species, s32 form) {
    if (!save::gSaveFile.randomizeLearnsets) {
        randomizeLearnsets.orig(out, species, form);
        return;
    }
    const std::string seed = std::format("learnset_{}_{}", species, form);
    auto rng = RngManager::NewRandomGenerator(seed);
    u64 species_form_index = orion::personal::GetSpeciesFormIndex(species, form);
    auto vanilla_items = orion::learn::sTotalLearnsetData[species_form_index];
    std::memcpy(out->learnsetItems, vanilla_items, sizeof(orion::learn::LearnsetItem) * 65);
    out->count = 0;
    int level_1_move_count = 0;
    for (int i = 0; i < 65; i++) {
        if (out->learnsetItems[i].moveId == -1 && out->learnsetItems[i].level == -1) {
            break;
        }
        if (out->learnsetItems[i].level == 1) {
            level_1_move_count++;
        }
        // replace existing moves
        out->learnsetItems[i].moveId = rng.RandValidMoveId();
        out->count++;
    }
    if (out->count) {
        // ensure at least 4 moves at level 1 (if possible)
        if (out->count < 65 - 3 && level_1_move_count < 4) {
            for (; level_1_move_count < 4; level_1_move_count++) {
                out->learnsetItems[out->count].moveId = rng.RandValidMoveId();
                out->learnsetItems[out->count].level = 1;
                out->count++;
            }
        }
    }
});

inline void installLearnsetHooks() {
    randomizeLearnsets.installAtPtr(pun<void*>(&orion::learn::GetLearnsetData));
}