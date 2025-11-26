#pragma once

#include <nn/hid.h>

namespace nn::hid::detail {
    int GetNpadStates(int*, NpadFullKeyState*, s32, const u32& port);
    int GetNpadStates(int*, NpadHandheldState*, s32, const u32& port);
    int GetNpadStates(int*, NpadJoyDualState*, s32, const u32& port);
    int GetNpadStates(int*, NpadJoyLeftState*, s32, const u32& port);
    int GetNpadStates(int*, NpadJoyRightState*, s32, const u32& port);
}