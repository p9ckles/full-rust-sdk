#pragma once
#include <cstdint>
#include <cstring>

#include "utilities/driver.h"
#include "utilities/debug.h"
#include "globals.h"
#include "sdk/offset.h"

extern c_memory m;

namespace decryption {


    inline uint64_t get_handle(uint32_t handle) {
        uint64_t rdi_1 = ((uint64_t)(handle >> 3));
        uint64_t rcx_1 = ((uint64_t)((handle & 7) - 1));
        uint64_t array = m.read<uint64_t>((rcx_1 * 0x28) + globals->game_assembly + offsets::Il2cpp::base + 0x8) + (rdi_1 << 3);

        if (m.read<uint8_t>((rcx_1 * 0x28) + globals->game_assembly + offsets::Il2cpp::base + 0x14) > 1)
            return m.read<uint64_t>(array);
        else {
            uint32_t eax = m.read<uint32_t>(array);

            eax = ~eax;

            return eax;
        }
    }

    // Decryption functions - verified by rust-dumper 2026-05-02
    namespace BaseNetworkable {
        // a1 = EntityRealm object ptr, reads encrypted GCHandle from +0x18
        inline uintptr_t decrypt(std::uintptr_t a1)
        {
            std::uintptr_t raw = m.read<std::uintptr_t>(a1 + 0x18);
            std::uintptr_t rax = raw;
            std::uint32_t* rdx = (std::uint32_t*)&rax;
            std::uint32_t r8d = 0x2;
            std::uint32_t eax, ecx;

            do {
                ecx = *(std::uint32_t*)(rdx);
                eax = *(std::uint32_t*)(rdx);
                rdx = (std::uint32_t*)((std::uint8_t*)rdx + 0x4);
                eax = eax << 0x6;
                ecx = ecx >> 0x1a;
                ecx = ecx | eax;
                ecx = ecx ^ 0x341495B6;
                eax = ecx;
                ecx = ecx << 0x11;
                eax = eax >> 0xf;
                eax = eax | ecx;
                *((std::uint32_t*)rdx - 1) = eax;
                --r8d;
            } while (r8d);
            return get_handle(rax);
        }

        // a1 = entity list object ptr, reads encrypted GCHandle from +0x18
        inline uintptr_t decrypt_list(std::uintptr_t a1)
        {
            std::uintptr_t rax = m.read<std::uintptr_t>(a1 + 0x18);
            std::uint32_t* rdx = (std::uint32_t*)&rax;
            std::uint32_t r8d = 0x2;
            std::uint32_t eax, ecx;

            do {
                eax = *(std::uint32_t*)(rdx);
                rdx = (std::uint32_t*)((std::uint8_t*)rdx + 0x4);
                eax = eax + 0x26A5239B;
                ecx = eax;
                eax = eax << 0xd;
                ecx = ecx >> 0x13;
                ecx = ecx | eax;
                ecx = ecx + 0x1B3D055E;
                ecx = ecx ^ 0xA0387451;
                *((std::uint32_t*)rdx - 1) = ecx;
                --r8d;
            } while (r8d);
            return get_handle(rax);
        }
    }

    namespace BasePlayer {
        // a1 = encrypted cl_active_item value (not a pointer, raw encrypted value)
        inline std::uintptr_t decrypt_clActiveItem(uintptr_t a1)
        {
            uintptr_t val = a1;
            uintptr_t base_ptr = (uintptr_t)&val;
            uintptr_t rdx = base_ptr;

            for (uint32_t i = 2; i > 0; --i) {
                uint32_t ecx = *(uint32_t*)(rdx);
                uint32_t eax = *(uint32_t*)(rdx);
                rdx = (uintptr_t)((uint8_t*)rdx + 0x04);
                eax = (eax << 0x1D) | (ecx >> 0x03);
                ecx = eax;
                ecx += 0xB5616B18;
                eax = ecx;
                ecx = (ecx << 0x1A) | (eax >> 0x06);
                *((uint32_t*)rdx - 1) = ecx;
            }

            return val;
        }

        inline uint64_t player_inventory(uint64_t enc)
        {
            if (!enc) return 0;

            uint32_t* chunks = reinterpret_cast<uint32_t*>(&enc);

            for (int i = 0; i < 2; i++) {
                uint32_t eax = chunks[i];
                eax += 0x5DF63C22;
                uint32_t ecx = (eax << 0x0A) | (eax >> 0x16);
                ecx += 0x482557EE;
                ecx ^= 0x0DAFE798;
                chunks[i] = ecx;
            }

            return get_handle(static_cast<uint32_t>(enc));
        }

        inline std::uintptr_t player_eyes(uintptr_t a1)
        {
            uintptr_t val = m.read<uintptr_t>(a1 + 0x10);

            // no transformation at all
            return get_handle(*reinterpret_cast<uint32_t*>(val));
        }
    }

    inline uint32_t decrypt_value(uint32_t val) {
        val = (val >> 14) | (val << 18);
        val -= 0x4FDF3546;
        val ^= 0x731A6B75;
        val = (val << 14) | (val >> 18);
        return val;
    }

    inline uint32_t encrypt_value(uint32_t val) {
        val = (val >> 14) | (val << 18);
        val ^= 0xAAAED93A;
        val += 0x526855B7;
        val = (val << 14) | (val >> 18);
        return val;
    }

    inline uint32_t decrypt_fov(uint32_t val) {
        return decrypt_value(val);
    }

    // Add this if you use get_component_name
    inline uint64_t get_component_name(uint64_t game_object, const char* component_name) {
        uint64_t game_object_class = m.read<uint64_t>(game_object + 0x30);

        for (int index = 0; index < 20; index++)
        {
            uint64_t component = m.read<uint64_t>(game_object_class + (0x10 * index + 0x8));

            if (!component)
                continue;

            uint64_t fields = m.read<uint64_t>(component + 0x28);

            if (!fields)
                continue;

            uint64_t name_ptr = m.read<uint64_t>(fields);

            name_ptr = m.read<uint64_t>(name_ptr + 0x10);

            if (!name_ptr)
                continue;

            std::string buffer = m.read_ascii(name_ptr);

            if (strcmp(buffer.c_str(), component_name) == 0)
                return fields;
        }

        return -1;
    }

    // Backward-compatible legacy wrappers.
    inline uint64_t decrypt_base_networkable(uint64_t a1) {
        return BaseNetworkable::decrypt(a1);
    }

    inline uint64_t decrypt_base_networkable_list(uint64_t a1) {
        return BaseNetworkable::decrypt_list(a1);
    }

    inline uint64_t decrypt_cl_active_weapon(uint64_t a1) {
        return BasePlayer::decrypt_clActiveItem(a1);
    }

} // namespace decryption