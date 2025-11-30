#pragma once
#include "hk/gfx/DebugRenderer.h"

namespace gui {
    namespace Tracker {
        void inputHandling();
        void draw(hk::gfx::DebugRenderer* renderer);

        typedef void (*OnCloseFunction)();
        void open(OnCloseFunction onClose);
        void close();
        bool getIsOpen();
    };
}