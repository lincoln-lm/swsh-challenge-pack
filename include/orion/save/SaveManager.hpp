#pragma once
#include "hk/types.h"
#include "orion/save/MyItem.hpp"

namespace orion::save {
    struct SaveManager {
        u8 unk0[0x1d8];
        MyItem* mMyItem;
        // ...
        static SaveManager* sInstance;
        inline static SaveManager* instance() { return sInstance; }
    };
}