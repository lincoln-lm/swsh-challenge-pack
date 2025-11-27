#pragma once
#include "hk/types.h"
namespace orion::personal {
    struct PersonalInfo {
        u64 unk0;
        u32 species;
        u32 form;
        struct {
            u8 baseHp;
            u8 baseAtk;
            u8 baseDef;
            u8 baseSpAtk;
            u8 baseSpDef;
            u8 baseSpeed;
            u8 type1; // TODO: type enum
            u8 type2;
            u8 catchRate;
            u8 evolutionStage;
            u8 evHp : 2;
            u8 evAtk : 2;
            u8 evDef : 2;
            u8 evSpAtk : 2;
            u8 evSpDef : 2;
            u8 evSpeed : 2;
            u8 : 4;
            u16 item1;
            u16 item2;
            u16 item3;
            u8 gender;
            u8 hatchCycle;
            u8 baseFriendship;
            u8 growthRate;
            u8 eggGroup1;
            u8 eggGroup2;
            u16 ability1;
            u16 ability2;
            u16 hiddenAbility;
            u16 formStatsIndex;
            u8 formCount;
            u8 color : 6;
            u8 presentInGame : 1;
            u8 spriteForm : 1;
            u16 baseExp;
            u16 height;
            u16 weight;
            // ... TODO
        } body;
    };
    static_assert(offsetof(PersonalInfo, body.weight) == 0x36);
    extern PersonalInfo* sCachedPersonalInfo;
    enum InfoField {
        TYPE_1 = 0x6,
        TYPE_2 = 0x7,
        EXP_GROWTH = 0x17,
        FORM_COUNT = 0x1e,
        // TODO
    };
    void GetInfo(PersonalInfo* out, u16 species, u16 form);
    void CacheInfo(u16 species, u16 form);
    u32 GetInfoField(PersonalInfo* personalInfo, InfoField field);
    u32 GetCacheInfoField(InfoField field);
}