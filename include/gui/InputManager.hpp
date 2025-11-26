#pragma once
#include "nn/hid.h"

namespace gui::InputManager {
    void lockInput();
    void unlockInput();
    void updateControllerState();
    bool isPressed(nn::hid::NpadButton button);
    bool isJustPressed(nn::hid::NpadButton button);
    nn::hid::NpadBaseState* getCurrentState();
    nn::hid::NpadBaseState* getLastState();
    void installInputHooks();
}