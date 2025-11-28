#pragma once
#include "hk/types.h"

namespace orion::evolution {
    enum class EvolutionType : u16 {
        None = 0,
        LevelUp = 4,
        Trade = 5,
        TradeHeldItem = 6,
        TradeShelmetKarrablast = 7,
        LevelUpHeldItemDay = 19,
        LevelUpWithTeammate = 22,
    };

    struct EvolutionMethod {
        EvolutionType type;
        u16 argument;
        u16 species;
        s8 form;
        u8 level;
    } __attribute__((packed));

    struct EvolutionSet {
        u64 unk;
        u32 species;
        u32 form;
        EvolutionMethod (*methods)[9];
    } __attribute__((packed));
    static_assert(sizeof(EvolutionSet) == 0x18);

    struct EvolutionSetCache {
        u64 unk0;
        // for some reason the size of each item is 2*sizeof(EvolutionSet)
        // despite it only ever writing/reading 1 EvolutionSet
        EvolutionSet (*ring)[2];
        u8 unk1[0x18];
        size ringSize;
        size writeIndex;

        void Add(EvolutionSet* set);
        bool TryGet(u32 species, u32 form, EvolutionSet* out);
    } __attribute__((packed));
    static_assert(offsetof(EvolutionSetCache, ringSize) == 0x28);

    extern EvolutionSet* sActiveEvolutionSet;
    extern EvolutionSetCache* sEvolutionSetCache;

    void FetchEvolutionSet(u32 species, u32 form);
    void LoadEvolutionSet(EvolutionSet* out, u32 species, u32 form);
}