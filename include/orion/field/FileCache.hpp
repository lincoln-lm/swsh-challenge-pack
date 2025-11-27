#pragma once
#include "hk/types.h"
#include "orion/filesystem/GFFile.hpp"
namespace orion::field {
    struct FileCache {
        struct CacheFileStack_t {
            u64 unk0;
            filesystem::GFFile* gfFile;
            // ...
        };
        void CacheFile(CacheFileStack_t* fileHolder, int category);
    };
}