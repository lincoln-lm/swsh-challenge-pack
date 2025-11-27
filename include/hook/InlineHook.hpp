#pragma once
#include "hk/hook/InstrUtil.h"
namespace hook {
    struct CpuState {
        u64 X[31];
        u64 SP;
    };
    static_assert(sizeof(CpuState) == 0x100);
    template <typename Fn>
    class InlineHook {
        public:
        using Callback = void (*)(CpuState* state);
        static inline Callback callback;
        InlineHook(Fn func) {
            callback = +func;
        }
        void installAtPtrOffset(ptr base, ptr offset) {
            hk::hook::writeBranchLinkAtPtr(base + offset, hook);
        }
        // TODO: uninstall
        private:
        __attribute__((naked))
        static void hook() {
            #pragma clang diagnostic push
            #pragma clang diagnostic ignored "-Winline-asm"
            asm volatile(
                "sub sp, sp, #(31*8 + 8)\n"

                // store registers
                "stp x0, x1, [sp, 0]\n"
                "stp x2, x3, [sp, 16]\n"
                "stp x4, x5, [sp, 32]\n"
                "stp x6, x7, [sp, 48]\n"
                "stp x8, x9, [sp, 64]\n"
                "stp x10, x11, [sp, 80]\n"
                "stp x12, x13, [sp, 96]\n"
                "stp x14, x15, [sp, 112]\n"
                "stp x16, x17, [sp, 128]\n"
                "stp x18, x19, [sp, 144]\n"
                "stp x20, x21, [sp, 160]\n"
                "stp x22, x23, [sp, 176]\n"
                "stp x24, x25, [sp, 192]\n"
                "stp x26, x27, [sp, 208]\n"
                "stp x28, x29, [sp, 224]\n"
                "str x30, [sp, 240]\n"
                "mov x0, sp\n"
                "str x0, [sp, 248]\n"
                // TODO: fp registers
                :
                :
                : "x0","x1","x2","x3","x4","x5","x6","x7",
                "x8","x9","x10","x11","x12","x13","x14","x15",
                "x16","x17","x18","x19","x20","x21","x22","x23",
                "x24","x25","x26","x27","x28","x29","x30",
                "memory"
            );
            asm volatile(
                "blr %x0\n"
                :
                : "r"(callback)
            );
            asm volatile(
                // restore registers
                "ldp x0, x1, [sp, 0]\n"
                "ldp x2, x3, [sp, 16]\n"
                "ldp x4, x5, [sp, 32]\n"
                "ldp x6, x7, [sp, 48]\n"
                "ldp x8, x9, [sp, 64]\n"
                "ldp x10, x11, [sp, 80]\n"
                "ldp x12, x13, [sp, 96]\n"
                "ldp x14, x15, [sp, 112]\n"
                "ldp x16, x17, [sp, 128]\n"
                "ldp x18, x19, [sp, 144]\n"
                "ldp x20, x21, [sp, 160]\n"
                "ldp x22, x23, [sp, 176]\n"
                "ldp x24, x25, [sp, 192]\n"
                "ldp x26, x27, [sp, 208]\n"
                "ldp x28, x29, [sp, 224]\n"
                "ldr x30, [sp, 240]\n"

                "add sp, sp, #(31*8 + 8)\n"
                "ret\n"
            );
            #pragma clang diagnostic pop
        }
    };
    template <typename Fn>
    InlineHook<Fn> inlineHook(Fn func) {
        return InlineHook<Fn>(func);
    }
}