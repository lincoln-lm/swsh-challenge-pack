#include "gui/SettingsMenu.hpp"
#include "gui/InputManager.hpp"
#include "hk/util/Math.h"
#include "nn/hid.h"
#include "save/SaveFile.hpp"
#include <array>
namespace gui::SettingsMenu {
    constexpr static size cRowCount = 8;
    constexpr static size cWidth = 800;
    constexpr static size cHeight = 20 + cRowCount * 32 + 20;
    // TODO
    constexpr static size cMaxEntries = 0x20;
    constexpr static size cConfirmIndex = 0;
    bool isOpen = true;
    size indexLookup[cMaxEntries] = {0 };
    size selectedIndex = 0;
    size scrollOffset = 0;
    size lastNumEntries = 0;

    bool getIsOpen() {
        return isOpen;
    }

    void inputHandling() {
        if (InputManager::isJustPressed(nn::hid::NpadButton::Up) || InputManager::isJustPressed(nn::hid::NpadButton::StickLUp)) {
            if (selectedIndex > 0) {
                selectedIndex--;
                if (selectedIndex < scrollOffset) {
                    scrollOffset--;
                }
            }
        }
        if (InputManager::isJustPressed(nn::hid::NpadButton::Down) || InputManager::isJustPressed(nn::hid::NpadButton::StickLDown)) {
            if (selectedIndex + 1 < lastNumEntries) {
                selectedIndex++;
                if (selectedIndex >= scrollOffset + cRowCount) {
                    scrollOffset++;
                }
            }
        }
        if (InputManager::isJustPressed(nn::hid::NpadButton::A)) {
            if (selectedIndex == cConfirmIndex) {
                isOpen = false;
                return;
            }
            auto entries = save::getSaveFileFields();
            size target = indexLookup[selectedIndex];
            entries[target]->cycleForward();
        }
        if (InputManager::isJustPressed(nn::hid::NpadButton::B)) {
            scrollOffset = 0;
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

        auto entries = save::getSaveFileFields();

        lastNumEntries = 0;
        if (selectedIndex == lastNumEntries) {
            renderer->setPrintColor(0xffffd700);
        } else {
            renderer->setPrintColor(0xffffffff);
        }
        renderer->printf("Confirm\n");
        lastNumEntries++;

        for (size i = 0; i < entries.size(); i++) {
            auto entry = entries[i];
            indexLookup[lastNumEntries] = i;
            if (scrollOffset <= lastNumEntries && lastNumEntries < scrollOffset + cRowCount) {
                if (lastNumEntries == selectedIndex) {
                    renderer->setPrintColor(0xffffd700);
                } else {
                    renderer->setPrintColor(0xffffffff);
                }
                if (entry->indented) {
                    renderer->printf("\t");
                }
                renderer->printf("%s: %s\n", entry->displayName, *entry ? "Enabled" : "Disabled");
            }
            lastNumEntries++;
            // new collapsed category
            if (!entry->indented && !*entry) {
                // skip over indented entries
                while (++i < entries.size() && entries[i--]->indented) {i++;}
            }
        }

        if (scrollOffset > 0) {
            renderer->setCursor({ 30 + cWidth - 30, 50 });
            renderer->setPrintColor(0xffffffff);
            renderer->printf("^\n");
        }
        if (scrollOffset + cRowCount < lastNumEntries) {
            renderer->setCursor({ 30 + cWidth - 30, 30 + cHeight - 32 - 20});
            renderer->setPrintColor(0xffffffff);
            renderer->printf("v\n");
        }
    }
}