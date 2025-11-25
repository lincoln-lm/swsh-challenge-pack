#include "hk/gfx/DebugRenderer.h"
#include "hk/hook/Trampoline.h"
#include "nvn/nvn_Cpp.h"
#include "nvn/nvn_CppMethods.h" // IWYU pragma: keep
#include "orion/graphics/FinalizeRenderer.hpp"

static nvn::CommandBuffer* queued_command_buffer = nullptr;

HkTrampoline<void, orion::graphics::FinalizeHolder*> finalizeDrawScreen = hk::hook::trampoline([](orion::graphics::FinalizeHolder* this_) -> void {
    finalizeDrawScreen.orig(this_);
    auto renderer = hk::gfx::DebugRenderer::instance();
    // hijack the FinalizeRenderer's command buffer
    auto command_buffer = static_cast<nvn::CommandBuffer*>(
        this_
            ->renderer
            ->command_buffer_container
            ->holders[1 ^ orion::graphics::ActiveCommandBufferIndex]
            .command_buffer
            .ToData()
            ->pNvnCommandBuffer);
    // store it so we can submit our commands in a later accessible queue
    queued_command_buffer = command_buffer;
    renderer->clear();
    command_buffer->BeginRecording();
    renderer->begin(command_buffer);

    renderer->setGlyphSize(0.45);

    renderer->drawQuad(
        { { 30, 30 }, { 0, 0 }, 0xef000000 },
        { { 300, 30 }, { 1.0, 0 }, 0xef000000 },
        { { 300, 100 }, { 1.0, 1.0 }, 0xef000000 },
        { { 30, 100 }, { 0, 1.0 }, 0xef000000 });

    renderer->setCursor({ 50, 50 });

    renderer->printf("Hello World!\n");
});

HkTrampoline<void, nvn::Queue*, nvn::Window*, int> nvnQueuePresentTextureTrampoline = hk::hook::trampoline([](nvn::Queue* queue, nvn::Window* window, int texIndex) -> void {
    // hijack the Queue the next time queuePresentTexture is called
    if (queued_command_buffer != nullptr) {
        auto* renderer = hk::gfx::DebugRenderer::instance();

        auto handle = queued_command_buffer->EndRecording();
        renderer->end();

        queue->SubmitCommands(1, &handle);
        queued_command_buffer = nullptr;
    }
    nvnQueuePresentTextureTrampoline.orig(queue, window, texIndex);
});

extern "C" void hkMain()
{
    finalizeDrawScreen.installAtPtr(pun<void*>(&orion::graphics::FinalizeHolder::FinalizeDrawScreen));
    nvnQueuePresentTextureTrampoline.installAtPtr(nvnBootstrapLoader("nvnQueuePresentTexture"));
    hk::gfx::DebugRenderer::instance()->installHooks();
}