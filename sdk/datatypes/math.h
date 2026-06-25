#pragma once

#include "sdk/datatypes/vector.h"
#include "sdk/datatypes/quaternion.h"

#define rad_to_degrees(x) ((float)(x) * (float)(180.f / ((float)(3.14159265358979323846f))))


namespace math {
    inline double rad(double degree) {
        double pi = 3.14159265359;
        return (degree * (pi / 180));
    }

    inline vector2 calculate_angle(const vector3& first, const vector3& second) {
        vector3 direction = first - second;

        float length = direction.length();

        return vector2{ rad_to_degrees(asin(direction.y / length)), rad_to_degrees(-atan2(direction.x, -direction.z)) };
    }

    inline void normalize(float& yaw, float& pitch) {
        if (pitch < -89) pitch = -89;
        else if (pitch > 89) pitch = 89;
        if (yaw < -360) yaw += 360;
        else if (yaw > 360) yaw -= 360;
    }

    inline void half_normalize(float& yaw, float& pitch) {
        if (pitch > 89.0f) pitch = 89.0f;
        if (pitch < -89.0f) pitch = -89.0f;
        while (yaw > 180.0f)  yaw -= 360.0f;
        while (yaw < -180.0f) yaw += 360.0f;
    }

    inline vector3 normalize(const vector3& v) {
        float length = sqrtf(v.x * v.x + v.y * v.y + v.z * v.z);

        if (length == 0.0f)
            return { 0.f, 0.f, 0.f };

        return { v.x / length, v.y / length, v.z / length };
    }

    inline vector4 eurler(vector3 position) {
        double heading = rad(position.x);
        double attitude = rad(position.y);
        double bank = rad(position.z);

        double c1 = cos(heading / 2);
        double s1 = sin(heading / 2);
        double c2 = cos(attitude / 2);
        double s2 = sin(attitude / 2);
        double c3 = cos(bank / 2);
        double s3 = sin(bank / 2);

        double c1c2 = c1 * c2;
        double s1s2 = s1 * s2;

        vector4 first;

        first.w = c1c2 * c3 - s1s2 * s3;
        first.x = c1c2 * s3 + s1s2 * c3;
        first.y = s1 * c2 * c3 + c1 * s2 * s3;
        first.z = c1 * s2 * c3 - s1 * c2 * s3;

        vector4 second;

        second = { first.y, first.z, (first.x * -1), first.w };

        return second;
    }
}