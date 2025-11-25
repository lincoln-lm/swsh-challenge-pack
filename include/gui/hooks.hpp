#pragma once
#include "hk/gfx/DebugRenderer.h"
#include "nvn/nvn_CppMethods.h" // IWYU pragma: keep
namespace gui {
    void drawFrame(hk::gfx::DebugRenderer* renderer);
    void installHooks();
}