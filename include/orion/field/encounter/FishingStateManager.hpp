#pragma once
#include "hk/types.h"
#include "orion/field/FieldObject.hpp"

namespace orion::field::encounter {
    struct FishingStateManager {
        enum class State : u32 {
            CAUGHT = 7,
        };
        u8 unk0[0xc0];
        State mState;
        u32 unk1[4];
        FishingPoint* mFishingPoint;
        // ...
        void Update();
    };
}