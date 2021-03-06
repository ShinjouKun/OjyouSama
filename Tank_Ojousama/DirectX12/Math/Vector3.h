#pragma once

class Vector2;

class Vector3
{
public:
	float x;
	float y;
	float z;
	Vector3();
	Vector3(float x,float y,float z);
	Vector3(const Vector2& vec2, float z = 0.f);
	Vector3(const Vector3& vec3);
	~Vector3();

	float Length()const;
	static Vector3 Lerp(const Vector3& v1,const Vector3& v2,float t);
	void normalize();
	Vector3& normal();//???K??
	static Vector3 normalize(const Vector3& vec);
	float dot(const Vector3& v)const;//????
	static float dot(const Vector3& v,const Vector3& v2);//????
	Vector3 cross(const Vector3&v)const;//?O??
	static Vector3 cross(const Vector3&v,const Vector3& v2);//?O??
	//?P?????Z?q?I?[?o?[???[?h
	Vector3 operator +()const;
	Vector3 operator -()const;
	//???????Z?q?I?[?o?[???[?h
	Vector3& operator=(const Vector3& vec);
	Vector3& operator += (const Vector3& v);
	Vector3& operator -= (const Vector3& v);
	Vector3& operator *=(const Vector3& v);
	Vector3& operator *=(float s);
	Vector3& operator /=(float s);

	static const Vector3 zero;
	static const Vector3 one;

	static const Vector3 right;
	static const Vector3 up;
	static const Vector3 forward;
	static const Vector3 left;
	static const Vector3 down;
	static const Vector3 back;
	static const Vector3 Infinity;
private:

};
//?񍀉??Z?q?I?[?o?[???[?h
const Vector3 operator + (const Vector3& v1, const Vector3& v2);
const Vector3 operator - (const Vector3& v1, const Vector3& v2);
const Vector3 operator * (const Vector3& left, const Vector3& right);
const Vector3 operator * (const Vector3& v, float s);
const Vector3 operator * (float s, const Vector3& v);
const Vector3 operator / (const Vector3& v, float s);
