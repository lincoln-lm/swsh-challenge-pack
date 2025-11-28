#pragma once

#include "hk/types.h"
#include "hk/util/Random.h"
#include "hk/util/hash.h"
#include "orion/battle/Move.hpp"
#include "save/SaveFile.hpp"
#include "util/Data.hpp"
#include "util/Personal.hpp"
#include <bit>
#include <random>
#include <span>

class MersenneTwister : public std::mt19937_64 {
    public:
        using std::mt19937_64::mt19937_64;
        u64 RandMax(u64 maximum) {
            if (maximum == 0 || maximum == 1) {
                return 0;
            }
            u64 mask = std::bit_ceil(maximum) - 1;
            u64 result;
            do {
                result = this->operator()() & mask;
            } while (result >= maximum);
            return result;
        }
        u64 RandRange(u64 minimum, u64 maximum) {
            u64 range = maximum - minimum + 1;
            return minimum + this->RandMax(range);
        }
        template<typename T, std::size_t Extent>
        T RandElement(const std::span<const T, Extent> input) {
            return input[this->RandMax(input.size())];
        }
        f64 RandDouble() {
            return this->operator()() / static_cast<f64>(std::numeric_limits<u64>::max());
        }
        bool RandChance(u64 denominator) {
            return this->RandMax(denominator) == 0;
        }
        std::tuple<u32, u16> RandSpeciesAndForm() {
            u32 species;
            u16 form;
            do {
                species = this->RandRange(1, 899);
                u32 form_count = util::getPersonalInfoField(species, 0, orion::personal::InfoField::FORM_COUNT);
                form = this->RandMax(form_count);
            } while (!util::isInGame(species, form));
            return {species, form};
        }
        s16 RandValidMoveId() {
            s16 move_id;
            do {
                move_id = static_cast<s16>(this->RandRange(1, 820));
            } while (!orion::battle::Move{0, (u32)move_id}.isUsable());
            return move_id;
        }
        void RandMoves(std::span<s16, 4> moves) {
            for (size_t i = 0; i < moves.size(); ++i) {
                do {
                    moves[i] = this->RandValidMoveId();
                } while (std::find(moves.begin(), moves.begin() + i, moves[i]) != moves.begin() + i);
            }
        }
        std::array<s16, 4> RandMoves() {
            std::array<s16, 4> moves;
            this->RandMoves(moves);
            return moves;
        }
        u16 RandHeldItem() {
            return this->RandElement(std::span(util::data::VALID_HELD_ITEMS));
        }
        u16 RandTM() {
            return this->RandElement(std::span(util::data::VALID_TMS));
        }
        u16 RandAbility() {
            return this->RandRange(1, 268);
        }
        u16 RandBall() {
            return this->RandRange(1, 27);
        }
};

namespace RngManager {
    template<typename T>
    inline MersenneTwister NewRandomGenerator(std::span<const T> input) {
        const size size = input.size_bytes();
        const u8* input_bytes = pun<u8*>(input.data());
        // TODO: this is probably dumb
        auto rng = std::mt19937_64 { save::gSaveFile.rngSeed };
        u64 high = rng() & 0xFFFFFFFF;
        u64 low = rng() & 0xFFFFFFFF;
        high = hk::util::hashMurmur(input_bytes, (u32)size, high);
        low = hk::util::hashMurmur(input_bytes, (u32)size, low);
        return MersenneTwister(low | (high << 32));
    }
    inline MersenneTwister NewRandomGenerator(const std::string input) {
        return NewRandomGenerator(std::span(input));
    }
    template<typename T>
    requires std::is_integral_v<T>
    inline MersenneTwister NewRandomGenerator(const T input) {
        return NewRandomGenerator(std::span<const char>(reinterpret_cast<const char*>(&input), sizeof(T)));
    }
    inline MersenneTwister NewRandomGenerator() {
        return NewRandomGenerator(hk::util::getRandomU64());
    }
};