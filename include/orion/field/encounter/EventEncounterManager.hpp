#pragma once
#include "hk/types.h"
namespace orion::field::encounter {
    enum EventEncounterScenario {
        NONE = 0,
        LEGENDARY,
        ETERNATUS = 4,
        ETERNAMAX_1,
        ETERNAMAX_2,
        FOG_BOX_LEGENDARY,
        MOTOSTOKE_ENCOUNTER,
        MAX_RAID_BATTLE_1,
        MAX_RAID_BATTLE_2,
        MAX_RAID_BATTLE_3,
        MAX_RAID_BATTLE_4,
        BOX_LEGENDARY_BOSS,
        FAST_SLOWPOKE,
        REGIGIGAS,
        SPECIAL_RAID_BATTLE,
        CALYREX,
        GLASTRIER_SPECTRIER,
        CALYREX_FUSION,
    };
    struct EventEncounter {
        u64 hash;
        u32 species;
        u16 form;
        u16 padding0;
        u32 unk0;
        u32 level;
        u32 shinyLock;
        u32 gender;
        u32 nature;
        u32 ability;
        u16 heldItem;
        u8 padding1[6];
        u64 backgroundFarTypeId;
        u64 backgroundNearTypeId;
        u32 encounterScenario;
        u32 moves[4];
        u8 dynamaxLevel;
        u8 unk1;
        bool canGigantamax;
        u8 ivs[6];
        u8 evs[6];
        u8 unk2;
    } __attribute__((packed));
    static_assert(sizeof(EventEncounter) == 0x64);
    struct EventEncounterManager {
        // TODO
        void UnpackEventEncounterArchive(void* archiveFlatbuffer);
        EventEncounter* GetEvent(u64* hashPtr);
    };
}