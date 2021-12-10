#pragma once
#include"Vector3.h"
#include<cmath>

class Matrix4
{
public:
	float m[4][4];
	Matrix4();
	~Matrix4();
	explicit Matrix4(float mat[4][4]);
	static const Matrix4 Identity;
	friend Matrix4 operator *(const Matrix4& a, const Matrix4& b);
	Matrix4& operator *=(const Matrix4& a);

	static Matrix4 createTranslation(const Vector3& trans);
	static Matrix4 createScale(float x, float y, float z);
	static Matrix4 createScale(const Vector3& scale);
	static Matrix4 Rotate(float x,float y,float z);
	static Matrix4 RotateX(float angleX);//X‰ñ“]
	static Matrix4 RotateY(float angleY);//Y‰ñ“]
	static Matrix4 RotateZ(float angleZ);//Z‰ñ“]

	static Matrix4 createLookAt(const Vector3& eye, const Vector3& target, const Vector3& up);

	static Matrix4 createPerspectiveFOV(float fovY, float width, float height, float _near, float _far);
	
private:

};
Vector3& operator *=(Vector3& v, const Matrix4& m);