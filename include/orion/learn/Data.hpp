#pragma once
#include "hk/types.h"

namespace orion::learn {
    struct LearnsetItem {
        s16 moveId;
        s16 level;
    } __attribute__((packed));
    struct Learnset {
        u64 unk;
        u32 species;
        u16 form;
        LearnsetItem learnsetItems[65];
        u8 count;
    } __attribute__((packed));
    extern LearnsetItem (*sTotalLearnsetData)[65];
    void GetLearnsetData(Learnset* out, s32 species, s32 form);
}