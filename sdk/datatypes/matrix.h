#pragma once

struct matrix_4x4
{
    float _11, _12, _13, _14;
    float _21, _22, _23, _24;
    float _31, _32, _33, _34;
    float _41, _42, _43, _44;

    matrix_4x4 transpose() const
    {
        matrix_4x4 t{};

        t._11 = _11; t._12 = _21; t._13 = _31; t._14 = _41;
        t._21 = _12; t._22 = _22; t._23 = _32; t._24 = _42;
        t._31 = _13; t._32 = _23; t._33 = _33; t._34 = _43;
        t._41 = _14; t._42 = _24; t._43 = _34; t._44 = _44;

        return t;
    }
};

struct matrix_3x3
{
    float _11, _12, _13;
    float _21, _22, _23;
    float _31, _32, _33;
};

struct matrix_3x4
{
    float _11, _12, _13, _14;
    float _21, _22, _23, _24;
    float _31, _32, _33, _34;
};

inline vector3 operator*(const matrix_3x3& m, const vector3& v)
{
    return {
        m._11 * v.x + m._12 * v.y + m._13 * v.z,
        m._21 * v.x + m._22 * v.y + m._23 * v.z,
        m._31 * v.x + m._32 * v.y + m._33 * v.z
    };
}