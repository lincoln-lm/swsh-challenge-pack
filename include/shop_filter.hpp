#pragma once

#include "hk/hook/Trampoline.h"
#include "mod_hooks.hpp"
#include "orion/shop/ShopManager.hpp"
#include "save/SaveFile.hpp"
#include "util/Data.hpp"


// modifying in place is problematic so we must allocate our own flatbuffer to replace the game's.
// the assumption is made that the game only tries to access one shop at a time.
inline std::vector<u8> sLastSelectedInventory;

inline HkTrampoline<void, orion::shop::ShopManager*, u64*, u8> shopFilter = hk::hook::trampoline([](orion::shop::ShopManager* this_, u64* hash_ptr, u8 index) {
    shopFilter.orig(this_, hash_ptr, index);
    if (!save::gSaveFile.filterShop || !sHooksEnabled) {
        return;
    }
    auto original_inventory = this_->mActiveInventory;
    if (original_inventory == nullptr) return;

    std::vector<int> items;

    for (u32 i = 0; i < original_inventory->items()->size(); i++) {
        auto item = original_inventory->items()->Get(i);
        if (std::find(util::data::POKEBALLS.begin(), util::data::POKEBALLS.end(), item) != util::data::POKEBALLS.end()) {
            items.push_back(item);
        }
    }
    // empty shops sell a useless rare bone
    if (items.size() == 0) {
        items.push_back(106);
    }
    flatbuffers::FlatBufferBuilder builder;
        builder.Finish(
            orion::shop::flatbuffers::CreateShopInventory(
                builder,
                builder.CreateVector(items.data(), items.size())));
        u8* data_ptr = builder.GetBufferPointer();
        size_t data_size = builder.GetSize();
        sLastSelectedInventory.clear();
        sLastSelectedInventory.insert(sLastSelectedInventory.end(), data_ptr, data_ptr + data_size);

        this_->mActiveInventory = flatbuffers::GetRoot<orion::shop::flatbuffers::ShopInventory>(sLastSelectedInventory.data());
});

inline void installShopFilterHooks() {
    shopFilter.installAtPtr(pun<void*>(&orion::shop::ShopManager::SelectShop));
}