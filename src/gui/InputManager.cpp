#include "hk/hook/Trampoline.h"
#include "hk/types.h"
#include "nn/hid.h"
#include "nn/hid_detail.h"

namespace gui::InputManager {
    static nn::hid::NpadBaseState last_state;
    static nn::hid::NpadBaseState current_state;
    static bool isInputLocked = false;
    static bool isReadingInput = false;
    void lockInput() {
        isInputLocked = true;
    }
    void unlockInput() {
        isInputLocked = false;
    }
    // only call once per frame
    void updateControllerState() {
        u64 port = 0;
        last_state = current_state;
        isReadingInput = true;
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
        isReadingInput = false;
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

    #define INPUT_HOOK(type) \
    static HkTrampoline<int, int*, nn::hid::Npad##type*, u32, u32 const&> Disable##type = hk::hook::trampoline([](int *unkInt, nn::hid::Npad##type *state, u32 count, u32 const &port){ \
        int result = Disable##type.orig(unkInt, state, count, port); \
        if (isInputLocked && !isReadingInput) { \
            auto tmp = state->mAttributes;\
            *state = nn::hid::Npad##type(); \
            state->mAttributes = tmp;\
        } \
        return result; \
    });

    INPUT_HOOK(FullKeyState);
    INPUT_HOOK(HandheldState);
    INPUT_HOOK(JoyDualState);
    INPUT_HOOK(JoyLeftState);
    INPUT_HOOK(JoyRightState);
    #undef INPUT_HOOK
    void installInputHooks() {
        DisableFullKeyState.installAtPtr(static_cast<int(*)(int*, nn::hid::NpadFullKeyState*, s32, const u32&)>(nn::hid::detail::GetNpadStates));
        DisableHandheldState.installAtPtr(static_cast<int(*)(int*, nn::hid::NpadHandheldState*, s32, const u32&)>(nn::hid::detail::GetNpadStates));
        DisableJoyDualState.installAtPtr(static_cast<int(*)(int*, nn::hid::NpadJoyDualState*, s32, const u32&)>(nn::hid::detail::GetNpadStates));
        DisableJoyLeftState.installAtPtr(static_cast<int(*)(int*, nn::hid::NpadJoyLeftState*, s32, const u32&)>(nn::hid::detail::GetNpadStates));
        DisableJoyRightState.installAtPtr(static_cast<int(*)(int*, nn::hid::NpadJoyRightState*, s32, const u32&)>(nn::hid::detail::GetNpadStates));
    }
}