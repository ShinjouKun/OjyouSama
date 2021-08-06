#pragma once
#include "../Math/Vector2.h"
#include "../Math/Vector3.h"
#include "../Math/Vector4.h"
//#include"Vector2.h"
//#include"Vector3.h"
//#include"Vector4.h"
#include<cmath>
//�����蔻��n
//#include"CollisonInfo.h"
#include "../Collision/CollisonInfo.h"
class BaseCollider;
enum ObjectType//�I�u�W�F�N�g�̃^�O
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
	//�����蔻��
	void SetCollidder(BaseCollider* collider);
	//�Փˎ��R�[���o�b�N
	virtual void OnCollison(const CollisonInfo& info) = 0;
	//Get�@Set
	virtual ObjectType GetType() { return objType; }
	virtual bool GetDeath() { return death; }
	virtual Vector3 GetPosition() { return position; }
	//AI�p
	size_t GetID()const;
	void SetID(size_t id);
	size_t AiID;//AI�ԍ�
protected:
	ObjectType objType;
	bool death;//����ł��邩
	Vector3 position;
	Vector3 velocity;
    Vector3 angle;
	float speed;
	//PI
	const float PI = 3.1415926535897932384626433832795f;
	//�����蔻��
	BaseCollider* collider = nullptr;
	
};
