#include "gui/SettingsMenu.hpp"
#include "gui/InputManager.hpp"
namespace gui::SettingsMenu {
    Setting cEntries[] = {
        { "Evolution Randomization", true },
        { "Learnset Randomization", true },
        { "Item Randomization", true },
        { "Gift TM Randomization", true },
    };
    constexpr static size cNumEntries = hk::util::arraySize(cEntries);
    constexpr static size cRowCount = 8;
    constexpr static size cWidth = 800;
    constexpr static size cHeight = 20 + cRowCount * 32 + 20;
    size selectedIndex = 0;
    size scrollOffset = 0;
    void inputHandling() {
        if (InputManager::isJustPressed(nn::hid::NpadButton::Up)) {
            if (selectedIndex > 0) {
                selectedIndex--;
                if (selectedIndex < scrollOffset) {
                    scrollOffset--;
                }
            }
        }
        if (InputManager::isJustPressed(nn::hid::NpadButton::Down)) {
            if (selectedIndex + 1 < cNumEntries) {
                selectedIndex++;
                if (selectedIndex >= scrollOffset + cRowCount) {
                    scrollOffset++;
                }
            }
        }
    }
    void draw(hk::gfx::DebugRenderer* renderer)
    {
        renderer->setGlyphSize(1.0);
        renderer->drawQuad(
            { { 30, 30 }, { 0, 0 }, 0xef000000 },
            { { 30 + cWidth, 30 }, { 1.0, 0 }, 0xef000000 },
            { { 30 + cWidth, 30 + cHeight }, { 1.0, 1.0 }, 0xef000000 },
            { { 30, 30 + cHeight }, { 0, 1.0 }, 0xef000000 });
        renderer->setCursor({ 50, 50 });
        for (size i = scrollOffset; i < cNumEntries && i < scrollOffset + cRowCount; i++) {
            Setting& entry = cEntries[i];
            if (i == selectedIndex) {
                renderer->setPrintColor(0xffffd700);
            } else {
                renderer->setPrintColor(0xffffffff);
            }
            renderer->printf("%s: %s\n", entry.name, entry.enabled ? "Enabled" : "Disabled");
        }
        if (scrollOffset > 0) {
            renderer->setCursor({ 30 + cWidth - 30, 50 });
            renderer->setPrintColor(0xffffffff);
            renderer->printf("^\n");
        }
        if (scrollOffset + cRowCount < cNumEntries) {
            renderer->setCursor({ 30 + cWidth - 30, 30 + cHeight - 32 - 20});
            renderer->setPrintColor(0xffffffff);
            renderer->printf("v\n");
        }
    }
}