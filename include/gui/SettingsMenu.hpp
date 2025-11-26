#pragma once
#include "hk/gfx/DebugRenderer.h"

namespace gui {
    struct Setting {
        const char* name;
        bool enabled;
    };
    namespace SettingsMenu {
        void inputHandling();
        void draw(hk::gfx::DebugRenderer* renderer);
    };
}