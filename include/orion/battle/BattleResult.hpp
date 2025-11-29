#pragma once
#include "hk/types.h"

namespace orion::battle {
    struct BattleResult {
        enum class BattleOutcome : u32 {
            Loss,
            Win,
            Unk2,
            Flee,
            Unk4,
            Capture,
            Unk6,
        };
        u8 unk[0xb38];
        BattleOutcome mBattleOutcome;
        // ...
    };
    extern BattleResult* sBattleResult;
}