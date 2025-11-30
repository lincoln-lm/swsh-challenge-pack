#pragma once

#include "hk/hook/Trampoline.h"
#include "hook/InlineHook.hpp"
#include "mod_hooks.hpp"
#include "orion/battle/BattleResult.hpp"
#include "orion/field/AreaLoader.hpp"
#include "orion/field/BattleStateManager.hpp"
#include "orion/field/EventManager.hpp"
#include "orion/field/FieldManager.hpp"
#include "orion/field/FieldObject.hpp"
#include "orion/field/encounter/FishingStateManager.hpp"
#include "save/SaveFile.hpp"
#include "util/Reflection.hpp"
#include "util/Zone.hpp"

inline void filterObject(orion::field::FieldObject* object) {
    if (object == nullptr) return;
    u64 obj_hash = object->uniqueHash;
    u64 zoneHash = util::lookupZone(obj_hash);
    if (std::find(save::gSaveFile.blacklistedZones.value.begin(), save::gSaveFile.blacklistedZones.value.end(), zoneHash) != save::gSaveFile.blacklistedZones.value.end()) {
        orion::field::FieldManager::RequestObjectDeletion(&obj_hash);
    }
}

inline auto filterLazyFieldObject = hook::inlineHook([](hook::CpuState* state) {
    auto new_object = pun<orion::field::FieldObject*>(state->X[0]);
    if (save::gSaveFile.routeRestriction && sHooksEnabled) {
        filterObject(new_object);
    }
    // original instruction
    state->X[8] = pun<u64>(util::getVTable(new_object));
});

inline auto filterInitialFieldObject = hook::inlineHook([](hook::CpuState* state) {
    auto new_object = pun<orion::field::FieldObject*>(state->X[0]);
    if (save::gSaveFile.routeRestriction && sHooksEnabled) {
        filterObject(new_object);
    }
    // original instruction
    state->X[22] += 0x20;
});

inline u64 sEncounterTriggeringObject = 0;

inline HkTrampoline<void, orion::field::encounter::FishingStateManager*> logFishingPoint = hk::hook::trampoline([](orion::field::encounter::FishingStateManager* this_) {
    if (this_->mState == orion::field::encounter::FishingStateManager::State::CAUGHT) {
        sEncounterTriggeringObject = this_->mFishingPoint->uniqueHash;
    }
    logFishingPoint.orig(this_);
});

inline auto logSpawner = hook::InlineHook([](hook::CpuState* state) {
    auto encount_object = pun<orion::field::EncountObject*>(state->X[23] - 0x50);
    sEncounterTriggeringObject = encount_object->spawnerHash;
    // original instruction
    state->X[8] = *pun<u64*>(&encount_object->overworldSpec.species);
});

inline HkTrampoline<int, orion::field::BattleStateManager*, void*> addBlacklistedRoute = hk::hook::trampoline([](orion::field::BattleStateManager* this_, void* p1) {
    if (save::gSaveFile.routeRestriction && sHooksEnabled) {
        if (this_->mState == orion::field::BattleStateManager::State::BATTLE_END) {
            if (this_->mBattleType == orion::field::BattleStateManager::BattleType::WILD) {
                if (orion::battle::sBattleResult->mBattleOutcome == orion::battle::BattleResult::BattleOutcome::Capture
                    || orion::battle::sBattleResult->mBattleOutcome == orion::battle::BattleResult::BattleOutcome::Win) {
                    if (sEncounterTriggeringObject != 0) {
                        save::gSaveFile.blacklistedZones.value.push_back(util::lookupZone(sEncounterTriggeringObject));
                        sEncounterTriggeringObject = 0;
                    }
                }
            }
        }
    }
    return addBlacklistedRoute.orig(this_, p1);
});

inline void installRouteRestrictionHooks() {
    filterLazyFieldObject.installAtPtrOffset(pun<ptr>(&orion::field::FieldManager::UpdateLazyObjects), 0xa0);
    filterInitialFieldObject.installAtPtrOffset(pun<ptr>(&orion::field::AreaLoader::PlaceObjects), 0x114);
    logFishingPoint.installAtPtr(pun<void*>(&orion::field::encounter::FishingStateManager::Update));
    logSpawner.installAtPtrOffset(pun<ptr>(&orion::field::EventManager::CheckEncounter), 0x140);
    addBlacklistedRoute.installAtPtr(pun<void*>(&orion::field::BattleStateManager::Update));
}