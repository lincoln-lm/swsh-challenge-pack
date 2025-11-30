#pragma once

#include "filesystem/FileHandler.hpp"
#include "hk/diag/diag.h"
#include "hk/types.h"
#include "hk/util/Random.h"
#include <format>
#include <nlohmann_json.hpp>
#include <array>
#include <cstddef>

namespace save {
    // TODO: make this more sane
    struct Setting {
        const char* serialName;
        const char* displayName;
        const char* description;
        u64 value;
        bool indented = false;
        bool display = true;
        operator u64() const { return value; }
        void operator=(u64 value) { this->value = value; }
        virtual void cycleForward();
        virtual void cycleBackward();
        virtual std::string toString() const;
        virtual nlohmann::json serialize() const { return value; };
        virtual void deserialize(nlohmann::json j) { value = j; };
        Setting(const char* serialName, const char* displayName, const char* description, u64 value, bool indented = false, bool display = true) : serialName(serialName), displayName(displayName), description(description), value(value), indented(indented), display(display) {}
    };
    struct BooleanSetting : public Setting {
        void cycleBackward() override { value = !value; }
        void cycleForward() override { value = !value; }
        std::string toString() const override { 
            return std::format("{}: {}", displayName, value ? "Enabled" : "Disabled");
        }
        operator bool() const { return value; }
        void operator=(bool value) { this->value = value; }
        BooleanSetting(const char* serialName, const char* displayName, const char* description, bool value, bool indented = false, bool display = true) : Setting(serialName, displayName, description, value, indented, display) {}
    };
    struct IntegerSetting : public Setting {
        void cycleBackward() override { value--; }
        void cycleForward() override { value++; }
        std::string toString() const override {
            return std::format("{}: {}", displayName, value);
        }
        operator u64() const { return value; }
        void operator=(u64 value) { this->value = value; }
        IntegerSetting(const char* serialName, const char* displayName, const char* description, u64 value, bool indented = false, bool display = true) : Setting(serialName, displayName, description, value, indented, display) {}
    };
    template<typename T>
    struct VectorSetting : public Setting {
        std::vector<T> value;
        void cycleBackward() override {  }
        void cycleForward() override {  }
        std::string toString() const override {
            return "SHOULD NOT BE DISPLAYED";
        }
        nlohmann::json serialize() const override {
            return value;
        }
        virtual void deserialize(nlohmann::json j) override { value = j.get<std::vector<u64>>(); };
        void operator=(std::vector<T> value) { this->value = value; }
        VectorSetting(const char* serialName, const char* displayName, const char* description, u64 value, bool indented = false, bool display = false) : Setting(serialName, displayName, description, value, indented, display) {}
    };

    #define SETTING(type, var, displayName, description, value, indented, display) type var{#var, displayName, description, value, indented, display}

    struct SaveFile {
        SETTING(BooleanSetting, qualityOfLife, "Quality of Life", "Enables the quality of life features category. (has subsettings)", true, false, true);
        SETTING(BooleanSetting, skipIntro, "Skip Intro", "Skips the intro cutscene with Rose (and any other \"sequences\").", true, true, true);
        SETTING(BooleanSetting, instantText, "Instant Text", "Instantly displays text when available. Additionally skips any \"wait\" commands while B is held.", true, true, true);
        SETTING(BooleanSetting, routeRestriction, "Nuzlocke Route Restriction", "Removes all spawns, fishing points, and raid dens from a route after a pokemon from the route has been caught or fainted. In the wild area, this is per what the game considers \"zones\".", true, false, true);
        SETTING(BooleanSetting, permadeath, "Nuzlocke Permadeath", "When one of your pokemon faint they are removed from your party permanently.", true, false, true);
        SETTING(BooleanSetting, randomizeWildEncounters, "Randomize Wild Encounters", "Randomize the species, form, ability, etc. of wild pokemon including gimmick spawns (strong spawns). (has subsettings)", true, false, true);
        SETTING(BooleanSetting, liveRandomizeWildEncounters, "Live Randomization", "Generates a random pokemon *each* time a wild pokemon is generated rather than referencing a static table for each route.", true, true, true);
        SETTING(BooleanSetting, wildLevelBoost, "Boost Level", "Boosts the level of wild pokemon by roughly 1.5x.", true, true, true);
        SETTING(BooleanSetting, randomizeGiftEncounters, "Randomize Gifts", "Randomize the species, form, ability, etc. of gift pokemon. Additionally removes their shiny locks.", true, false, true);
        SETTING(BooleanSetting, randomizeEventEncounters, "Randomize Scripted Encounters", "Randomize the species, form, ability, etc. of scripted encounters like legendaries. Additionally removes their shiny locks.", true, false, true);
        SETTING(BooleanSetting, randomizeTrainerTeams, "Randomize Trainer Teams", "Randomizes the species, form, ability, etc. of trainer pokemon. Additionally gives them a 1/4096 chance of being shiny :). (has subsettings)", true, false, true);
        SETTING(BooleanSetting, trainerLevelBoost, "Boost Level", "Boosts the level of trainer pokemon by roughly 1.5x.", true, true, true);
        SETTING(BooleanSetting, boostLevelCap, "Boost Capture Level Cap", "Boosts the capture level cap by roughly 1.5x (matches traier & wild boosts).", true, false, true);
        SETTING(BooleanSetting, randomizePokemonModels, "Randomize Pokemon Models", "Randomizes overworld pokemon models for things like starters and gifts (matches the received pokemon if applicable). (has subsettings)", true, false, true);
        SETTING(BooleanSetting, hideStarters, "Hide Starters", "Displays starters as pikachu so you won't know what they are randomized to.", false, true, true);
        SETTING(BooleanSetting, randomizeEvolutions, "Randomize Evolutions", "Randomizes what pokemon evolve into. (has subsettings)", true, false, true);
        SETTING(BooleanSetting, randomizeEvolutionsMatchTypes, "Match Types", "Ensures the evolution shares a type with the pokemon it evolves from.", false, true, true);
        SETTING(BooleanSetting, randomizeEvolutionsEachLevel, "Each Level", "Every pokemon evolves after each level up.", false, true, true);
        SETTING(BooleanSetting, randomizeLearnsets, "Randomize Learnsets", "Randomizes the moves that pokemon learn. Additionally tries to ensure there are at least 4 moves at level 1 for a complete moveset.", true, false, true);
        SETTING(BooleanSetting, randomizePersonalInfo, "Randomize Personal Info", "Randomizes the base stats and abilities of all pokemon. Attempts to maintain the pokemon's original base stat total by redistributing the stats.", true, false, true);
        SETTING(BooleanSetting, randomizeFieldItems, "Randomize Field Items", "Randomizes hidden and visible field items. TMs are randomized to other TMs.", true, false, true);
        SETTING(BooleanSetting, randomizeGiftTMs, "Randomize Gift TMs", "Randomizes the TMs gifted to you by gym leaders and hop [incomplete].", true, false, true);
        SETTING(BooleanSetting, filterShop, "Filter Shops", "Filters shops to only sell pokeballs. Empty shops sell only rare bones.", true, false, true);

