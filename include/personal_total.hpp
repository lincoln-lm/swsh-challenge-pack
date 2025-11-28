#include "hk/hook/Trampoline.h"
#include "orion/personal/PersonalInfo.hpp"
#include "rng/RngManager.hpp"
#include "save/SaveFile.hpp"

inline HkTrampoline<void, orion::personal::PersonalInfo*, u16, u16> randomizePersonalInfo = hk::hook::trampoline([](orion::personal::PersonalInfo* out, u16 species, u16 form) {
    randomizePersonalInfo.orig(out, species, form);
    if (species == 0 && form == 0) {
        return;
    }
    // unlikely to have valid stats to reference
    if (!out->body.presentInGame) {
        return;
    }
    if (!save::gSaveFile.randomizePersonalInfo) {
        return;
    }

    const std::string seed = std::format("personal_total_{}_{}", species, form);
    auto rng = RngManager::NewRandomGenerator(seed);
    // bst calc taken from UPR ZX
    s64 bst;
    
    // shedinja gets special handling
    bool is_shedinja = species == 292;
    auto out_base_stats = out->body.baseStats();
    if (is_shedinja) {
        bst = std::accumulate(
            out_base_stats.begin(),
            out_base_stats.end(),
            0
        ) - 51;
    } else {
        bst = std::accumulate(
            out_base_stats.begin(),
            out_base_stats.end(),
            0
        ) - 70;
    }
    if (bst <= 0) {
        return;
    }
    std::array<u16, 6> base_stats;
    do {
        f64 hp_weight = rng.RandDouble();
        f64 attack_weight = rng.RandDouble();
        f64 defense_weight = rng.RandDouble();
        f64 special_attack_weight = rng.RandDouble();
        f64 special_defense_weight = rng.RandDouble();
        f64 speed_weight = rng.RandDouble();
        f64 total_weight = (
            (is_shedinja ? 0.0 : hp_weight)
            + attack_weight
            + defense_weight
            + special_attack_weight
            + special_defense_weight
            + speed_weight
        );
        if (is_shedinja) {
            base_stats[0] = 1;
        } else {
            base_stats[0] = std::max(1.0, std::round(hp_weight / total_weight * bst)) + 20;
        }
        base_stats[1] = std::max(1.0, std::round(attack_weight / total_weight * bst)) + 10;
        base_stats[2] = std::max(1.0, std::round(defense_weight / total_weight * bst)) + 10;
        base_stats[3] = std::max(1.0, std::round(special_attack_weight / total_weight * bst)) + 10;
        base_stats[4] = std::max(1.0, std::round(special_defense_weight / total_weight * bst)) + 10;
        base_stats[5] = std::max(1.0, std::round(speed_weight / total_weight * bst)) + 10;
        
    } while (
        std::any_of(
            base_stats.begin(),
            base_stats.end(),
            [](u16 stat) { return stat > 255; }
        )
    );
    std::copy(
        base_stats.begin(),
        base_stats.end(),
        out_base_stats.begin()
    );
    out->body.ability1 = rng.RandAbility();
    out->body.ability2 = rng.RandAbility();
    out->body.hiddenAbility = rng.RandAbility();
    out->body.item1 = rng.RandHeldItem();
    out->body.item2 = rng.RandHeldItem();
    out->body.item3 = rng.RandHeldItem();
    // TODO: randomize TR/TM/Tutors?
});


inline void installPersonalTotalHooks() {
    randomizePersonalInfo.installAtPtr(pun<void*>(&orion::personal::GetInfo));
}