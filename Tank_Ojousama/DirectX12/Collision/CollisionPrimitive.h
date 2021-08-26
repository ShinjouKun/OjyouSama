#pragma once
#include "../Math/Math.h"
#include<cmath>
//��
struct Sphere
{
	Vector3 center = Vector3(0, 0, 0);//���S�_
	float radius = 1.0f;
};
//����
struct  Plane
{
	Vector3 normal = Vector3(0, 1, 0);//�@��
	float distance = 0.0f;//���_�i0,0,0�j�̋���
};
//�O�p�`
class Triangle
{
public:
	//���_�R��
	Vector3 p0;
	Vector3 p1;
	Vector3 p2;
	Vector3 normal;
};
//Box
struct AABB
{
	Vector3 max;//�ő�
	Vector3 min;//�ŏ��i���_�j
};

class CollisionPrimitive
{
public:
	CollisionPrimitive();
	~CollisionPrimitive();
private:

};
