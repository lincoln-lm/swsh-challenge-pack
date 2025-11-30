#pragma once

#include "hk/types.h"
#include "hook/InlineHook.hpp"
#include "mod_hooks.hpp"
#include "orion/field/EventManager.hpp"
#include "orion/field/FieldObject.hpp"
#include "rng/RngManager.hpp"
#include "save/SaveFile.hpp"
#include <format>

inline auto randomizeBallItem = hook::inlineHook([](hook::CpuState* state) {
    // original instruction
    state->X[5] = 0;
    if (!save::gSaveFile.randomizeFieldItems || !sHooksEnabled) {
        return;
    }

    u64& item = state->X[2];

    u64 ball_item_hash = state->X[1];
    const std::string seed = std::format("ball_item_{}_{}", ball_item_hash, 0);
    auto rng = RngManager::NewRandomGenerator(seed);
    if (std::find(util::data::VALID_TMS.begin(), util::data::VALID_TMS.end(), item) == util::data::VALID_TMS.end()) {
        item = rng.RandHeldItem();
    } else {
        item = rng.RandTM();
    }
});

inline auto randomizeSparkleItem = hook::inlineHook([](hook::CpuState* state) {
    // original instruction
    state->X[5] = 0;
    if (!save::gSaveFile.randomizeFieldItems || !sHooksEnabled) {
        return;
    }

    u64& item = state->X[2];

    auto sparkle_item = pun<orion::field::FieldSparkleItem*>(state->X[26]);
    u64 sparkle_item_hash = state->X[1];
    u8 item_value = *pun<u8*>(state->X[19]);
    u8 item_idx = 0;
    for (u8 i = 0; i < 8; i++) {
        if (i < 7 && sparkle_item->itemThresholds[i] < item_value) continue;
        item_idx = i;
    }
    const std::string seed = std::format("sparkle_item_{}_{}", sparkle_item_hash, item_idx);
    auto rng = RngManager::NewRandomGenerator(seed);
    item = rng.RandHeldItem();
});

inline void installFieldItemsHooks() {
    randomizeBallItem.installAtPtrOffset(pun<ptr>(&orion::field::EventManager::OnInteract), 0x160);
    randomizeSparkleItem.installAtPtrOffset(pun<ptr>(&orion::field::EventManager::OnInteract), 0x32c);
}