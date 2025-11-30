#pragma once

#include "hk/hook/Trampoline.h"
#include "hook/InlineHook.hpp"
#include "mod_hooks.hpp"
#include "orion/battle/Party.hpp"
#include "orion/field/Party.hpp"
#include "save/SaveFile.hpp"

inline void eraseFaintedPokemon(orion::battle::BattleParty* party) {
    u8 new_size = 0;
    for (u8 i = 0; i < party->size; i++) {
        auto member = party->Get(i);
        if (member->currentHp == 0) {
            continue;
        };
        party->Set(new_size++, member);
    }
    party->size = new_size;
    for (u8 i = new_size; i < 6; i++) {
        party->Set(i, nullptr);
    }
}

inline HkTrampoline<void, orion::battle::BattleParty*, u8, u8> detectSwap = hk::hook::trampoline([](orion::battle::BattleParty* this_, u8 index1, u8 index2) {
    detectSwap.orig(this_, index1, index2);
    if (!save::gSaveFile.permadeath || !sHooksEnabled) return;
    eraseFaintedPokemon(this_);
});

inline u8 sOutOfBattleCount = 0;

inline HkTrampoline<void, orion::battle::PartyManager*, orion::field::Party**, u8, u8*> fixParty = hk::hook::trampoline([](orion::battle::PartyManager* this_, orion::field::Party** outOfBattlePartyPtr, u8 playerId, u8* touched) {
    if (save::gSaveFile.permadeath && sHooksEnabled) {
        auto party = this_->Get(playerId);
        eraseFaintedPokemon(party);
        sOutOfBattleCount = party->size;
    }
    return fixParty.orig(this_, outOfBattlePartyPtr, playerId, touched);
});

inline auto fixPartyCount = hook::inlineHook([](hook::CpuState* state) {
    // x19 = out of battle party count
    // x21 = battle party count
    // original instruction
    state->X[19] = state->X[21];
    if (save::gSaveFile.permadeath && sHooksEnabled) {
        state->X[21] = sOutOfBattleCount;
    }
});

inline void installPermadeathHooks() {
    detectSwap.installAtPtr(pun<void*>(&orion::battle::BattleParty::SwapMembers));
    fixParty.installAtPtr(pun<void*>(&orion::battle::PartyManager::RestorePartyInOrder));
    fixPartyCount.installAtPtrOffset(pun<ptr>(&orion::battle::PartyManager::RestorePartyInOrder), 0x64);
}