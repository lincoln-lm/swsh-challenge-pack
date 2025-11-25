#pragma once
#include <vector>
#include "hk/types.h"
#include "FieldObject.hpp"
namespace orion {
namespace field {
    struct FieldManager {
        static FieldManager* sInstance;
        u8 unk0[0xb0];
        std::vector<FieldObject*> fieldObjects;
        u64 unk1;

        inline static FieldManager* instance() {
            return sInstance;
        }
        inline static std::vector<FieldObject*>& getFieldObjects() {
            return sInstance->fieldObjects;
        }
    };
    static_assert(offsetof(FieldManager, fieldObjects) == 0xb0);
    static_assert(sizeof(FieldManager) == 0xd0);
}
}