#include "Vector3.h"
#include "Math.h"
#include<cmath>
#include<iostream>

const Vector3 Vector3::zero(0.0f, 0.0f, 0.0f);
const Vector3 Vector3::one(1.0f, 1.0f, 1.0f);

const Vector3 Vector3::right(1.0f, 0.0f, 0.0f);
const Vector3 Vector3::up(0.0f, 1.0f, 0.0f);
const Vector3 Vector3::forward(0.0f, 0.0f, 1.0f);
const Vector3 Vector3::left(-1.0f, 0.0f, 0.0f);
const Vector3 Vector3::down(0.0f, -1.0f, 0.0f);
const Vector3 Vector3::back(0.0f, 0.0f, -1.0f);
const Vector3 Vector3::Infinity(Math::infinity, Math::infinity, Math::infinity);

Vector3::Vector3() :x(0), y(0), z(0)
{
}

Vector3::Vector3(float x, float y, float z):x(x),y(y),z(z)
{
}

Vector3::Vector3(const Vector2 & vec2, float z):
	x(vec2.x),
	y(vec2.y),
	z(z)
{
}

Vector3::Vector3(const Vector3 & vec3):
	x(vec3.x),
	y(vec3.y),
	z(vec3.z)
{
}

Vector3::~Vector3()
{
}

float Vector3::Length() const
{
	return (float)std::sqrtf(x*x+y*y+z*z);
}

Vector3 Vector3::Lerp(const Vector3 & v1, const Vector3 & v2, float t)
{
	return (1 - t)*v1 + t * v2;
}


void Vector3::normalize()
{
	float Len = Length();
	if (Len == 0)return;
	x /= Len;
	y/= Len;
	z /= Len;
}

Vector3 & Vector3::normal()
{
	float len;

	if ((len = Length()) != 0)
		*this /= len;

	return *this;
}

Vector3 Vector3::normalize(const Vector3 & vec)
{
	Vector3 temp = vec;
	temp.normalize();
	return temp;
}

float Vector3::dot(const Vector3 & v) const
{
	return (x*v.x + y*v.y+z*v.z);
}

 float Vector3::dot(const Vector3 & v, const Vector3 & v2)
{
	return (v2.x*v.x + v2.y * v.y + v2.z * v.z);
}

Vector3 Vector3::cross(const Vector3 & v) const
{
	return Vector3(
	y*v.z-z*v.y,
		z*v.x-x*v.z,
		x*v.y-y*v.x);
}

 Vector3 Vector3::cross(const Vector3 & v, const Vector3 & v2)
{
	return Vector3(
		v.y*v2.z - v.z*v2.y,
		v.z*v2.x - v.x*v2.z,
		v.x*v2.y - v.y*v2.x);
}

Vector3 Vector3::operator+() const
{
	return *this;
}

Vector3 Vector3::operator-() const
{
	return Vector3(-x,-y,-z);
}

Vector3 & Vector3::operator=(const Vector3 & vec)
{
	x = vec.x;
	y = vec.y;
	z = vec.z;
	return *this;
}

Vector3 & Vector3::operator+=(const Vector3 & v)
{
	x += v.x;
	y += v.y;
	z += v.z;
	return *this;
}

Vector3 & Vector3::operator-=(const Vector3 & v)
{
	x -= v.x;
	y -= v.y;
	z -= v.z;
	return *this;
}

Vector3 & Vector3::operator*=(const Vector3 & v)
{
	x *= v.x;
	y *= v.y;
	z *= v.z;
	return *this;
}

Vector3 & Vector3::operator*=(float s)
{
	x *= s;
	y *= s;
	z *= s;
	return *this;
}

Vector3 & Vector3::operator/=(float s)
{
	if (s == 0)
	{
		x = x;
		y = y;
		z = z;
	}
	else
	{
		x /= s;
		y /= s;
		z /= s;
	}

	return *this;
}

const Vector3 operator+(const Vector3 & v1, const Vector3 & v2)
{
	return Vector3(v1.x + v2.x, v1.y + v2.y,v1.z + v2.z);
}

const Vector3 operator-(const Vector3 & v1, const Vector3 & v2)
{
	return Vector3(v1.x - v2.x, v1.y - v2.y,v1.z - v2.z);
}

const Vector3 operator*(const Vector3 & left, const Vector3 & right)
{
	return Vector3(left.x * right.x, left.y * right.y, left.z * right.z);
}


const Vector3 operator*(const Vector3 & v, float s)
{
	return Vector3(v.x *s, v.y*s , v.z*s);
}

const Vector3 operator*(float s, const Vector3 & v)
{
	return Vector3(v.x *s, v.y*s ,v.z*s);
}

const Vector3 operator/(const Vector3 & v, float s)
{
	return Vector3(v.x / s, v.y / s,v.z/s);
}
