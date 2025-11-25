#pragma once
#include "hk/types.h"
#include <nn/gfx/gfx_CommandBuffer.h>
#include <nn/gfx/gfx_Device.h>
#include <nn/gfx/gfx_Types.h>
namespace orion {
namespace graphics {
    extern int ActiveCommandBufferIndex;
    struct CommandBufferHolder {
        nn::gfx::CommandBuffer command_buffer;
        u8 unk170[0x170 - sizeof(nn::gfx::CommandBuffer)];
    };
    static_assert(sizeof(CommandBufferHolder) == 0x170);
    struct CommandBufferContainer {
        u8 unk0[0x58];
        CommandBufferHolder* holders;
    };
    static_assert(offsetof(CommandBufferContainer, holders) == 0x58);
    struct FinalizeRenderer {
        u8 unk0[0x60];
        CommandBufferContainer* command_buffer_container;
        // ...
    };
    static_assert(offsetof(FinalizeRenderer, command_buffer_container) == 0x60);
    struct FinalizeHolder {
        void FinalizeDrawScreen();
        u64 unk0;
        FinalizeRenderer* renderer;
        // ...
    };
    static_assert(offsetof(FinalizeHolder, renderer) == 0x8);
}
}