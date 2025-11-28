#pragma once
#include "hk/types.h"
namespace orion::options {
    enum class TextSpeed {
        Slow = 0,
        Normal = 1,
        Fast = 2,
        Invalid = 3
    };
    struct OptionsHolder {
        u8 unk0[0x60];
        u64 options;
        TextSpeed GetTextSpeed();
    };
}