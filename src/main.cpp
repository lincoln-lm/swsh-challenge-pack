#include "gui/hooks.hpp"
#include "gui/SettingsMenu.hpp"
#include "gui/InputManager.hpp"
#include "quality_of_life.hpp"


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

extern "C" void hkMain()
{
    installQualityOfLifeHooks();
    gui::installHooks();
}