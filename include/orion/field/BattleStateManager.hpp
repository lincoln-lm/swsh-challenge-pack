#pragma once
#include "hk/types.h"

namespace orion::field {
    struct BattleStateManager {
        enum class BattleType : u32 {
            TRAINER = 1,
            WILD = 3,
        };
        enum class State : u32 {
            BATTLE_END = 7
        };
        u8 unk0[0x68];
        State mState;
        u8 unk1[0xc0];
        BattleType mBattleType;
        // ...
        int Update(void* p1);
    };
}