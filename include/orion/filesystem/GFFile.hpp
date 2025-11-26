#pragma once
#include "hk/types.h"
namespace orion::filesystem {
    struct GFFile {
        u8 unk0[0x68 + 0x8];
        const char* filePath;
        // ...
    };
    static_assert(offsetof(GFFile, filePath) == 0x68 + 0x8);
}