#include "Vector4.h"

const Vector4 Vector4::zero(0.0f, 0.0f, 0.0f, 0.0f);
const Vector4 Vector4::one(1.0f, 1.0f, 1.0f, 1.0f);

Vector4::Vector4():x(0.0f),y(0.0f),z(0.0f),w(0.0f)
{
}

Vector4::Vector4(float x, float y, float z, float w) : x(x), y(y), z(z), w(w)
{
}

Vector4::~Vector4()
{
}

Vector4::Vector4(const Vector3 & vec3, float w) : x(vec3.x), y(vec3.y), z(vec3.z), w(w)
{
}

float Vector4::dot(const Vector4 & v) const
{
	return (x*v.x + y*v.y+z*v.z+w*v.w);
}

float Vector4::dot(const Vector4 & v, const Vector4 & v2)
{
	return (v2.x*v.x+v2.y*v.y+v2.z*v.z+v2.w*v.w);
}

Vector4 Vector4::operator+() const
{
	return *this;
}

Vector4 Vector4::operator-() const
{
	return Vector4(-x,-y,-z,-w);
}

Vector4 & Vector4::operator+=(const Vector4 & v)
{
	x += v.x;
	y += v.y;
	z += v.z;
	w += v.w;
	return *this;
}

Vector4 & Vector4::operator-=(const Vector4 & v)
{
	x -= v.x;
	y -= v.y;
	z -= v.z;
	w -= v.w;
	return *this;
}

Vector4 & Vector4::operator*=(float s)
{
	x *= s;
	y *= s;
	z *= s;
	w *= s;

	return *this;
}

Vector4 & Vector4::operator=(const Vector4 & vec)
{
	x = vec.x;
	y = vec.y;
	z = vec.z;
	w = vec.w;
	return *this;
}

const Vector4 operator+(const Vector4 & v1, const Vector4 & v2)
{
	return Vector4(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z,v2.w+v1.w);
}

const Vector4 operator-(const Vector4 & v1, const Vector4 & v2)
{
	return Vector4(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z,v2.w-v1.w);
}

const Vector4 operator*(const Vector4 & v, float s)
{
	return Vector4(v.x*s,v.y*s,v.z*s,v.w*s);
}

const Vector4 operator*(float s, const Vector4 & v)
{
	return Vector4(v.x*s, v.y*s, v.z*s, v.w*s);
}
