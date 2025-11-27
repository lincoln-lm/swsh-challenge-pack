#pragma once
#include "hk/types.h"
namespace orion::battle {
    struct Move {
        u64 unk0;
        u32 moveId;
        bool isUsable();
    };
}