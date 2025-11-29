#pragma once
#include "hk/types.h"

namespace orion::field {
    struct Party {
        u8 unk0[0x58];
        void* members[6];
        u8 size;
    };
}