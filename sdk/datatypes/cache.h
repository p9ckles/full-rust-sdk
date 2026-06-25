#pragma once

#include "include.h"
#include "sdk/interfaces/bones.h"

namespace bone_cache {
    inline static std::vector<uint64_t> cached_players;

    inline static std::unordered_map<uint64_t, std::unordered_map<int, vector3>> cached_bones;
    inline static std::unordered_map<uint64_t, std::unordered_map<int, vector3>> stable_bones;

    inline static uint64_t last_players_cache_time = 0;
    inline static uint64_t last_bones_cache_time = 0;

    inline static const uint64_t players_cache_interval = 5000;
    inline static const uint64_t bones_cache_interval = 50;

    inline bool should_update_players() {
        uint64_t current_time = GetTickCount64();

        return (current_time - last_players_cache_time) >= players_cache_interval;
    }

    inline void update_players_time() {
        last_players_cache_time = GetTickCount64();
    }

    inline vector3 bone(uint64_t object, int bone_id) {
        static uint64_t last_update_time = 0;

        uint64_t current_time = GetTickCount64();

        if ((current_time - last_bones_cache_time) >= bones_cache_interval) {
            cached_bones.clear();

            last_bones_cache_time = current_time;
        }

        auto it = cached_bones.find(object);

        if (it == cached_bones.end()) {
            vector3 bone_pos = bones::bone(object, bone_id);

            cached_bones[object][bone_id] = bone_pos;
            stable_bones[object][bone_id] = bone_pos;

            return bone_pos;
        }

        auto bone_it = it->second.find(bone_id);

        if (bone_it == it->second.end()) {
            vector3 bone_pos = bones::bone(object, bone_id);

            cached_bones[object][bone_id] = bone_pos;
            stable_bones[object][bone_id] = bone_pos;

            return bone_pos;
        }

        return bone_it->second;
    }
}