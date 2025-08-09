#include "amx_handler.hpp"
#include "lib.hpp"
#include "external.hpp"
#include "savefile.hpp"
#include "rng_manager.hpp"
#include "symbols.hpp"
#include "util.hpp"

struct TM_SCRIPT_IDENTIFIER {
    std::string identifier;
    u32 cip;
    u64 old;
    u32 arg;
};

// TODO: verify source amx? probably dont need to
// TODO: rest of tms
static std::vector<TM_SCRIPT_IDENTIFIER> TM_IDENTIFIERS = {
    { "hop_swift", 0x9258, 367, 0 },
    { "hop_swift", 0x92B8, 367, 0 },
    { "hop_swift", 0x21d8, 367, 2 },
    { "hop_swift", 0x8ff8, 367, 0 },
    { "hop_swift", 0x2228, 367, 2 },
    { "hop_swift", 0x94d0, 367, 0 },
    { "hop_swift", 0x8af8, 367, 0 },
    { "hop_swift", 0x8b90, 367, 0 },
    { "hop_swift", 0xbd80, 367, 0 },

    { "milo_magical_leaf", 0xb030, 337, 0 },
    { "milo_magical_leaf", 0xb090, 337, 0 },
    { "milo_magical_leaf", 0x2a00, 337, 2 },
    { "milo_magical_leaf", 0xadd0, 337, 0 },
    { "milo_magical_leaf", 0x2a50, 337, 2 },
    { "milo_magical_leaf", 0xb2a8, 337, 0 },
    { "milo_magical_leaf", 0xa6c8, 337, 0 },
    { "milo_magical_leaf", 0xa760, 337, 0 },
    { "milo_magical_leaf", 0x10b88, 337, 0 },

    { "nessa_whirlpool", 0xa9d0, 363, 0 },
    { "nessa_whirlpool", 0xaa30, 363, 0 },
    { "nessa_whirlpool", 0x2a88, 363, 2 },
    { "nessa_whirlpool", 0xa770, 363, 0 },
    { "nessa_whirlpool", 0x2ad8, 363, 2 },
    { "nessa_whirlpool", 0xac48, 363, 0 },
    { "nessa_whirlpool", 0xa068, 363, 0 },
    { "nessa_whirlpool", 0xa100, 363, 0 },
    { "nessa_whirlpool", 0xff20, 363, 0 },

    { "kabu_will_o_wisp", 0x9be0, 365, 0 },
    { "kabu_will_o_wisp", 0x9c40, 365, 0 },
    { "kabu_will_o_wisp", 0x2358, 365, 2 },
    { "kabu_will_o_wisp", 0x9980, 365, 0 },
    { "kabu_will_o_wisp", 0x23a8, 365, 2 },
    { "kabu_will_o_wisp", 0x9e58, 365, 0 },
    { "kabu_will_o_wisp", 0x9278, 365, 0 },
    { "kabu_will_o_wisp", 0x9310, 365, 0 },
    { "kabu_will_o_wisp", 0xbb00, 365, 0 },

    { "allister_hex", 0xa2e8, 404, 0 },
    { "allister_hex", 0xa348, 404, 0 },
    { "allister_hex", 0x25f8, 404, 2 },
    { "allister_hex", 0xa088, 404, 0 },
    { "allister_hex", 0x2648, 404, 2 },
    { "allister_hex", 0xa560, 404, 0 },
    { "allister_hex", 0x9980, 404, 0 },
    { "allister_hex", 0x9a18, 404, 0 },
    { "allister_hex", 0xeb20, 404, 0 },

    { "opal_draining_kiss", 0xab98, 414, 0 },
    { "opal_draining_kiss", 0xabf8, 414, 0 },
    { "opal_draining_kiss", 0x2550, 414, 2 },
    { "opal_draining_kiss", 0xa938, 414, 0 },
    { "opal_draining_kiss", 0x25a0, 414, 2 },
    { "opal_draining_kiss", 0xae10, 414, 0 },
    { "opal_draining_kiss", 0xa230, 414, 0 },
    { "opal_draining_kiss", 0xa2c8, 414, 0 },
    { "opal_draining_kiss", 0xf830, 414, 0 },

    { "fake_tears", 0x90a0, 374, 0 },
    { "fake_tears", 0x9680, 374, 0 },
    { "fake_tears", 0x96e0, 374, 0 },
    { "fake_tears", 0x1908, 374, 2 },
    { "fake_tears", 0x9420, 374, 0 },
    { "fake_tears", 0x1958, 374, 2 },
    { "fake_tears", 0x98f8, 374, 0 },
    { "fake_tears", 0x8f20, 374, 0 },
    { "fake_tears", 0x8fb8, 374, 0 },
    { "fake_tears", 0x9260, 374, 0 },

    { "screech", 0x90a0, 343, 0 },
    { "screech", 0x9680, 343, 0 },
    { "screech", 0x96e0, 343, 0 },
    { "screech", 0x1908, 343, 2 },
    { "screech", 0x9420, 343, 0 },
    { "screech", 0x1958, 343, 2 },
    { "screech", 0x98f8, 343, 0 },
    { "screech", 0x8f20, 343, 0 },
    { "screech", 0x8fb8, 343, 0 },
    { "screech", 0x9260, 343, 0 },
};

void replace_tm(AMX::AMX_INSTANCE* instance, u64* args) {
    if (!save_file.gift_tms_rng.enabled) return;
    u64 arg_count = args[0] / 8;
    for (auto& identifier : TM_IDENTIFIERS) {
        if (instance->cip != identifier.cip)
            continue;
        if (identifier.arg >= arg_count)
            continue;
        if (args[identifier.arg + 1] != identifier.old)
            continue;
        auto rng = RngManager::NewRandomGenerator(identifier.identifier);
        args[identifier.arg + 1] = rng.RandTM();
    }
}

void install_gift_tms_patch() {
    AMX::add_amx_callback(&replace_tm);
}
