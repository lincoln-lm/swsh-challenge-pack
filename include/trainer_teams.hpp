#pragma once

#include "hook/InlineHook.hpp"
#include "mod_hooks.hpp"
#include "orion/trainer/Data.hpp"
#include "orion/filesystem/GFFile.hpp"
#include "rng/RngManager.hpp"
#include "save/SaveFile.hpp"
#include "util/LevelBoost.hpp"

inline auto randomizeTrainerTeams = hook::inlineHook([](hook::CpuState* state) {
    auto trainer_poke_file = pun<orion::filesystem::GFFile*>(state->X[20]);
    auto file_path = trainer_poke_file->filePath;
    void* trainer_data_out = pun<void*>(state->X[0]);
    auto trainer_team = pun<orion::trainer::TrainerPokemon*>(trainer_poke_file->buffer);
    size file_buffer_size = trainer_poke_file->bufferSize;
    s32 team_size = file_buffer_size / sizeof(orion::trainer::TrainerPokemon);

    if (save::gSaveFile.randomizeTrainerTeams && sHooksEnabled) {
        auto rng = RngManager::NewRandomGenerator(file_path);
        for (int i = 0; i < team_size; i++) {
            if (trainer_team[i].level == 0 || trainer_team[i].level > 100 || trainer_team[i].species == 0) {
                continue;
            }
            if (save::gSaveFile.trainerLevelBoost) {
                trainer_team[i].level = util::levelBoost(trainer_team[i].level);
            }
            auto [
                species,
                form
            ] = rng.RandSpeciesAndForm();
            trainer_team[i].species = species;
            trainer_team[i].form = form;
            // TODO: gender ratios
            trainer_team[i].gender = rng.RandMax(2);
            trainer_team[i].ability = rng.RandMax(3);
            trainer_team[i].held_item = rng.RandHeldItem();
            rng.RandMoves(std::span(trainer_team[i].moves));

            // TODO: randomize nature/ivs/evs?
            trainer_team[i].shiny = rng.RandChance(4096);
        }
    }

    // original instruction
    // the vanilla game will always memcpy 192 bytes which will read out of bounds of the buffer if there are less than 6 pokemon!
    // this extra copied data is not actually accessed (the game checks the # of mons on the team and won't access past it)
    // but reading past the bounds is still theoretically problematic
    std::memcpy(trainer_data_out, trainer_team, file_buffer_size);
});

inline void installTrainerTeamsHooks() {
    randomizeTrainerTeams.installAtPtrOffset(pun<ptr>(&orion::trainer::LoadTrainerPokemon), 0xf8);
}