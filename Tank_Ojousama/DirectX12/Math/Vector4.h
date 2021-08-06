#pragma once
#include "Vector3.h"
class Vector4
{
public:
	float x;
	float y;
	float z;
	float w;
	Vector4();
	Vector4(float x, float y,float z,float w);
	~Vector4();
	Vector4(const Vector3&vec3, float w);
	float dot(const Vector4& v)const;//ì‡êœ
	static float dot(const Vector4& v, const Vector4& v2);//ì‡êœ
	Vector4 operator +()const;
	Vector4 operator -()const;

	Vector4& operator += (const Vector4& v);
	Vector4& operator -= (const Vector4& v);
	Vector4& operator *=(float s);
	Vector4& operator =(const Vector4& vec);

	static const Vector4 zero;
	static const Vector4 one;
private:

};
const Vector4 operator + (const Vector4& v1, const Vector4& v2);
const Vector4 operator - (const Vector4& v1, const Vector4& v2);
const Vector4 operator * (const Vector4& v, float s);
const Vector4 operator * (float s, const Vector4& v);
