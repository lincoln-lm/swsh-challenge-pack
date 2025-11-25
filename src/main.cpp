#include "gui/hooks.hpp"


void gui::drawFrame(hk::gfx::DebugRenderer* renderer) {
    renderer->setGlyphSize(1.0);

    renderer->drawQuad(
        { { 30, 30 }, { 0, 0 }, 0xef000000 },
        { { 630, 30 }, { 1.0, 0 }, 0xef000000 },
        { { 630, 186 }, { 1.0, 1.0 }, 0xef000000 },
        { { 30, 186 }, { 0, 1.0 }, 0xef000000 });

    renderer->setCursor({ 50, 50 });
    renderer->printf("Hello World!\n");
}


extern "C" void hkMain()
{
    gui::installHooks();
}