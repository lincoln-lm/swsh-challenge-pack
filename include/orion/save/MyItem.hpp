#pragma once
#include "hk/types.h"

namespace orion::save {
    struct MyItem {
        struct Item {
            u32 id : 15;
            u32 count : 15;
            u32 isNew : 1;
            u32 isFavorite : 1;
        };
        Item mItems[1214];
        // ...
    };
}