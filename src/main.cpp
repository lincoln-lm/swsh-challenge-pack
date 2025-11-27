#include "gui/hooks.hpp"
#include "gui/SettingsMenu.hpp"
#include "gui/InputManager.hpp"
#include "hk/mem/BssHeap.h"
#include "hk/hook/Trampoline.h"
#include "orion/field/FieldManager.hpp"
#include "pokemon_model.hpp"
#include "quality_of_life.hpp"
#include "save/SaveFile.hpp"

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
    // TODO: better logic where locking/unlocking isnt done every frame
    if (!SettingsMenu::getIsOpen()) {
        InputManager::unlockInput();
        return;
    }
    InputManager::lockInput();
    SettingsMenu::inputHandling();
    SettingsMenu::draw(renderer);
}

HkTrampoline<orion::field::FieldManager*, orion::field::FieldManager*, int, void*, u64> onGameInit = hk::hook::trampoline([](orion::field::FieldManager* this_, int param_1, void* param_2, u64 param_3) -> orion::field::FieldManager* {
    save::load();
    return onGameInit.orig(this_, param_1, param_2, param_3);
});

extern "C" void hkMain()
{
    // arbitrary function only called once at game init some time past nnMain
    onGameInit.installAtPtr(pun<void*>(&orion::field::FieldManager::ctor));
    installQualityOfLifeHooks();
    installPokemonModelHooks();
    gui::installHooks();
}