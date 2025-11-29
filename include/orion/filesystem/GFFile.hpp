#pragma once
#include "hk/types.h"
#include "orion/field/FieldObject.hpp"
namespace orion::filesystem {
    struct GFFile {
        u8 unk0[0x68 + 0x8];
        const char* filePath;
        // ...
        u8 unk1[0x180 - 0x68 - 0x8 - 0x8];
        size fileSize;
        size filePosition;
        void* buffer;
        Allocator* activeAllocator;
        size bufferSize;
        size bufferAlignment;
        void WriteToDisk();
    };
    static_assert(offsetof(GFFile, filePath) == 0x68 + 0x8);
    static_assert(offsetof(GFFile, fileSize) == 0x180);
    static_assert(offsetof(GFFile, buffer) == 0x190);
}