#pragma once

#include "hk/types.h"
#include "orion/field/Party.hpp"

namespace orion::battle {
    struct BattlePartyMember {
        u8 unk0[0x74];
        u16 currentHp;
        u8 unk1[0x7];
        u8 id;
        u8 friendship;
        bool unk2;
    };
    struct BattleParty {
        BattlePartyMember* members[6];
        u8 size;
        void SwapMembers(u64 index1, u64 index2);
        BattlePartyMember* Get(u64 index);
        inline void Set(u64 index, BattlePartyMember* memberPtr) { members[index] = memberPtr; }
    };
    struct PartyManager {
        u64 unk0;
        BattleParty parties[5];
        // ...
        void RestoreParty(field::Party** out, u8 playerId);
        void RestorePartyInOrder(field::Party** out, u8 playerId, u8* touched);
        inline BattleParty* Get(u64 playerId) { return &parties[playerId]; };
    };
}