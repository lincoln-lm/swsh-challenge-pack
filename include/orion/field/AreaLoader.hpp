#pragma once
#include "hk/types.h"
namespace orion::field {
    struct AreaLoader {
        void InitializeAreaCaches(void* areaFlatbuffer);
        void LoadAreaResources(void* param_1);
        bool PlaceObjects(u64* hash);
    }; // TODO
}