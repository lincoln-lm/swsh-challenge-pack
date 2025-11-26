#include "hk/types.h"
#include "nn/hid.h"
namespace gui::InputManager {
    static nn::hid::NpadBaseState last_state;
    static nn::hid::NpadBaseState current_state;
    // only call once per frame
    void updateControllerState() {
        u64 port = 0;
        last_state = current_state;
        nn::hid::NpadStyleSet style_set = nn::hid::GetNpadStyleSet(port);
        if (style_set.Test((int) nn::hid::NpadStyleTag::NpadStyleFullKey)) {
            nn::hid::GetNpadState((nn::hid::NpadFullKeyState *)&current_state, port);
        } else if (style_set.Test((int) nn::hid::NpadStyleTag::NpadStyleJoyDual)) {
            nn::hid::GetNpadState((nn::hid::NpadJoyDualState *)&current_state, port);
        } else if (style_set.Test((int) nn::hid::NpadStyleTag::NpadStyleJoyLeft)) {
            nn::hid::GetNpadState((nn::hid::NpadJoyLeftState *)&current_state, port);
        } else if (style_set.Test((int) nn::hid::NpadStyleTag::NpadStyleJoyRight)) {
            nn::hid::GetNpadState((nn::hid::NpadJoyRightState *)&current_state, port);
        } else {
            nn::hid::GetNpadState((nn::hid::NpadHandheldState *)&current_state, 0x20);
        }
    }
    bool isPressed(nn::hid::NpadButton button) {
        return current_state.mButtons.Test((int)button);
    }
    bool isJustPressed(nn::hid::NpadButton button) {
        return current_state.mButtons.Test((int)button) && !last_state.mButtons.Test((int)button);
    }
    nn::hid::NpadBaseState* getCurrentState() {
        return &current_state;
    }
    nn::hid::NpadBaseState* getLastState() {
        return &last_state;
    }
}