#pragma once
#include "hk/types.h"
#include "orion/field/encounter/EventEncounterManager.hpp"
namespace orion::field::encounter {
    enum class EncounterWeather {}; // TODO
    EncounterWeather GetCurrentWeather();
    // gimmick encounters are event encounters!
    using GimmickSpec = orion::field::encounter::EventEncounter;
    struct OverworldSpec {
        u32 species;
        u16 form;
        u8 level;
        u8 unk0;
        u32 shininess;
        u16 nature;
        u16 unk1;
        u32 gender;
        u32 ability;
        u32 hasItem;
        u16 heldItem;
        u16 unk2;
        u32 moves[4];
        u32 unk3;
        u8 unk4;
        u8 guaranteedIVs;
        u16 ivs[6];
        u8 unk5[6]; // evs?
        u16 mark;
        u8 brilliantIndex;
        u8 unk6[5];
        u64 fixedSeed;
    };
    static_assert(sizeof(OverworldSpec) == 0x58);
    struct EncounterSlot {
        u16 rate;
        u16 species;
        u16 form;
    };
    struct EncounterTable {
        u8 minLevel;
        u8 maxLevel;
        EncounterSlot slots[10];
    };
    static_assert(sizeof(EncounterTable) == 0x3e);
    struct SymbolEncounterAreaTables {
        bool isValid;
        u64 hash;
        EncounterTable tables[9]; // TODO weather::count
    };
    struct HiddenEncounterAreaTables {
        bool isValid;
        u64 hash;
        EncounterTable tables[11]; // TODO weather::count + 2
    };
    struct EncounterGenerator {
        // TODO
        // TODO: flags
        void GenerateBasicSpec(OverworldSpec* spec, EncounterSlot* slot, s32 minLevel, s32 maxLevel, void* flags);
    };
    struct OverworldEncounterManager {
        // TODO
        bool GenerateGimmick(GimmickSpec* spec, OverworldSpec* result);
        SymbolEncounterAreaTables FetchSymbolEncounterTable(u64* hash_ptr);
        HiddenEncounterAreaTables FetchHiddenEncounterTable(u64* hash_ptr);
    };
}