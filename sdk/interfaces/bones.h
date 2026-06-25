#pragma once

#include "include.h"

#include "sdk/datatypes/array.h"

namespace bones {
    inline vector3 bone_position(uint64_t transform) {
        __m128 result{};

        const __m128 quat_mul_x = { -2.000f,  2.000f, -2.000f, 0.000f };
        const __m128 quat_mul_y = { 2.000f, -2.000f, -2.000f, 0.000f };
        const __m128 quat_mul_z = { -2.000f, -2.000f,  2.000f, 0.000f };

        transform_access_read access = m.read<transform_access_read>(transform + 0x38);

        transform_data data = m.read<transform_data>(access.transform_data + 0x18);

        unsigned int bone_index = m.read<unsigned int>(transform + 0x40);

        if (data.transform_array && data.transform_indices) {
            result = m.read<__m128>(data.transform_array + 0x30 * bone_index);

            int parent_index = m.read<int>(data.transform_indices + 0x4 * bone_index);

            while (parent_index >= 0) {
                types transform_matrix = m.read<types>(data.transform_array + 0x30 * parent_index);

                __m128i quat_shuffled = *(__m128i*)(&transform_matrix.second);

                __m128 quat_x = _mm_castsi128_ps(_mm_shuffle_epi32(quat_shuffled, 0x00));
                __m128 quat_y = _mm_castsi128_ps(_mm_shuffle_epi32(quat_shuffled, 0x55));
                __m128 quat_z = _mm_castsi128_ps(_mm_shuffle_epi32(quat_shuffled, 0x8E));
                __m128 quat_w = _mm_castsi128_ps(_mm_shuffle_epi32(quat_shuffled, 0xDB));
                __m128 quat_yy = _mm_castsi128_ps(_mm_shuffle_epi32(quat_shuffled, 0xAA));
                __m128 quat_zz = _mm_castsi128_ps(_mm_shuffle_epi32(quat_shuffled, 0x71));

                __m128 transformed = _mm_mul_ps(*(__m128*)(&transform_matrix.third), result);

                __m128i transformed_i = _mm_castps_si128(transformed);

                __m128 term_x = _mm_mul_ps(_mm_sub_ps(_mm_mul_ps(_mm_mul_ps(quat_x, quat_mul_y), quat_z), _mm_mul_ps(_mm_mul_ps(quat_y, quat_mul_z), quat_w)), _mm_castsi128_ps(_mm_shuffle_epi32(transformed_i, 0xAA)));
                __m128 term_y = _mm_mul_ps(_mm_sub_ps(_mm_mul_ps(_mm_mul_ps(quat_yy, quat_mul_z), quat_w), _mm_mul_ps(_mm_mul_ps(quat_x, quat_mul_x), quat_zz)), _mm_castsi128_ps(_mm_shuffle_epi32(transformed_i, 0x55)));
                __m128 term_z = _mm_add_ps(_mm_mul_ps(_mm_sub_ps(_mm_mul_ps(_mm_mul_ps(quat_y, quat_mul_x), quat_zz), _mm_mul_ps(_mm_mul_ps(quat_yy, quat_mul_y), quat_z)), _mm_castsi128_ps(_mm_shuffle_epi32(transformed_i, 0x00))), transformed);

                result = _mm_add_ps(_mm_add_ps(term_x, term_y), _mm_add_ps(term_z, *(__m128*)(&transform_matrix.first)));

                parent_index = m.read<int>(data.transform_indices + 0x4 * parent_index);
            }
        }

        return vector3(result.m128_f32[0], result.m128_f32[1], result.m128_f32[2]);
    }

    inline vector3 bone(uint64_t player, int bone_index) {
        uint64_t model = m.read<uint64_t>(player + offsets::BaseEntity::model);

        if (!model)
            return vector3(0, 0, 0);

        uint64_t bone_transforms = m.read<uint64_t>(model + offsets::Model::boneTransforms);

        if (!bone_transforms)
            return vector3(0, 0, 0);

        uint64_t bone = m.read<uint64_t>(bone_transforms + (0x28 + (bone_index * 0x8)));

        if (!bone)
            return vector3(0, 0, 0);

        uint64_t transform = m.read<uint64_t>(bone + 0x10);

        if (!transform)
            return vector3(0, 0, 0);

        return bone_position(transform);
    }
}