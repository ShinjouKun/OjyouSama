#include "Quaternion.h"
#include "Math.h"

Quaternion::Quaternion()
{
	*this = Quaternion::identity;
}

Quaternion::Quaternion(float x, float y, float z, float w)
{
	set(x, y, z, w);
}

Quaternion::Quaternion(const Vector3 & axis, float angle)
{
	float scalar = Math::sin(angle / 2.0f);
	x = axis.x * scalar;
	y = axis.y * scalar;
	z = axis.z * scalar;
	w = Math::cos(angle / 2.0f);
}

Quaternion::~Quaternion() = default;

void Quaternion::set(float inX, float inY, float inZ, float inW)
{
	x = inX;
	y = inY;
	z = inZ;
	w = inW;
}

Quaternion operator*(const Quaternion & q1, const Quaternion & q2)
{
	Quaternion result = Quaternion(
		q1.x * q2.w + q1.y*q2.z - q1.z*q2.y + q1.w*q2.x,
		-q1.x*q2.z + q1.y*q2.w + q1.z*q2.x + q1.w*q2.y,
		q1.x * q2.y - q1.y*q2.x + q1.z*q2.w + q1.w*q2.z,
		-q1.x*q2.x - q1.y*q2.y - q1.z*q2.z + q1.w*q2.w
	);

	return result;
}

Quaternion & Quaternion::operator*=(const Quaternion & q2)
{
	Quaternion q1 = Quaternion(x, y, z, w);
	x = q1.x * q2.w + q1.y*q2.z - q1.z*q2.y + q1.w*q2.x;
	y = -q1.x*q2.z + q1.y*q2.w + q1.z*q2.x + q1.w*q2.y;
	z = q1.x * q2.y - q1.y*q2.x + q1.z*q2.w + q1.w*q2.z;
	w = -q1.x*q2.x - q1.y*q2.y - q1.z*q2.z + q1.w*q2.w;

	return *this;
}

void Quaternion::conjugate()
{
	x *= -1.0f;
	y *= -1.0f;
	z *= -1.0f;
}

float Quaternion::lengthSq() const
{
	return (x * x + y * y + z * z + w * w);
}

float Quaternion::length() const
{
	return Math::sqrt(lengthSq());
}

void Quaternion::normalize()
{
	float len = length();
	if (len == 0) {
		return;
	}
	x /= len;
	y /= len;
	z /= len;
	w /= len;
}

Quaternion Quaternion::normalize(const Quaternion & q)
{
	Quaternion retVal = q;
	retVal.normalize();
	return retVal;
}

Quaternion Quaternion::lerp(const Quaternion & a, const Quaternion & b, float f)
{
	Quaternion retVal;
	retVal.x = Math::lerp(a.x, b.x, f);
	retVal.y = Math::lerp(a.y, b.y, f);
	retVal.z = Math::lerp(a.z, b.z, f);
	retVal.w = Math::lerp(a.w, b.w, f);
	retVal.normalize();
	return retVal;
}

float Quaternion::dot(const Quaternion & a, const Quaternion & b)
{
	return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
}

Quaternion Quaternion::slerp(const Quaternion & a, const Quaternion & b, float f)
{
	float rawCosm = Quaternion::dot(a, b);

	float cosom = -rawCosm;
	if (rawCosm >= 0.0f) {
		cosom = rawCosm;
	}

	float scale0, scale1;

	if (cosom < 0.9999f) {
		const float omega = Math::acos(cosom);
		const float invSin = 1.f / Math::sin(omega);
		scale0 = Math::sin((1.f - f) * omega) * invSin;
		scale1 = Math::sin(f * omega) * invSin;
	}
	else {
		// Use linear interpolation if the quaternions
		// are collinear
		scale0 = 1.0f - f;
		scale1 = f;
	}

	if (rawCosm < 0.0f) {
		scale1 = -scale1;
	}

	Quaternion retVal;
	retVal.x = scale0 * a.x + scale1 * b.x;
	retVal.y = scale0 * a.y + scale1 * b.y;
	retVal.z = scale0 * a.z + scale1 * b.z;
	retVal.w = scale0 * a.w + scale1 * b.w;
	retVal.normalize();
	return retVal;
}

Quaternion Quaternion::concatenate(const Quaternion & q, const Quaternion & p)
{
	Quaternion retVal;

	// Vector component is:
	// ps * qv + qs * pv + pv x qv
	Vector3 qv(q.x, q.y, q.z);//qのベクトル
	Vector3 pv(p.x, p.y, p.z);//pのベクトル
	Vector3 newVec = p.w * qv + q.w * pv + Vector3::cross(pv, qv);
	retVal.x = newVec.x;
	retVal.y = newVec.y;
	retVal.z = newVec.z;

	// Scalar component is:
	// ps * qs - pv . qv
	retVal.w = p.w * q.w - Vector3::dot(pv, qv);

	return retVal;
}

const Quaternion  Quaternion::identity(0.0f, 0.0f, 0.0f, 1.0f);


