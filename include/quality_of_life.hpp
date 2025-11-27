#pragma once
#include "hk/hook/Trampoline.h"
#include "nn/hid.h"
#include "orion/movie/BSeqHandler.hpp"
#include "orion/options/OptionsHolder.hpp"
#include "orion/pawn/Pawn.hpp"
#include "save/SaveFile.hpp"
#include "gui/InputManager.hpp"

inline HkTrampoline<void, orion::movie::BSeqHandler*, u64> skipBSeq = hk::hook::trampoline([](orion::movie::BSeqHandler* this_, u64 param_1) -> void {
    if (save::gSaveFile.qualityOfLife && save::gSaveFile.skipIntro) {
        if (auto gf_file = this_->gfFile; gf_file != nullptr) {
            const char* file_path = gf_file->filePath;
            if (
                // skipping the title screen breaks things
                strcmp(file_path, "bin/demo/sequence/sd9010_title.bseq")
                // skipping evolutions break things
                && strstr(file_path, "evolution") == nullptr
            ) {
                auto bseq_header = this_->bSeqHeader;

                // make all sequences effectively read as empty
                bseq_header->frameCount = 0;
                bseq_header->groupOptionCount = 0;
                bseq_header->hashSizeCount = 0;
            }
        }
    }
    skipBSeq.orig(this_, param_1);
});

inline HkTrampoline<orion::options::TextSpeed, orion::options::OptionsHolder*> instantText = hk::hook::trampoline([](orion::options::OptionsHolder* this_) -> orion::options::TextSpeed {
    if (save::gSaveFile.qualityOfLife && save::gSaveFile.instantText) {
        // out of range setting causes instant text
        return orion::options::TextSpeed::Invalid;
    }
    return instantText.orig(this_);
});

inline HkTrampoline<orion::pawn::ucell, orion::pawn::AMX*> skipTextWait = hk::hook::trampoline([](orion::pawn::AMX* amx) -> orion::pawn::ucell {
    if (save::gSaveFile.qualityOfLife && save::gSaveFile.instantText && gui::InputManager::isPressed(nn::hid::NpadButton::B)) {
        return 1;
    }
    return skipTextWait.orig(amx);
});

inline void installQualityOfLifeHooks() {
    skipBSeq.installAtPtr(pun<void*>(&orion::movie::BSeqHandler::Deserialize));
    instantText.installAtPtr(pun<void*>(&orion::options::OptionsHolder::GetTextSpeed));
    skipTextWait.installAtPtr(pun<void*>(&orion::pawn::ABKeyWait_));
}