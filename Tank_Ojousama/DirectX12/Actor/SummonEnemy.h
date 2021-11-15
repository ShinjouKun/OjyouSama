#pragma once
#include "BaseObject.h"
#include"ObjectManager.h"
#include "../Render/ModelRenderer.h"
#include "../Render/ParticleManager.h"

class AttackArea;
class Timer;

//ƒ{ƒX‚É¢Š«‚³‚ê‚éG‹›“G
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

	/*w’è‹——£ˆÈ“à‚©‚Ç‚¤‚©‚ğ’²‚×‚é*/
	bool WithinDistance(const Vector3& targetPosition, const float distance);

private:

	enum AttackStep
	{
		FALL_DOWN,//“|‚ê‚±‚Ş
		WAIT,     //“|‚ê‚½‚Ü‚Ü’â~
		GET_UP,   //‹N‚«ã‚ª‚é
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

	const float SEARCH_LENGTH = 50.0f;//‚±‚Ì‹——£ˆÈ“à‚É‚È‚Á‚½‚ç’ÇÕ‚·‚é
	const float FIND_LRNGTH = 20.0f;  //‚±‚Ì‹——£ˆÈ“à‚É‚È‚Á‚½‚ç“ËŒ‚‚·‚é
	const float ATTACK_LENGTH = 5.0f; //‚±‚Ì‹——£ˆÈ“à‚É‚È‚Á‚½‚çUŒ‚‚·‚é
	float tt = 0.0f;
	float t = 3.0f;

	bool mWithinPlayerFlag;//”ÍˆÍ“à‚ÉƒvƒŒƒCƒ„[‚ª‚¢‚é‚©H
	bool mAttackFlag;//UŒ‚’†‚©
	bool mStep;//true = ‰E, false = ¶

	string mModelNum;
	string mModelName01 = "SummonEnemy";
	string mModelNumName01;

	int test = 0;
};