#include "gui/SettingsMenu.hpp"
#include "gui/ExtendedRenderer.hpp"
#include "gui/InputManager.hpp"
#include "hk/util/Math.h"
#include "nn/hid.h"
#include "save/SaveFile.hpp"
#include <array>
namespace gui::SettingsMenu {
    constexpr static size cGlyphHeight = 32;
    constexpr static size cGlyphWidth = 16;
    constexpr static size cRowCount = 8;
    constexpr static size cLineLength = 48;
    constexpr static size cMargin = 30;
    constexpr static size cPadding = 20;
    constexpr static size cWidth = cPadding + cLineLength * cGlyphWidth + cPadding;
    constexpr static size cHeight = cPadding + cRowCount * cGlyphHeight + cPadding;
    constexpr static u32 cTextColor = 0xffffffff;
    constexpr static u32 cSelectedTextColor = 0xffffd700;
    constexpr static u32 cWindowColor = 0xef000000;
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
                save::save();
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
        ExtendedRenderer ext_renderer(renderer);
        renderer->setGlyphSize(1.0);

        // main window

        ext_renderer.drawRect(cMargin, cMargin, cWidth, cHeight, cWindowColor, 25.0f);
        renderer->setCursor({ cMargin + cPadding, cMargin + cPadding });

        auto entries = save::getSaveFileFields();

        lastNumEntries = 0;
        if (scrollOffset == 0) {
            if (selectedIndex == lastNumEntries) {
                renderer->setPrintColor(cSelectedTextColor);
            } else {
                renderer->setPrintColor(cTextColor);
            }
            renderer->printf("Confirm\n");
        }
        lastNumEntries++;

        for (size i = 0; i < entries.size(); i++) {
            auto entry = entries[i];
            indexLookup[lastNumEntries] = i;
            if (scrollOffset <= lastNumEntries && lastNumEntries < scrollOffset + cRowCount) {
                if (lastNumEntries == selectedIndex) {
                    renderer->setPrintColor(cSelectedTextColor);
                } else {
                    renderer->setPrintColor(cTextColor);
                }
                hk::diag::log(entry->toString().c_str());
                renderer->printf(
                    "%s%s\r\n",
                    entry->indented ? "\t" : "",
                    entry->toString().c_str()
                );
            }
            lastNumEntries++;
            // new collapsed category
            if (!entry->indented && !*entry) {
                // skip over indented entries
                while (++i < entries.size() && entries[i--]->indented) {i++;}
            }
        }

        constexpr size scroll_indicator_x = cMargin + cWidth - cPadding - cGlyphWidth;
        constexpr size scroll_indicator_y_top = cMargin + cPadding;
        constexpr size scroll_indicator_y_bottom = cMargin + cHeight - cPadding - cGlyphHeight;
        if (scrollOffset > 0) {
            renderer->setCursor({ scroll_indicator_x,  scroll_indicator_y_top});
            renderer->setPrintColor(cTextColor);
            renderer->printf("^\n");
        }
        if (scrollOffset + cRowCount < lastNumEntries) {
            renderer->setCursor({ scroll_indicator_x, scroll_indicator_y_bottom});
            renderer->setPrintColor(cTextColor);
            renderer->printf("v\n");
        }

        // description window

        ext_renderer.drawRect(cMargin, cMargin + cMargin + cHeight, cWidth, cHeight, cWindowColor, 25.0f);
        renderer->setCursor({ cMargin + cPadding, cMargin + cMargin + cPadding + cHeight });
        renderer->setPrintColor(cTextColor);
        if (selectedIndex == cConfirmIndex) {
            ext_renderer.printWrap<cLineLength>("Confirm and save settings");
        } else {
            ext_renderer.printWrap<cLineLength>(entries[indexLookup[selectedIndex]]->description);
        }
    }
}