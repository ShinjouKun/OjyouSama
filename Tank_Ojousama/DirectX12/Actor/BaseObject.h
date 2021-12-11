#pragma once
#include "../Math/Vector2.h"
#include "../Math/Vector3.h"
#include "../Math/Vector4.h"
#include"../Math/Easing.h"
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
	MIDDLEENEMY,
	BULLET,
	GROUND,
	ENEMYBULLET,
	BOSS,
	BLOCK,
	BREADCRUMB,
	ITEM,
	CAMEAR,
	ITEMBOX,
	DEFENCEPOINT,
	TREASURE,
	SMOKE,
	BORDER_LINE,
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
	virtual Vector3 RotateX(float angle);
	virtual Vector3 RotateY(float angle);
	int GetDamage() { return damage; }//ダメージ

	void SetHP(const int hp);

	int GetHP() const { return HP; }
	
	//当たり判定
	void SetCollidder(BaseCollider* collider);
	void SetCollidder(const Vector3& center,float rad);//円
	void SetCollidder(const Vector3& min,const Vector3& max);//AABB
	//衝突時コールバック
	virtual void OnCollison(BaseCollider* col) = 0;
	//Get　Set
	virtual ObjectType GetType() { return objType; }
	virtual int GetHp() const { return HP; }
	virtual bool GetDeath() { return death; }
	virtual Vector3 GetPosition() { return position; }
	virtual Vector3 GetAngle() { return angle; }
	virtual bool GetActive() { return IsActive; }
	virtual bool GetTreasure() {return getTreasure;}
	virtual int GetShotCount() { return shotMoney; }
	virtual bool GetSceneFinish() { return sceneCamFinish; }
	void SetShotCount(int money);
	void SetTresureGet(bool val);
	void SetActive(bool val);
	void SetNumber(int num);
	void SetSceneCamFinish(bool val);
	int GetNumber();

	//個体番号
	int number = 0;
	//AI用
	size_t GetID()const;
	void SetID(size_t id);
	size_t AiID;//AI番号
protected:
	bool sceneCamFinish;//カメラ演出終了
	int shotMoney;//一発ごとの値段
	bool getTreasure;//クリアに必要なアイテムを確保したか？
	ObjectType objType;
	int HP;
	bool death;//死んでいるか
	bool IsActive;//カメラの範囲内にいるか（又はプレイヤーの視界にいるか）
	Vector3 position;
	Vector3 velocity;
    Vector3 angle;
	int damage;
	float speed;
	//PI
	const float PI = 3.1415926535897932384626433832795f;
	//当たり判定
	BaseCollider* collider = nullptr;
};
