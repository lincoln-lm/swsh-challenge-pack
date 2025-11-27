#pragma once

#include <map>
#include <set>
#include "hk/hook/Trampoline.h"
#include "hk/types.h"
#include "orion/field/AreaLoader.hpp"
#include "orion/field/FieldObject.hpp"
#include "orion/field/FileCache.hpp"
#include "rng/RngManager.hpp"
#include "save/SaveFile.hpp"
#include "util/FNV.hpp"
#include "hook/InlineHook.hpp"
#include "orion/field/FieldObjects_flatbuffer.h"

// specifically the initial starter models
inline const std::set<u64> cStarterHashes = {
    // sobble
    util::fnv1a("z_t0101_MIZU"),
    // scorbunny
    util::fnv1a("z_t0101_HONO"),
    // grookey
    util::fnv1a("z_t0101_KUSA"),
};

inline const std::map<u64, u64> cHashToSeedMap = {
    // sobble
    { util::fnv1a("z_t0101_MIZU"), 0x289d0e4aa0fd660d },
    { util::fnv1a("z_t0101_i0101_MIZU"), 0x289d0e4aa0fd660d },
    // scorbunny
    { util::fnv1a("z_t0101_HONO"), 0xe611a220de507f60 },
    { util::fnv1a("z_t0101_i0101_HONO"), 0xe611a220de507f60 },
    // grookey
    { util::fnv1a("z_t0101_KUSA"), 0xd0f69218d6d84126 },
    { util::fnv1a("z_t0101_i0101_KUSA"), 0xd0f69218d6d84126 },
    // fog zamazenta
    { util::fnv1a("z_d0101_PM_889_OOKAMIT"), 0x6f0b36ae4a5df4a9 },
    // TODO: fog zacian z_d0101_PM_888_OOKAMIK
    { util::fnv1a("z_r0501_i0101_PM_848_PUNKII1"), 0x534abd7f00ee7e61 },
    // fire gym mons
    { util::fnv1a("z_c0101_g0102_CNT_POKE"), 0x59f8ac028fe0e5c },
    { util::fnv1a("z_c0101_g0102_RIGHT_POKE"), 0x59f8bc028fe100f },
    { util::fnv1a("z_c0101_g0102_LEFT_POKE"), 0x8c311b85a7bab25a },
    // wedgehurst slowpoke
    { util::fnv1a("z_t0201_s0101_PM_79_YADONR"), 0xf619d64a8e0641cb },
};

static void replace_species_form(u64 hash, s32* species_ptr, s16* form_ptr) {
    u64 seed;
    // use gift's add_pokemon hash and event encounters' event_encount hash for the seed
    // TODO: type: null, kanto starters, kubfu, cosmog, poipole, other scripted encounters
    auto find_seed_result = cHashToSeedMap.find(hash);
    if (find_seed_result != cHashToSeedMap.end()) {
        seed = find_seed_result->second;
    } else {
        // it would be neat to randomize all models based on their hash
        // but there is something else limiting how many unique pokemon
        // npcs can exist.
        // randomizing based on species/form maps 1:1 from the game
        // which should guarantee to dodge this limit.
        seed = (*species_ptr) | (*form_ptr) << 16;
    }
    // randomize the same as gifts.hpp
    auto rng = RngManager::NewRandomGenerator(seed);
    auto [species, form] = rng.RandSpeciesAndForm();
    if (save::gSaveFile.randomizePokemonModels && save::gSaveFile.hideStarters && cStarterHashes.find(hash) != cStarterHashes.end()) {
        // (0, 0) is a pikachu
        // TODO: fun custom model?
        species = 0;
        form = 0;
    }
    *species_ptr = species;
    *form_ptr = form;
}

inline auto randomizePokemonModelsOnLoad = hook::inlineHook([](hook::CpuState* state) {
    // original instruction
    state->X[9] = *pun<s32*>(state->X[27]);
    if (!save::gSaveFile.randomizePokemonModels) {
        return;
    }
    auto fb = pun<orion::field::flatbuffers::PokemonModel*>(state->X[27]);
    u64 sp = state->X[1] - 0x100;
    u64 hash = fb->inner()->inner()->field_object()->unique_hash();
    s32* species_ptr = pun<s32*>(sp + 0xE0);
    s16* form_ptr = pun<s16*>(sp + 0xE4);
    replace_species_form(hash, species_ptr, form_ptr);
    state->X[0] = *species_ptr;
    state->X[8] = *form_ptr;
});

inline bool sIsConstructingEncountObject = false;
constexpr u64 cPokemonCenterHashes[6] = {
    util::fnv1a("HealPoke_00"),
    util::fnv1a("HealPoke_01"),
    util::fnv1a("HealPoke_02"),
    util::fnv1a("HealPoke_03"),
    util::fnv1a("HealPoke_04"),
    util::fnv1a("HealPoke_05"),
};
inline auto randomizePokemonModels = hook::inlineHook([](hook::CpuState* state) {
    // original instruction
    state->X[8] = *pun<s32*>(state->X[20]);
    if (!save::gSaveFile.randomizePokemonModels) {
        return;
    }
    if (sIsConstructingEncountObject) {
        return;
    }
    auto pokemon_model = pun<orion::field::PokemonModel*>(state->X[19]);
    for (auto pokemon_center_hash : cPokemonCenterHashes) {
        if (pokemon_model->uniqueHash == pokemon_center_hash) {
            return;
        }
    }
    u64 hash = pokemon_model->uniqueHash;
    s32* species_ptr = &pokemon_model->species;
    s16* form_ptr = pun<s16*>(&pokemon_model->form);
    replace_species_form(hash, species_ptr, form_ptr);
});

inline HkTrampoline<void, orion::field::FileCache*, orion::field::FileCache::CacheFileStack_t*, int> patchNullPtrDeref = hk::hook::trampoline([](orion::field::FileCache* this_, orion::field::FileCache::CacheFileStack_t* stack_struct, int category) -> void {
    // when preparing pokemon models in specific locations with modified models, the game tries to cache resources that do not exist for those pokemon.
    // it does not gracefully handle this case and will crash the game in specific circumstances (problem case was a model being replaced with zekrom).
    // the game can gracefully handle the files not being cached so we patch the function to just return early if the file doesnt exist.
    if (stack_struct->gfFile == nullptr) {
        return;
    }
    return patchNullPtrDeref.orig(this_, stack_struct, category);
});

inline HkTrampoline<void, orion::field::EncountObject*, u64, u64, u64> logEncountObjectConstructor = hk::hook::trampoline([](orion::field::EncountObject* obj, u64 param_1, u64 param_2, u64 param_3) {
    sIsConstructingEncountObject = true;
    logEncountObjectConstructor.orig(obj, param_1, param_2, param_3);
    sIsConstructingEncountObject = false;
});

inline void installPokemonModelHooks() {
    logEncountObjectConstructor.installAtPtr(pun<void*>(&orion::field::EncountObject::Constructor));
    patchNullPtrDeref.installAtPtr(pun<void*>(&orion::field::FileCache::CacheFile));
    randomizePokemonModelsOnLoad.installAtPtrOffset(pun<ptr>(&orion::field::AreaLoader::InitializeAreaCaches), 0x16a8);
    randomizePokemonModels.installAtPtrOffset(pun<ptr>(&orion::field::PokemonModel::Constructor), 0xc4);
}
