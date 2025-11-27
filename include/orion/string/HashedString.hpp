#pragma once
#include "hk/types.h"
namespace orion::string {
    struct HashedString {
        u64 hash;
        const char* string;
        u64 length;
        u64 unk;
    };
}