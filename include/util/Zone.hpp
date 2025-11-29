#pragma once
#include "util/Data.hpp"

namespace util {
    inline u64 lookupZone(u64 object) {
        for (auto item : data::OBJECT_TO_ZONE) {
            if (std::get<0>(item) == object) {
                return std::get<1>(item);
            }
        }
        return 0;
    }
}