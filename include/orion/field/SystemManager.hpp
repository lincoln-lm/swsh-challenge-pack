#pragma once
#include "hk/types.h"
#include "orion/field/Party.hpp"

namespace orion::field {
    struct SystemData {
        u8 unk0[0x58];
        Party* mParty;
    };
    struct SystemManager {
        u8 unk0[0x90];
        SystemData* mSystemData;
        // TODO: xoroshiro
        void* mRng;

        static SystemManager* sInstance;
        inline static SystemManager* instance() {
            return sInstance;
        }
    };
}