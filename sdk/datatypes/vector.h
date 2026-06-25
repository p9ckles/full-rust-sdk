#pragma once

#include "include.h"

struct vector3
{
public:
	float x, y, z;

	vector3()
	{
		x = y = z = 0.0f;
	}

	vector3(float X, float Y, float Z)
	{
		x = X; y = Y; z = Z;
	}

	float operator[](int i) const
	{
		return ((float*)this)[i];
	}

	vector3& operator-=(float v)
	{
		x -= v; y -= v; z -= v; return *this;
	}

	vector3 operator*=(float v)
	{
		x *= v; y *= v; z *= v;
		return *this;
	}

	vector3 operator*(float v) const
	{
		return vector3(x * v, y * v, z * v);
	}

	vector3 operator/(float v) const
	{
		return vector3(x / v, y / v, z / v);
	}

	vector3& operator+=(const vector3& v)
	{
		x += v.x; y += v.y; z += v.z; return *this;
	}

	vector3 operator-(const vector3& v) const
	{
		return vector3(x - v.x, y - v.y, z - v.z);
	}

	vector3 operator+(const vector3& v) const
	{
		return vector3(x + v.x, y + v.y, z + v.z);
	}

	vector3 operator*(const vector3& v) const
	{
		return vector3(x * v.x, y * v.y, z * v.z);
	}

	float distance(vector3 v)
	{
		return float(sqrtf(powf(v.x - x, 2.0) + powf(v.y - y, 2.0) + powf(v.z - z, 2.0)));
	}

	float length() const
	{
		return sqrtf(x * x + y * y + z * z);
	}

	float length_squared() const
	{
		return x * x + y * y + z * z;
	}

	vector3 cross(const vector3& v) const
	{
		return vector3(
			y * v.z - z * v.y,
			z * v.x - x * v.z,
			x * v.y - y * v.x
		);
	}

	vector3 normalize() const
	{
		float len = length();

		if (len == 0.0f) 
			return vector3(0.0f, 0.0f, 0.0f);

		return vector3(x / len, y / len, z / len);
	}

	float dot(vector3 input) const
	{
		return (x * input.x) + (y * input.y) + (z * input.z);
	}

	bool is_empty() const
	{
		return (x == 0.0f && y == 0.0f && z == 0.0f);
	}

	bool operator==(const vector3& v) const
	{
		if (this->x == v.x && this->y == v.y && this->z == v.z)
			return true;
		else
			return false;
	}

	bool operator!=(const vector3& v) const
	{
		if (this->x == v.x && this->y == v.y && this->z == v.z)
			return false;
		else
			return true;
	}

	float magnitude() const
	{
		return sqrtf(x * x + y * y + z * z);
	}

	vector3 normalized() const
	{
		float mag = magnitude();
		if (mag == 0.0f)
			return vector3(0, 0, 0);
		return vector3(x / mag, y / mag, z / mag);
	}
};

struct vector2
{
public:
	float x, y;

	vector2()
	{
		x = y = 0.0f;
	}

	vector2 operator/(float v) const {
		return vector2(x / v, y / v);
	}
	vector2(float X, float Y) {
		x = X; y = Y;
	}

	void operator -= (vector2 other)
	{
		x -= other.x;
		y -= other.y;
	}

	vector2 operator-(const vector2& v) const {
		return vector2(x - v.x, y - v.y);
	}

	vector2 operator+(const vector2& v) const {
		return vector2(x + v.x, y + v.y);
	}

	vector2& operator+=(const vector2& v) {
		x += v.x; y += v.y; return *this;
	}

	float distance(vector2 b)
	{
		return sqrt(pow(b.x - x, 2) + pow(b.y - y, 2));
	}

	bool zero() const
	{
		return (x > -0.1f && x < 0.1f && y > -0.1f && y < 0.1f);
	}

	vector2 normalize()
	{

		if (x < -89)
			x = -89;


		else if (x > 89)
			x = 89;


		if (y < -360)
			y += 360;


		else if (y > 360)
			y -= 360;

		return { x,y };
	}
};

struct vector4
{
public:
	float x, y, z, w;

	vector4()
	{
		x = y = z = w = 0.0f;
	}

	vector4(float X, float Y, float Z, float W)
	{
		x = X; y = Y; z = Z; w = W;
	}

	float operator[](int i) const
	{
		return ((float*)this)[i];
	}

	vector4& operator-=(float v)
	{
		x -= v; y -= v; z -= v; w -= v; return *this;
	}

	vector4 operator*=(float v)
	{
		x *= v; y *= v; z *= v; w *= v;
		return *this;
	}

	vector4 operator*(float v) const
	{
		return vector4(x * v, y * v, z * v, w * v);
	}

	vector4 operator/(float v) const
	{
		return vector4(x / v, y / v, z / v, w / v);
	}

	vector4& operator+=(const vector4& v)
	{
		x += v.x; y += v.y; z += v.z; w += v.w; return *this;
	}

	vector4 operator-(const vector4& v) const
	{
		return vector4(x - v.x, y - v.y, z - v.z, w - v.w);
	}

	vector4 operator+(const vector4& v) const
	{
		return vector4(x + v.x, y + v.y, z + v.z, w + v.w);
	}

	vector4 operator*(const vector4& v) const
	{
		return vector4(
			w * v.x + x * v.w + y * v.z - z * v.y,
			w * v.y - x * v.z + y * v.w + z * v.x,
			w * v.z + x * v.y - y * v.x + z * v.w,
			w * v.w - x * v.x - y * v.y - z * v.z
		);
	}

	float distance(vector4 v)
	{
		return float(sqrtf(powf(v.x - x, 2.0) + powf(v.y - y, 2.0) + powf(v.z - z, 2.0) + powf(v.w - w, 2.0)));
	}

	float length()
	{
		return sqrtf(x * x + y * y + z * z + w * w);
	}

	float dot(vector4 input) const
	{
		return (x * input.x) + (y * input.y) + (z * input.z) + (w * input.w);
	}

	vector4 conjugate() const
	{
		return vector4(-x, -y, -z, w);
	}

	vector3 rotate(const vector3& v) const
	{
		vector4 qv(v.x, v.y, v.z, 0.0f);
		vector4 res = (*this) * qv * this->conjugate();

		return vector3(res.x, res.y, res.z);
	}

	vector3 rotate_inv(const vector3& v) const
	{
		vector4 q_conj = this->conjugate();
		vector4 qv(v.x, v.y, v.z, 0.0f);
		vector4 res = q_conj * qv * (*this);

		return vector3(res.x, res.y, res.z);
	}

	vector3 multiply(const vector3& point) {
		float num = x * 2.00f;
		float num2 = y * 2.00f;
		float num3 = z * 2.00f;
		float num4 = x * num;
		float num5 = y * num2;
		float num6 = z * num3;
		float num7 = x * num2;
		float num8 = x * num3;
		float num9 = y * num3;
		float num10 = w * num;
		float num11 = w * num2;
		float num12 = w * num3;

		vector3 result;

		result.x = (1.00f - (num5 + num6)) * point.x + (num7 - num12) * point.y + (num8 + num11) * point.z;
		result.y = (num7 + num12) * point.x + (1.00f - (num4 + num6)) * point.y + (num9 - num10) * point.z;
		result.z = (num8 - num11) * point.x + (num9 + num10) * point.y + (1.00f - (num4 + num5)) * point.z;

		return result;
	}

	bool operator==(const vector4& v) const
	{
		return (x == v.x && y == v.y && z == v.z && w == v.w);
	}

	bool operator!=(const vector4& v) const
	{
		return !(*this == v);
	}
};