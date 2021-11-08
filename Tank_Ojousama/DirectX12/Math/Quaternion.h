#pragma once
class Vector3;

class Quaternion
{
public:
	float x;
	float y;
	float z;
	float w;

	Quaternion();
	explicit Quaternion(float x, float y, float z, float w);
	explicit Quaternion(const Vector3& axis, float angle);
	~Quaternion();
	void set(float inX, float inY, float inZ, float inW);
	friend Quaternion operator *(const Quaternion& q1, const Quaternion& q2);
	Quaternion& operator *= (const Quaternion& q2);
	void conjugate();
	float lengthSq() const;
	float length() const;
	void normalize();
	// Normalize the provided quaternion
	static Quaternion normalize(const Quaternion& q);
	// Linear interpolation
	static Quaternion lerp(const Quaternion& a, const Quaternion& b, float f);
	static float dot(const Quaternion& a, const Quaternion& b);
	// Spherical Linear Interpolation
	static Quaternion slerp(const Quaternion& a, const Quaternion& b, float f);
	// Concatenate
	// Rotate by q FOLLOWED BY p
	//ä|ÇØéZ  âÒì]ÇÃÉtÉHÉçÅ[ä÷êî
	static Quaternion concatenate(const Quaternion& q, const Quaternion& p);

	static const Quaternion identity;

};