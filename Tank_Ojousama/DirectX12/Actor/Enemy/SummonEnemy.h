#pragma once
#include "../BaseObject.h"
#include "../ObjectManager.h"
#include "../../Render/ModelRenderer.h"
#include "../../Render/ParticleManager.h"

class AttackArea;
class Timer;

//ボスに召喚される雑魚敵
class SummonEnemy : public BaseObject
{
public:

	SummonEnemy(
		const Vector3 & position,
		const Vector3 & angle,
		ObjectManager * objManager,
		shared_ptr<ModelRenderer>modelRender,
		shared_ptr<ParticleManager> effectManager,
		int num
	);

	~SummonEnemy();

private:

	virtual void Init() override;
	virtual void Update() override;
	virtual void Rend() override;
	virtual void ImGuiDebug() override;
	virtual void OnCollison(BaseCollider * col) override;

	/*一度だけオブジェクトを生成*/
	void CreateObject();

	/*指定距離以内かどうかを調べる*/
	bool WithinDistance(const Vector3& targetPosition, const float distance);

private:

	enum AttackStep
	{
		FALL_DOWN,//倒れこむ
		WAIT,     //倒れたまま停止
		GET_UP,   //起き上がる
	};
	AttackStep mAttackStep;

	ObjectManager * mObjManager;
	shared_ptr<ModelRenderer> mModelRender;
	shared_ptr<ParticleManager> mEffectManager;

	shared_ptr<AttackArea> mAttackArea;
	shared_ptr<Timer> mGetupTimer;

	Vector3 mScale;
	Vector3 mPlayerPosition;
	Vector3 mPreviousPosition;

	const float SEARCH_LENGTH = 50.0f;//この距離以内になったら追跡する
	const float FIND_LRNGTH = 20.0f;  //この距離以内になったら突撃する
	const float ATTACK_LENGTH = 5.0f; //この距離以内になったら攻撃する
	float tt = 0.0f;
	float t = 3.0f;

	bool mWithinPlayerFlag;//範囲内にプレイヤーがいるか？
	bool mAttackFlag;//攻撃中か
	bool mStep;//true = 右, false = 左
	bool mCreateObject;

	string mModelNum;
	string mModelName01 = "SummonEnemy";
	string mModelNumName01;

	int test = 0;
};