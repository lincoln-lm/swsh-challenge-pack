#pragma once

#include "filesystem/FileHandler.hpp"
#include "hk/diag/diag.h"
#include "hk/types.h"
#include <nlohmann_json.hpp>
#include <array>
#include <cstddef>

namespace save {
    // TODO: make this more sane
    struct Setting {
        const char* serialName;
        const char* displayName;
        u64 value;
        bool indented = false;
        operator u64() const { return value; }
        void operator=(u64 value) { this->value = value; }
        virtual void cycleForward();
        virtual void cycleBackward();
        Setting(const char* serialName, const char* displayName, u64 value, bool indented = false) : serialName(serialName), displayName(displayName), value(value), indented(indented) {}
    };
    struct BooleanSetting : public Setting {
        void cycleBackward() override { value = !value; }
        void cycleForward() override { value = !value; }
        operator bool() const { return value; }
        void operator=(bool value) { this->value = value; }
        BooleanSetting(const char* serialName, const char* displayName, bool value, bool indented = false) : Setting(serialName, displayName, value, indented) {}
    };

    #define SETTING(type, var, displayName, value, indented) type var{#var, displayName, value, indented}

    struct SaveFile {
        SETTING(BooleanSetting, qualityOfLife, "Quality of Life", true, false);
        SETTING(BooleanSetting, skipIntro, "Skip Intro", true, true);
        SETTING(BooleanSetting, instantText, "Instant Text", true, true);
    };
    extern SaveFile gSaveFile;
    inline auto getSaveFileFields() {
        return std::array{
            &gSaveFile.qualityOfLife,
            &gSaveFile.skipIntro,
            &gSaveFile.instantText
        };
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
    inline void save() {
        if (!filesystem::FileHandler::MountSD()) { 
            hk::diag::log("Failed to mount SD");
            return;
        }
        if (!filesystem::FileHandler::WriteFile(cSaveFilePath, serialzeSaveFile())) {
            hk::diag::log("Failed to write save file");
            return;
        }
    }
    inline void deserializeSaveFile(std::string data) {
        nlohmann::json json = nlohmann::json::parse(data);
        auto fields = getSaveFileFields();
        for (auto field : fields) {
            if (!json.contains(field->serialName)) {
                continue;
            }
            field->value = json[field->serialName];
        }
    }
    inline void load() {
        if (!filesystem::FileHandler::MountSD()) { 
            hk::diag::log("Failed to mount SD");
            return;
        }
        std::string data;
        if (!filesystem::FileHandler::ReadFile(cSaveFilePath, data)) {
            hk::diag::log("Failed to read save file");
            return;
        }
        deserializeSaveFile(data);
    }
}

