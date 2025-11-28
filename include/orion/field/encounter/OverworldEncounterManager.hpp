#pragma once
#include "hk/types.h"
#include "orion/field/encounter/EventEncounterManager.hpp"
namespace orion::field::encounter {
    enum class EncounterWeather {}; // TODO
    EncounterWeather GetCurrentWeather();
    // gimmick encounters are event encounters!
    using GimmickSpec = orion::field::encounter::EventEncounter;
    struct OverworldSpec {}; // TODO
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