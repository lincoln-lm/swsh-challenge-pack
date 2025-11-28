#pragma once
#include "hk/types.h"
namespace orion::field::encounter {
    enum EventEncounterScenario {
        None,
        Legendary_Pokemon,
        _2,
        _3,
        Eternatus,
        Eternamax_Eternatus_1,
        Eternamax_Eternatus_2,
        Zacian_Zamazenta_Fog,
        Motostoke_Gym_Challenge,
        Max_Raid_Battle_1,
        Max_Raid_Battle_2,
        Max_Raid_Battle_3,
        Max_Raid_Battle_4,
        Zacian_Zamazenta_Boss,
        Fast_Slowpoke,
        Regigigas_Raid_Battle,
        Special_Raid_Battle,
        Calyrex,
        Glastrier_Spectrier,
        Calyrex_Fusion,
    }
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
    };
}