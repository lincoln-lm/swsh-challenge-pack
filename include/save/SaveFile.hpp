#pragma once

#include "hk/types.h"
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
    };
    extern SaveFile gSaveFile;
    inline auto getSaveFileFields() {
        return std::array{&gSaveFile.qualityOfLife, &gSaveFile.skipIntro};
    }
}

