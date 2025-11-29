#pragma once
#include "hk/types.h"
#include "orion/field/encounter/GiftEncounter_flatbuffer.h"
namespace orion::field::encounter {
    struct GiftEncounterManager {
        // TODO
        flatbuffers::GiftEncounter* GetGift(u64* hashPtr);
    };
}