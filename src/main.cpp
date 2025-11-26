#include "gui/hooks.hpp"
#include "gui/SettingsMenu.hpp"
#include "gui/InputManager.hpp"
#include "quality_of_life.hpp"


void gui::onFrame(hk::gfx::DebugRenderer* renderer) {
    InputManager::updateControllerState();
    SettingsMenu::inputHandling();
    SettingsMenu::draw(renderer);
}

extern "C" void hkMain()
{
    installQualityOfLifeHooks();
    gui::installHooks();
}