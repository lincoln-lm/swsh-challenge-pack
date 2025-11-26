#pragma once

#include "hk/types.h"
#include "orion/filesystem/GFFile.hpp"

namespace orion::movie {
    struct BSeqHeader {
        u32 unk0;
        u32 unk1;
        u32 unk2;
        u32 frameCount;
        u32 groupOptionCount;
        u32 hashSizeCount;
        // ...
    };
    struct BSeqHandler {
        u8 unk0[0x68];
        filesystem::GFFile* gfFile;
        u8 unk1[0x178-0x8-0x68];
        BSeqHeader* bSeqHeader;
        void Deserialize(u64 param_1);
    };
    static_assert(offsetof(BSeqHandler, gfFile) == 0x68);
    static_assert(offsetof(BSeqHandler, bSeqHeader) == 0x178);
}