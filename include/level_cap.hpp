#pragma once

#include "hk/hook/Trampoline.h"
#include "mod_hooks.hpp"
#include "orion/capture/LevelCap.hpp"
#include "orion/save/MiscBlock.hpp"
#include "save/SaveFile.hpp"
#include "util/LevelBoost.hpp"

inline HkTrampoline<u32> boostLevelCap1 = hk::hook::trampoline([]() -> u32 {
    u32 level_cap = boostLevelCap1.orig();
    if (save::gSaveFile.boostLevelCap && sHooksEnabled) {
        return util::levelBoost(level_cap);
    }
    return level_cap;
});

inline HkTrampoline<u32, orion::save::MiscBlock*> boostLevelCap2 = hk::hook::trampoline([](orion::save::MiscBlock* miscBlock) -> u32 {
    u32 level_cap = boostLevelCap2.orig(miscBlock);
    if (save::gSaveFile.boostLevelCap && sHooksEnabled) {
        return util::levelBoost(level_cap);
    }
    return level_cap;
});

inline void installLevelCapHooks() {
    boostLevelCap1.installAtPtr(pun<void*, u32(*)()>(&orion::capture::GetLevelCap));
    boostLevelCap1.installAtPtr(pun<void*, u32(*)(orion::save::MiscBlock*)>(&orion::capture::GetLevelCap));
}