#include "gui/Tracker.hpp"
#include "gui/ExtendedRenderer.hpp"
#include "gui/InputManager.hpp"
#include "hk/util/Math.h"
#include "orion/field/SystemManager.hpp"
#include "orion/save/SaveManager.hpp"
#include "util/Data.hpp"
#include "nn/hid.h"
#include "util/Personal.hpp"
#include <algorithm>
#include <numeric>

namespace gui::Tracker {
    constexpr static size cGlyphHeight = 32;
    constexpr static size cGlyphWidth = 16;
    constexpr static size cRowCount = 14;
    constexpr static size cLineLength = 48;
    constexpr static size cMargin = 30;
    constexpr static size cPadding = 20;
    constexpr static size cWidth = cPadding + cLineLength * cGlyphWidth + cPadding;
    constexpr static size cHeight = cPadding + cRowCount * cGlyphHeight + cPadding;
    constexpr static u32 cTextColor = 0xffffffff;
    constexpr static u32 cRedTextColor = 0xff0000ff;
    constexpr static u32 cGreenTextColor = 0xff00ff00;
    constexpr static u32 cWindowColor = 0xef000000;
    constexpr static size cMaxPokemon = 6;
    constexpr static size cConfirmIndex = 0;
    bool isOpen = false;
    size selectedPokemon = 0;

    OnCloseFunction onClose = nullptr;

    void open(OnCloseFunction onCloseFn) {
        InputManager::lockInput();
        onClose = onCloseFn;
        isOpen = true;
    }

    void close() {
        InputManager::unlockInput();
        isOpen = false;
        if (onClose != nullptr) {
            onClose();
        }
    }

    bool getIsOpen() {
        return isOpen;
    }

    void inputHandling() {
        if (!isOpen) return;
        if (InputManager::isJustPressed(nn::hid::NpadButton::Up) || InputManager::isJustPressed(nn::hid::NpadButton::StickLUp)) {
            selectedPokemon = std::clamp<size>(selectedPokemon - 1, 0, cMaxPokemon + 5) % cMaxPokemon;
        }
        if (InputManager::isJustPressed(nn::hid::NpadButton::Down) || InputManager::isJustPressed(nn::hid::NpadButton::StickLDown)) {
            selectedPokemon = std::clamp<size>(selectedPokemon + 1, 0, cMaxPokemon + 5) % cMaxPokemon;
        }
        if (InputManager::isJustPressed(nn::hid::NpadButton::B)) {
            close();
        }
    }
    void draw(hk::gfx::DebugRenderer* renderer)
    {
        if (!isOpen) return;
        ExtendedRenderer ext_renderer(renderer);
        renderer->setGlyphSize(1.0);

        ext_renderer.drawRect(cMargin, cMargin, cWidth, cHeight, cWindowColor, 25.0f);
        renderer->setCursor({ cMargin + cPadding, cMargin + cPadding });

        auto member = orion::field::SystemManager::instance()->mSystemData->mParty->GetMember(selectedPokemon);
        if (member == nullptr) {
            renderer->setPrintColor(cTextColor);
            renderer->printf("Slot %d: No Pokemon\n", selectedPokemon + 1);
            return;
        }
        renderer->printf("Slot %d: %s\n", selectedPokemon + 1, util::data::SPECIES_EN[member->GetSpecies()]);
        renderer->printf("HP: %d/%d\n", member->GetCurrentHP(), member->GetMaxHP());
        renderer->printf("Lv.%d\n", member->GetLevel());
        renderer->printf("%s\n", util::data::ITEMS_EN[member->GetHeldItem()]);
        renderer->printf("%s\n", util::data::ABILITIES_EN[member->GetAbility()]);
        renderer->printf("Heals in Bag:\n");

        auto max_hp = member->GetMaxHP();
        auto my_item = orion::save::SaveManager::instance()->mMyItem;
        f32 restore_percentage = 0.0f;
        for (auto item : my_item->mItems) {
            for (auto healing_item : util::data::HP_RESTORING_ITEMS) {
                if (item.id == healing_item.itemId) {
                    restore_percentage += (healing_item.percentHp + (healing_item.absoluteHp / (f32)max_hp)) * (f32)item.count;
                    break;
                }
            }
        }

        renderer->printf("%.0f%% HP\n", restore_percentage * 100.0f);

        renderer->setCursor({ cMargin + cPadding + cLineLength/2.0f * cGlyphWidth, cMargin + cPadding });
        const auto nature = member->GetNature();
        const u8 increased = nature / 5;
        const u8 decreased = nature % 5;
        const auto getColor = [increased, decreased](u8 stat) {
            if (increased == decreased) return cTextColor;
            if (decreased == stat) return cRedTextColor;
            if (increased == stat) return cGreenTextColor;
            return cTextColor;
        };
        renderer->printf("HP\t%d\n", member->GetMaxHP());
        renderer->setPrintColor(getColor(0));
        renderer->printf("ATK\t%d\n", member->GetAttack());
        renderer->setPrintColor(getColor(1));
        renderer->printf("DEF\t%d\n", member->GetDefense());
        renderer->setPrintColor(getColor(2));
        renderer->printf("SPA\t%d\n", member->GetSpecialAttack());
        renderer->setPrintColor(getColor(3));
        renderer->printf("SPD\t%d\n", member->GetSpecialDefense());
        renderer->setPrintColor(getColor(4));
        renderer->printf("SPE\t%d\n", member->GetSpeed());
        renderer->setPrintColor(cTextColor);
        auto base_stats = util::getBaseStats(member->GetSpecies(), member->GetForm());
        u16 bst = std::accumulate(base_stats.begin(), base_stats.end(), 0);
        renderer->printf("BST\t%d\n", bst);
    }
}