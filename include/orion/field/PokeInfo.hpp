#pragma once

#include "hk/types.h"

namespace orion::field {
    struct PokeInfo {
        // ...
        u16 GetSpecies();
        u16 GetForm();
        u16 GetHeldItem();
        u16 GetNature();
        u16 GetCurrentHP();
        u16 GetAttack();
        u16 GetDefense();
        u16 GetSpecialAttack();
        u16 GetSpecialDefense();
        u16 GetSpeed();
        u16 GetMaxHP();
        u16 GetAbility();
        u8 GetLevel();
        static u32 CalcLevel(u32 species, u32 form, u32 experience);
    };
}