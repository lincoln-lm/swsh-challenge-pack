#pragma once
#include "hk/types.h"
#include "orion/field/encounter/OverworldEncounterManager.hpp"
namespace orion {
    // TODO: better organize
    struct TypeInfo {
        const TypeInfo* parent;
    };
    struct Allocator {
        virtual ~Allocator();
        virtual void* Alloc(u64 size, u64 alignment);
    };
    // TODO: multiple vtables
    struct BaseObject {
        static void* sVTable[];
        virtual ~BaseObject();
        virtual int func_0x10();
        virtual int func_0x18();
        virtual int func_0x20();
        virtual int func_0x28();

        Allocator* allocator;
        u8 unk0[0x8];
        u64 size;
        u64 alignment;
        u8 unk1[0x28];
        // TODO
        u64* inheritance_function;
        u64 unk2;
    };
    static_assert(sizeof(BaseObject) == 0x60);

namespace field {
    struct WorldObject : public BaseObject {
        static void* sVTable[];
        virtual const TypeInfo* getTypeInfo() const;
        f32 unkMat[4][4];
        // TODO: vecs
        f32 rotation[4];
        f32 position[4];
        bool positionModified;
    };
    template <typename T>
    const TypeInfo* classTypeInfo() {
        static_assert(std::is_base_of_v<WorldObject, T>,
                  "T must inherit from WorldObject to have getTypeInfo");
        void* vtable_address = &T::sVTable;
        // pretend this is an object of type T
        // getTypeInfo doesnt access any members, so this is safe
        void* fake_object = &vtable_address;
        return reinterpret_cast<WorldObject*>(fake_object)->getTypeInfo();
    }
    template <typename T, typename U>
    bool isInstanceOf(U* object) {
        static_assert(std::is_base_of_v<WorldObject, U>,
                  "U must inherit from WorldObject to use isInstanceOf");
        const TypeInfo* class_type_info = classTypeInfo<T>();
        const TypeInfo* instance_type_info = object->getTypeInfo();
        while (instance_type_info != nullptr) {
            if (instance_type_info == class_type_info) return true;
            instance_type_info = instance_type_info->parent;
        }
        return false;
    }
    struct ScaledWorldObject : public WorldObject {
        static void* sVTable[];
        f32 unk3[3];
        f32 scale[4];
        u8 unk5[0xb0];
    };
    static_assert(sizeof(ScaledWorldObject) == 0x190);
    struct FieldObject : public ScaledWorldObject {
        static void* sVTable[];
        virtual int func_0x38();
        virtual int OnDespawn();
        virtual int func_0x48();
        virtual bool HandleRender(u64 param_2, bool value);
        virtual int func_0x58();
        virtual int func_0x60();
        virtual int func_0x68();
        virtual int func_0x70();
        virtual int func_0x78();
        virtual int func_0x80();
        virtual int func_0x88();
        virtual int OnTick();
        virtual int func_0x98();
        virtual int func_0xa0();
        virtual int func_0xa8();
        virtual int func_0xb0();
        virtual int func_0xb8();
        virtual int func_0xc0();
        virtual int func_0xc8();
        virtual int func_0xd0();
        virtual int func_0xd8();
        virtual int func_0xe0();
        virtual int func_0xe8();
        virtual int OnLoad();
        virtual int func_0xf8();
        virtual int func_0x100();
        virtual int func_0x108();
        virtual int func_0x110();
        virtual int func_0x118();
        virtual int func_0x120();
        virtual int func_0x128();
        virtual int func_0x130();
        virtual int func_0x138();
        virtual int func_0x140();
        virtual int func_0x148();
        virtual int func_0x150();
        virtual int func_0x158();
        virtual int func_0x160();
        virtual int func_0x168();
        virtual int func_0x170();

        u8 unk6[0x10];
        u64 uniqueHash;
        u8 unk7[0x1C];
        bool isRendered;
        u8 unk88[0x53];
        bool isVisible;
        bool isCulling;
        u8 unk8[0x17E];
    };
    static_assert(sizeof(FieldObject) == 0x398);
    struct PokemonModel : public FieldObject {
        static void* sVTable[];
        static void Constructor(PokemonModel* obj, u64 p1, u64 p2, u64 p3);
        // ...
        u8 unk9[0x5cc - 0x398];
        s32 species;
        s32 form;
        // ...
        u8 unk10[0x8a0 - 0x5cc - 4 - 4];
    };
    static_assert(offsetof(PokemonModel, species) == 0x5cc);
    static_assert(offsetof(PokemonModel, form) == 0x5cc + 4);
    static_assert(sizeof(PokemonModel) == 0x8a0);
    struct EncountObject : PokemonModel {
        static void* sVTable[];
        u32 unk11;
        s32 unk12;
        encounter::OverworldSpec overworldSpec;
        u8 unk13[0xd48 - 0x8a8 - 0x58];
        u64 spawnerHash;
        u8 unk14[0xd80-0xd50];
        // ...
        static void Constructor(EncountObject* obj, u64 p1, u64 p2, u64 p3);
    };
    static_assert(sizeof(EncountObject) == 0xd80);
    struct GimmickEncountSpawner : FieldObject {
        static void* sVTable[];
        bool Init(void* p1, void* p2, void* p3, void* p4, void* p5, void* p6);
        void Spawn();
        // ...
    };
    struct FieldSparkleItem : FieldObject {
        static void* sVTable[];
        u8 unk9[0x500-0x398];
        s32 itemThresholds[8];
        s32 itemCounts[8];
        // ...
    };
    static_assert(offsetof(FieldSparkleItem, itemThresholds) == 0x500);
    struct FishingPoint : FieldObject {
        static void* sVTable[];
        // ...
    };
}
}