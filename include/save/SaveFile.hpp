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
        operator u64() const { return value; }
        void operator=(u64 value) { this->value = value; }
        virtual void cycleForward();
        virtual void cycleBackward();
        virtual std::string toString() const;
        Setting(const char* serialName, const char* displayName, const char* description, u64 value, bool indented = false) : serialName(serialName), displayName(displayName), description(description), value(value), indented(indented) {}
    };
    struct BooleanSetting : public Setting {
        void cycleBackward() override { value = !value; }
        void cycleForward() override { value = !value; }
        std::string toString() const override { 
            return std::format("{}: {}", displayName, value ? "Enabled" : "Disabled");
        }
        operator bool() const { return value; }
        void operator=(bool value) { this->value = value; }
        BooleanSetting(const char* serialName, const char* displayName, const char* description, bool value, bool indented = false) : Setting(serialName, displayName, description, value, indented) {}
    };
    struct IntegerSetting : public Setting {
        void cycleBackward() override { value--; }
        void cycleForward() override { value++; }
        std::string toString() const override {
            return std::format("{}: {}", displayName, value);
        }
        operator u64() const { return value; }
        void operator=(u64 value) { this->value = value; }
        IntegerSetting(const char* serialName, const char* displayName, const char* description, u64 value, bool indented = false) : Setting(serialName, displayName, description, value, indented) {}
    };

    #define SETTING(type, var, displayName, description, value, indented) type var{#var, displayName, description, value, indented}

    struct SaveFile {
        SETTING(BooleanSetting, qualityOfLife, "Quality of Life", "Enables the quality of life features category. (has subsettings)", true, false);
        SETTING(BooleanSetting, skipIntro, "Skip Intro", "Skips the intro cutscene with Rose (and any other \"sequences\").", true, true);
        SETTING(BooleanSetting, instantText, "Instant Text", "Instantly displays text when available. Additionally skips any \"wait\" commands while B is held.", true, true);
        SETTING(BooleanSetting, randomizeGiftEncounters, "Randomize Gifts", "Randomize the species, form, ability, etc. of gift pokemon. Additionally removes their shiny locks.", true, false);
        SETTING(BooleanSetting, randomizeEventEncounters, "Randomize Scripted Encounters", "Randomize the species, form, ability, etc. of scripted encounters like legendaries. Additionally removes their shiny locks.", true, false);
        SETTING(BooleanSetting, randomizePokemonModels, "Randomize Pokemon Models", "Randomizes overworld pokemon models for things like starters and gifts (matches the received pokemon if applicable). (has subsettings)", true, false);
        SETTING(BooleanSetting, hideStarters, "Hide Starters", "Displays starters as pikachu so you won't know what they are randomized to.", false, true);

        SETTING(IntegerSetting, rngSeed, "RNG Seed", "Global seed for random number generation.", 0, false);
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
            &gSaveFile.randomizeGiftEncounters,
            &gSaveFile.randomizeEventEncounters,
            &gSaveFile.randomizePokemonModels,
            &gSaveFile.hideStarters,
            &gSaveFile.rngSeed,
        });
    }
    inline std::string serialzeSaveFile() {
        nlohmann::json json;
        auto fields = getSaveFileFields();
        for (auto field : fields) {
            json[field->serialName] = field->value;
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
            field->value = json[field->serialName];
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

