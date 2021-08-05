#pragma once
#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Matrix4.h"
#include "Easing.h"

namespace Math
{
	constexpr float PI = 3.1415926535f;
	constexpr float TwoPI = PI * 2.f;
	constexpr float PIOver2 = PI / 2.f;
	constexpr float degree = PI / 180.f;
	constexpr float radians = 180.f / PI;
	constexpr float epsilon = std::numeric_limits<float>::epsilon();
	constexpr float infinity = std::numeric_limits<float>::infinity();

	inline float toRadians(float degrees) {
		return degrees * PI / 180.0f;
	}

	inline float toDegrees(float radians) {
		return radians * 180.0f / PI;
	}

	inline bool nearZero(float val, float epsilon = 0.001f) {
		return (fabs(val) <= epsilon);
	}

	inline bool equal(float a, float b) {
		return (fabsf(a - b) <= epsilon);
	}

	template <typename T>
	T Max(const T& a, const T& b) {
		return (a < b ? b : a);
	}

	template <typename T>
	T Min(const T& a, const T& b) {
		return (a < b ? a : b);
	}

	template <typename T>
	T clamp(const T& value, const T& min, const T& max) {
		return Min(max, Max(min, value));
	}

	template <typename T>
	T pow(T value, int pow) {//ó›èÊ
		for (int i = 0; i < pow; i++) {
			value *= value;
		}
		return value;
	}

	inline float abs(float value) {//ê‚ëŒíl
		return fabs(value);
	}

	inline float cos(float Degree) {
		return cosf(Degree *  degree);
	}

	inline float sin(float Degree) {
		return sinf(Degree *  degree);
	}

	inline float tan(float Degree) {
		return tanf(Degree *  degree);
	}

	inline float asin(float value) {
		return asinf(value);
	}

	inline float acos(float value) {
		return acosf(value);
	}

	inline float atan2(float y, float x) {
		return atan2f(y, x);
	}

	inline float cot(float Degree) {
		return 1.0f / tan(Degree);
	}

	inline float lerp(float a, float b, float f) {
		return a + f * (b - a);
	}

	inline float sqrt(float value) {
		return sqrtf(value);
	}

	inline float fmod(float numer, float denom) {//Ç†Ç‹ÇË
		return fmod(numer, denom);
	}

	//ç≈è¨ílà»è„ç≈ëÂílà»â∫Çï‘Ç∑
	inline float clampOnRange(float x, float min, float max)
	{
		if (x < min) {
			return min;
		}
		else if (x > max) {
			return max;
		}
		else {
			return x;
		}
	}
}