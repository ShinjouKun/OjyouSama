#pragma once
#include "CollisionPrimitive.h"

class Collision
{
public:
	Collision();
	~Collision();
	static bool StoSColl(const Sphere& s1, const Sphere& s2,Vector3* inter);//���΋�
	static bool AABBColl(const AABB& box1, const AABB& box2);//���Δ�
	static bool StoAABBCol(const Sphere& s, const AABB& b);
	static bool StoPColl(const Sphere& sphere, const Plane& plane, Vector3* inter = nullptr);//���Ε���
	static bool StoRColl(const Ray& ray, const Sphere& sphere,float *distance = nullptr ,Vector3* inter = nullptr);//���C�Ƌ�
	static void PointToTriangle(const Vector3& point, const Triangle&triangle, Vector3* closest);//�_�ΎO�p�`
	static bool StoTColl(const Sphere& sphere, const Triangle& triangle, Vector3* inter = nullptr);//���ΎO�p�`
private:

};
