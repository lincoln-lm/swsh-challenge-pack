#pragma once
#include "hk/types.h"
#include "orion/gift/GiftEncounter_flatbuffer.h"
namespace orion::gift {
    struct GiftEncounterManager {
        // TODO
        flatbuffers::GiftEncounter* GetGift(u64* hash_ptr);
    };
}