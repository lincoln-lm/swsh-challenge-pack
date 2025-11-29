#pragma once
#include "hk/types.h"

namespace orion::save {
    struct MiscBlock {
        u8 unk0[0x60];
        u32 badges;
        // ...
        u32 GetBadgeCount();
    };
}