        SETTING(VectorSetting<u64>, blacklistedZones, "", "", 0, false, false);
        SETTING(IntegerSetting, rngSeed, "RNG Seed", "Global seed for random number generation.", 0, false, true);
        SaveFile() {
            rngSeed = hk::util::getRandomU64();
        }
    };
    extern SaveFile gSaveFile;
    inline auto getSaveFileFields() {
        return std::to_array<Setting*>({
            &gSaveFile.qualityOfLife,
            &gSaveFile.skipIntro,
            &gSaveFile.instantText,
            &gSaveFile.routeRestriction,
            &gSaveFile.permadeath,
            &gSaveFile.randomizeWildEncounters,
            &gSaveFile.liveRandomizeWildEncounters,
            &gSaveFile.wildLevelBoost,
            &gSaveFile.randomizeGiftEncounters,
            &gSaveFile.randomizeEventEncounters,
            &gSaveFile.randomizeTrainerTeams,
            &gSaveFile.trainerLevelBoost,
            &gSaveFile.boostLevelCap,
            &gSaveFile.randomizePokemonModels,
            &gSaveFile.hideStarters,
            &gSaveFile.randomizeEvolutions,
            &gSaveFile.randomizeEvolutionsMatchTypes,
            &gSaveFile.randomizeEvolutionsEachLevel,
            &gSaveFile.randomizeLearnsets,
            &gSaveFile.randomizePersonalInfo,
            &gSaveFile.randomizeFieldItems,
            &gSaveFile.randomizeGiftTMs,
            &gSaveFile.filterShop,

            &gSaveFile.blacklistedZones,
            &gSaveFile.rngSeed,
        });
    }
    inline std::string serialzeSaveFile() {
        nlohmann::json json;
        auto fields = getSaveFileFields();
        for (auto field : fields) {
            json[field->serialName] = field->serialize();
        }
        return json.dump(4);
    }
    constexpr const char* cSaveFilePath = "sd:/switch/swsh_challenge_pack_save.json";
    inline bool save() {
        if (!filesystem::FileHandler::MountSD()) { 
            hk::diag::log("Failed to mount SD");
            return false;
        }
        if (!filesystem::FileHandler::WriteFile(cSaveFilePath, serialzeSaveFile())) {
            hk::diag::log("Failed to write save file");
            return false;
        }
        return true;
    }
    inline void deserializeSaveFile(std::string data) {
        // TODO: exception handling?
        nlohmann::json json = nlohmann::json::parse(data);
        auto fields = getSaveFileFields();
        for (auto field : fields) {
            if (!json.contains(field->serialName)) {
                continue;
            }
            field->deserialize(json[field->serialName]);
        }
    }
    inline bool load() {
        if (!filesystem::FileHandler::MountSD()) { 
            hk::diag::log("Failed to mount SD");
            return false;
        }
        std::string data;
        if (!filesystem::FileHandler::ReadFile(cSaveFilePath, data)) {
            hk::diag::log("Failed to read save file");
            return false;
        }
        deserializeSaveFile(data);
        return true;
    }
}

