#pragma once

#include "hk/types.h"
namespace util {
    inline void* getVTable(void* obj) {
        return *pun<void**>(obj);
    }
}