#pragma once
#include "orion/personal/PersonalInfo.hpp"

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
}