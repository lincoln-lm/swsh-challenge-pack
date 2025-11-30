#pragma once

#include "hk/hook/Trampoline.h"
#include "mod_hooks.hpp"
#include "orion/filesystem/GFFile.hpp"
#include "save/SaveFile.hpp"

inline HkTrampoline<void, orion::filesystem::GFFile*> detectSave = hk::hook::trampoline([](orion::filesystem::GFFile* this_) -> void {
    if (sHooksEnabled && std::strcmp(this_->filePath, "main") == 0) {
        hk::diag::log("Detected game save");
        save::save();
    }
    detectSave.orig(this_);
});

inline void installSaveDetectionHooks() {
    detectSave.installAtPtr(pun<void*>(&orion::filesystem::GFFile::WriteToDisk));
}