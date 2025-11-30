#pragma once
#include "hk/types.h"
#include "orion/field/PokeInfo.hpp"

namespace orion::field {
    struct Party {
        virtual ~Party();
        virtual int func_0x10();
        virtual int func_0x18();
        virtual int func_0x20();
        virtual bool AddMember(void*);
        virtual int func_0x30();
        virtual void RemoveMember(u32 index);
        virtual int func_0x40();
        virtual int func_0x48();
        virtual PokeInfo* GetMember(u32 index);
        virtual PokeInfo* GetMember2(u32 index);
        virtual bool IsFull();
        virtual void Copy(Party* from);
        virtual void Clear();

        u8 unk0[0x50];
        PokeInfo* members[6];
        u8 size;
    };
}