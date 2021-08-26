#pragma once
#include "../Math/Math.h"
#include<cmath>
//球
struct Sphere
{
	Vector3 center = Vector3(0, 0, 0);//中心点
	float radius = 1.0f;
};
//平面
struct  Plane
{
	Vector3 normal = Vector3(0, 1, 0);//法線
	float distance = 0.0f;//原点（0,0,0）の距離
};
//三角形
class Triangle
{
public:
	//頂点３つ
	Vector3 p0;
	Vector3 p1;
	Vector3 p2;
	Vector3 normal;
};
//Box
struct AABB
{
	Vector3 max;//最大
	Vector3 min;//最小（原点）
};

class CollisionPrimitive
{
public:
	CollisionPrimitive();
	~CollisionPrimitive();
private:

};
