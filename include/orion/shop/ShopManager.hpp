#pragma once
#include "hk/types.h"
#include "orion/shop/ShopInventory_flatbuffer.h"

namespace orion::shop {
    struct ShopManager {
        u8 unk0[0x60];
        const orion::shop::flatbuffers::ShopInventory* mActiveInventory;
        void SelectShop(u64* hashPtr, u8 index);
    };
}