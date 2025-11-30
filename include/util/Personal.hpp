#pragma once
#include "hk/types.h"
#include "orion/personal/PersonalInfo.hpp"
#include <array>

namespace util {
    inline u32 getPersonalInfoField(s32 species, s16 form, orion::personal::InfoField field) {
        // restore cached personal info in case the game was using it
        auto last_species = orion::personal::sCachedPersonalInfo->species;
        auto last_form = orion::personal::sCachedPersonalInfo->form;

        orion::personal::CacheInfo(species, form);

        auto result = orion::personal::GetCacheInfoField(field);

        orion::personal::CacheInfo(last_species, last_form);

        return result;
    }
    inline bool isInGame(s32 species, s16 form) {
        // restore cached personal info in case the game was using it
        auto last_species = orion::personal::sCachedPersonalInfo->species;
        auto last_form = orion::personal::sCachedPersonalInfo->form;

        orion::personal::CacheInfo(species, form);

        auto result = orion::personal::sCachedPersonalInfo->body.presentInGame;

        orion::personal::CacheInfo(last_species, last_form);

        return result;
    }
    inline std::array<u8, 6> getBaseStats(s32 species, s32 form) {
        // restore cached personal info in case the game was using it
        auto last_species = orion::personal::sCachedPersonalInfo->species;
        auto last_form = orion::personal::sCachedPersonalInfo->form;

        orion::personal::CacheInfo(species, form);

        auto base_stats = orion::personal::sCachedPersonalInfo->body.baseStats();
        auto result = std::array<u8, 6>{base_stats[0], base_stats[1], base_stats[2], base_stats[3], base_stats[4], base_stats[5]};

        orion::personal::CacheInfo(last_species, last_form);

        return result;
    }
}