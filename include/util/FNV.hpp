#pragma once

#include "orion/string/HashedString.hpp"
namespace util {
    inline constexpr orion::string::HashedString fnv1aString(const char* str) {
        orion::string::HashedString result = {
            0xcbf29ce484222645,
            str,
            __builtin_strlen(str),
            0
        };
        for (size i = 0; i < result.length; i++) {
            result.hash = (result.hash ^ str[i]) * 0x100000001b3;
        }
        return result;
    }
    inline constexpr u64 fnv1a(const char* str) {
        return fnv1aString(str).hash;
    }
}