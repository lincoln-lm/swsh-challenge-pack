#pragma once
#include "orion/pawn/amx.h"
namespace orion::pawn {
    using AMX = tagAMX;
    ucell ABKeyWait_(AMX* amx, ucell* params);
    ucell IsMsgWinEnd_(AMX* amx, ucell* params);
}