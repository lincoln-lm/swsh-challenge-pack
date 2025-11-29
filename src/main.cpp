#include "event_encounters.hpp"
#include "evolutions.hpp"
#include "field_items.hpp"
#include "gift_tms.hpp"
#include "gui/hooks.hpp"
#include "gui/SettingsMenu.hpp"
#include "gui/InputManager.hpp"
#include "hk/mem/BssHeap.h"
#include "hk/hook/Trampoline.h"
#include "level_cap.hpp"
#include "orion/field/FieldManager.hpp"
#include "learnset.hpp"
#include "gift_encounters.hpp"
#include "pokemon_model.hpp"
#include "quality_of_life.hpp"
#include "route_restriction.hpp"
#include "save_detection.hpp"
#include "shop_filter.hpp"
#include "trainer_teams.hpp"
#include "wild_encounters.hpp"
#include "personal_total.hpp"

extern "C" {
    void* __libc_malloc(std::size_t size) {
        return hk::mem::sMainHeap.allocate(size);
    }
    void __libc_free(void* ptr) {
        hk::mem::sMainHeap.free(ptr);
    }
}

void gui::onFrame(hk::gfx::DebugRenderer* renderer) {
    InputManager::updateControllerState();
    if (!SettingsMenu::getIsOpen()) {
        return;
    }
    SettingsMenu::inputHandling();
    SettingsMenu::draw(renderer);
}

void installModHooks() {
    installQualityOfLifeHooks();
    installPokemonModelHooks();
    installGiftEncountersHooks();
    installEventEncountersHooks();
    installWildEncountersHooks();
    installTrainerTeamsHooks();
    installEvolutionsHooks();
    installFieldItemsHooks();
    installLearnsetHooks();
    installPersonalTotalHooks();
    installShopFilterHooks();
    installGiftTMsHooks();
    installLevelCapHooks();
    installRouteRestrictionHooks();
    installSaveDetectionHooks();
}

HkTrampoline<orion::field::FieldManager*, orion::field::FieldManager*, int, void*, u64> onGameInit = hk::hook::trampoline([](orion::field::FieldManager* this_, int param_1, void* param_2, u64 param_3) -> orion::field::FieldManager* {
    gui::SettingsMenu::open(installModHooks);
    return onGameInit.orig(this_, param_1, param_2, param_3);
});

extern "C" void hkMain()
{
    // arbitrary function only called once at game init some time past nnMain
    onGameInit.installAtPtr(pun<void*>(&orion::field::FieldManager::ctor));
    gui::installHooks();
}