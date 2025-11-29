#pragma once

#include "hk/hook/Trampoline.h"
#include "orion/evolution/Data.hpp"
#include "orion/personal/PersonalInfo.hpp"
#include "rng/RngManager.hpp"
#include "save/SaveFile.hpp"
#include "util/Personal.hpp"
#include <tuple>

inline HkTrampoline<void, orion::evolution::EvolutionSet*, u32, u32> randomizeEvolutions = hk::hook::trampoline([](orion::evolution::EvolutionSet* set, u32 species, u32 form) {
    randomizeEvolutions.orig(set, species, form);
    if (!save::gSaveFile.randomizeEvolutions) return;
    u8 base_type_1 = util::getPersonalInfoField(species, form, orion::personal::InfoField::TYPE_1);
    u8 base_type_2 = util::getPersonalInfoField(species, form, orion::personal::InfoField::TYPE_2);
    u8 base_exp_growth = util::getPersonalInfoField(species, form, orion::personal::InfoField::EXP_GROWTH);
    auto randomEvolution = [base_type_1, base_type_2, base_exp_growth](auto rng, auto &method, auto species, auto form) {
        while (true) {
            auto [evo_species, evo_form] = rng.RandSpeciesAndForm();
            // force changed evo
            if (evo_species == method.species && evo_form == method.form) {
                continue;
            }
            if (species == method.species && form == method.form) {
                continue;
            }
            u8 evo_type_1 = util::getPersonalInfoField(evo_species, evo_form, orion::personal::InfoField::TYPE_1);
            u8 evo_type_2 = util::getPersonalInfoField(evo_species, evo_form, orion::personal::InfoField::TYPE_2);
            u8 evo_exp_growth = util::getPersonalInfoField(evo_species, evo_form, orion::personal::InfoField::EXP_GROWTH);
            if (evo_exp_growth != base_exp_growth) {
                continue;
            }
            if (!save::gSaveFile.randomizeEvolutionsMatchTypes
                || base_type_1 == evo_type_1
                || base_type_1 == evo_type_2
                || base_type_2 == evo_type_1
                || base_type_2 == evo_type_2) {
                method.species = evo_species;
                method.form = evo_form;
                break;
            }
        }
    };
    if (save::gSaveFile.randomizeEvolutionsEachLevel) {
        auto rng = RngManager::NewRandomGenerator();
        // give everything a valid level up evo as their first method
        orion::evolution::EvolutionMethod& method = (*set->methods)[0];
        method.type = orion::evolution::EvolutionType::LevelUp;
        // every level is > 1 so it always triggers on levelup
        method.level = 1;
        randomEvolution(rng, method, species, form);
        for (int i = 1; i < 9; i++) {
            orion::evolution::EvolutionMethod& method = (*set->methods)[i];
            method.type = orion::evolution::EvolutionType::None;
        }
        return;
    }
    const std::string seed = std::format("evolution_binary_{}_{}", species, form);
    auto rng = RngManager::NewRandomGenerator(seed);
    for (int i = 0; i < 9; i++) {
        orion::evolution::EvolutionMethod& method = (*set->methods)[i];
        if (method.species == 0) {
            continue;
        }
        // regular trade evos -> level 30
        if (method.type == orion::evolution::EvolutionType::Trade) {
            method.type = orion::evolution::EvolutionType::LevelUp;
            method.level = 30;
        // trade held item -> level held item
        } else if (method.type == orion::evolution::EvolutionType::TradeHeldItem) {
            method.type = orion::evolution::EvolutionType::LevelUpHeldItemDay;
        // trade with mon -> level with mon
        } else if (method.type == orion::evolution::EvolutionType::TradeShelmetKarrablast) {
            // karrablast
            if (method.species == 588) {
                method.argument = 616;
            // shelmet
            } else if (method.species == 616) {
                method.argument = 588;
            }
        }
        randomEvolution(rng, method, species, form);
    }
});

inline std::tuple<u32, u32> sLastEvolution = std::make_tuple(0, 0);

inline HkTrampoline<void, u32, u32> logLastEvolution = hk::hook::trampoline([](u32 species, u32 form) {
    logLastEvolution.orig(species, form);
    sLastEvolution = std::make_tuple(species, form);
});

inline HkTrampoline<bool, orion::evolution::EvolutionSetCache*, u32, u32, orion::evolution::EvolutionSet*> invalidateCache = hk::hook::trampoline([](orion::evolution::EvolutionSetCache* cache, u32 species, u32 form, orion::evolution::EvolutionSet* out) {
    // invalidate the cache if we're randomizing evolutions each level so that the same species doesn't always get the same evolution
    bool shouldnt_hook = !save::gSaveFile.randomizeEvolutions || !save::gSaveFile.randomizeEvolutionsEachLevel;
    // the game queries the evolution cache multiple times per actual evolution.
    // this means we cannot just invalidate the cache every time and must only invalidate it if the species/form has changed.
    // technically, if the player evolves the same species twice in a row it will maintain the cache, but this is unlikely enough to not matter.
    if (shouldnt_hook || std::make_tuple(species, form) == sLastEvolution) {
        return invalidateCache.orig(cache, species, form, out);
    }
    return false;
});

inline void installEvolutionsHooks() {
    randomizeEvolutions.installAtPtr(pun<void*>(&orion::evolution::LoadEvolutionSet));
    logLastEvolution.installAtPtr(pun<void*>(&orion::evolution::FetchEvolutionSet));
    invalidateCache.installAtPtr(pun<void*>(&orion::evolution::EvolutionSetCache::TryGet));
}