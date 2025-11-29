#pragma once
#include "hk/types.h"
#include <algorithm>

namespace util {
    // TODO: multiple curves?
    inline s32 levelBoost(s32 level) {
        // 0.02x^2 + 0.5x + 5;
        return std::clamp(0.02f * level * level + 0.5f * level + 5.0f, 0.0f, 100.0f);
    }
}