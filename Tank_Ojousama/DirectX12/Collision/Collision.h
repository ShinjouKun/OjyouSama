#pragma once
#include "CollisionPrimitive.h"

class Collision
{
public:
	Collision();
	~Collision();
	static bool StoSColl(const Sphere& s1, const Sphere& s2,Vector3* inter);//球対球
	static bool AABBColl(const AABB& box1, const AABB& box2);//箱対箱
	static bool StoAABBCol(const Sphere& s, const AABB& b);
	static bool StoPColl(const Sphere& sphere, const Plane& plane, Vector3* inter = nullptr);//球対平面
	static bool StoRColl(const Ray& ray, const Sphere& sphere,float *distance = nullptr ,Vector3* inter = nullptr);//レイと球
	static void PointToTriangle(const Vector3& point, const Triangle&triangle, Vector3* closest);//点対三角形
	static bool StoTColl(const Sphere& sphere, const Triangle& triangle, Vector3* inter = nullptr);//球対三角形
private:

};
