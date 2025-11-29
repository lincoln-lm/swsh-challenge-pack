#pragma once

#include "orion/save/MiscBlock.hpp"
namespace orion::capture {
    u32 GetLevelCap();
    u32 GetLevelCap(save::MiscBlock* miscBlock);
};