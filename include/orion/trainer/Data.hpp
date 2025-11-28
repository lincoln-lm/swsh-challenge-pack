#pragma once
#include "hk/types.h"

namespace orion::trainer {
    struct TrainerPokemon {
        u8 gender : 2;
        u8 unk_0 : 2;
        u8 ability : 2;
        u8 unk_1 : 2;
        u8 nature;
        u8 ev_hp;
        u8 ev_atk;
        u8 ev_def;
        u8 ev_spa;
        u8 ev_spd;
        u8 ev_spe;
        u8 dynamax_level;
        u8 can_gigantamax;
        u16 level;
        u16 species;
        u16 form;
        u16 held_item;
        s16 moves[4];
        u16 unk_2;
        u32 iv_hp : 5;
        u32 iv_atk : 5;
        u32 iv_def : 5;
        u32 iv_spa : 5;
        u32 iv_spd : 5;
        u32 iv_spe : 5;
        bool shiny : 1;
        bool can_dynamax : 1;
    };
    static_assert(sizeof(TrainerPokemon) == 0x20);
    void LoadTrainerPokemon(TrainerPokemon* out, void* param_2, u64 trainerId);
}