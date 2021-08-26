#pragma once
#include "../Math/Vector2.h"
#include "../Math/Vector3.h"
#include "../Math/Vector4.h"
#include<cmath>
//当たり判定系
#include "../Collision/CollisonInfo.h"
class BaseCollider;
class SphereCollider;
class AABBCollider;
enum ObjectType//オブジェクトのタグ
{
	PLAYER,
	ENEMY,
	BULLET,
	GROUND,
	ENEMYBULLET,
	BOSS,
	BLOCK,
};

class BaseObject
{
public:
	BaseObject() =default;
	virtual~BaseObject();
	virtual void Init() = 0;
	virtual void Update() = 0;
	virtual void Rend() = 0;
	virtual void ImGuiDebug() = 0;
	virtual void CollisonUpdate();
	virtual Vector3 RotateY(float angle);
	//当たり判定
	void SetCollidder(BaseCollider* collider);
	void SetCollidder(const Vector3& center,float rad);//円
	//void SetCollidder(const Vector3& min,const Vector3& max);//AABB
	//衝突時コールバック
	virtual void OnCollison(BaseCollider* col) = 0;
	//Get　Set
	virtual ObjectType GetType() { return objType; }
	virtual bool GetDeath() { return death; }
	virtual Vector3 GetPosition() { return position; }
	void SetNumber(int num);
	int GetNumber();

	//個体番号
	int number;
	//AI用
	size_t GetID()const;
	void SetID(size_t id);
	size_t AiID;//AI番号
protected:
	ObjectType objType;
	bool death;//死んでいるか
	Vector3 position;
	Vector3 velocity;
    Vector3 angle;
	float speed;
	//PI
	const float PI = 3.1415926535897932384626433832795f;
	//当たり判定
	BaseCollider* collider = nullptr;
};
