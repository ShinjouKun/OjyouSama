#pragma once
#include "BaseObject.h"
#include"ObjectManager.h"
#include "../Render/ModelRenderer.h"
#include "../Render/ParticleManager.h"

class Timer;

class MemberEnemy : public BaseObject
{
public:

	MemberEnemy(
		const Vector3 & position,
		const Vector3 & angle,
		ObjectManager * objManager,
		shared_ptr<ModelRenderer>modelRender,
		shared_ptr<ParticleManager> effectManager,
		int num
	);

	~MemberEnemy();

	/*€–S‚µ‚½‚©‚Ç‚¤‚©*/
	bool GetDeadFlag() const;

	/*–Ú•WˆÊ’u‚ğ‘à’·‚©‚çó‚¯æ‚é*/
	void ReceivePosition(const Vector3& targetPosition);

	/*UŒ‚‘ÎÛ‚ÌˆÊ’u‚ÆAUŒ‚w—ß‚ğ‘à’·‚©‚çó‚¯æ‚é*/
	void ReceiveAttackCommand(const Vector3& attackTarget, const bool attackCommand);

	/*õ“G‘ÎÛ‚ÌˆÊ’u‚ğ‘à’·‚©‚çó‚¯æ‚é*/
	void ReceiveSearchCommand(const Vector3& searchTarget, const bool searchCommand);

	/*‘à’·‚Ì€‚ğó‚¯æ‚é*/
	void ReceiveCaptainLost(const bool captainLost);

	/*õ“GŒ‹‰Ê‚ğ‘à’·‚É’m‚ç‚¹‚é*/
	bool SendSearchResult() const;

	/*õ“GŒ‹‰Ê‚ÌˆÊ’u‚ğ‘à’·‚É’m‚ç‚¹‚é*/
	Vector3 SendSearchPosition() const;

private:

	virtual void Init() override;
	virtual void Update() override;
	virtual void Rend() override;
	virtual void ImGuiDebug() override;
	virtual void OnCollison(BaseCollider * col) override;

	/*–Ú•WˆÊ’u‚ÉˆÚ“®&‹ß‚©‚Á‚½‚ç’â~*/
	void MoveTarget(const Vector3& targetPosition, const float distance);

	/*w’è‹——£ˆÈ“à‚©‚Ç‚¤‚©‚ğ’²‚×‚é*/
	bool WithinDistance(const Vector3& targetPosition, const float distance);

	/*Šp“x‚ğƒxƒNƒgƒ‹‚É•ÏŠ·*/
	Vector3 AngleToVectorY(float angle)const;


#pragma region UŒ‚‚ÌƒXƒeƒbƒv(•Ê‚Ì‚¢‚¢•û–@‚ª‚ ‚è‚»‚¤)

	void AttackStep_NONE();
	void AttackStep_AIMING();
	void AttackStep_FIRE();
	void AttackStep_RELOAD();

#pragma endregion

private:

	enum AttackStep
	{
		NONE,  //UŒ‚‚µ‚Ä‚È‚¢
		AIMING,//Æ€‡‚í‚¹’†
		FIRE,  //’e”­ËI
		RELOAD,//ƒŠƒ[ƒh’†
	};
	AttackStep mAttackStep;

	ObjectManager * mObjManager;
	shared_ptr<ModelRenderer> mModelRender;
	shared_ptr<ParticleManager> mEffectManager;

	shared_ptr<Timer> mAimingTime;//‘_‚¤ŠÔ
	shared_ptr<Timer> mReloadTime;//ƒŠƒ[ƒhŠÔ
	shared_ptr<Timer> mRandomMoveTimer;//‚¿‚è‚¶‚è‚É‚È‚é

	Vector3 scale;
	Vector3 mFixedPosition; //ŒÅ’èˆÊ’u
	Vector3 mAttackTarget;  //UŒ‚ˆÊ’u
	Vector3 mSearchTarget;  //õ“GˆÊ’u
	Vector3 mSearchPosition;//õ“GŒ‹‰ÊˆÊ’u
	Vector3 mRandomDirection;//ƒ‰ƒ“ƒ_ƒ€ˆÚ“®•ûŒü
	Vector3 mPreviousPosition;

	float radius;//”¼Œa
	float barrelAngle;
	float turretAngle;
	const float SEARCH_RANGE = 30.0f;//õ“G”ÍˆÍ(‰~)

	int mBulletNumber = 0;//”­Ë‚µ‚½’e”
	int mMoveDirection = 0;

	bool mDeadFlag;     //€–S‚µ‚½‚©‚Ç‚¤‚©
	bool mAttackCommand;//UŒ‚w—ß
	bool mSearchCommand;//õ“Gw—ß
	bool mSearchResult; //ƒvƒŒƒCƒ„[‚ğ”­Œ©‚µ‚½‚©
	bool mCaptainLost;  //‘à’·‚ğ¸‚Á‚½‚©‚Ç‚¤‚©

	//•àsƒAƒjƒ[ƒVƒ‡ƒ“ŠÖ˜A
	float mLegRotate = 0.0f;//‹r‚ÌŒ»İ‚Ì‰ñ“]—Ê
	const float LEG_SPEED = 1.5f; //‹r‚Ì‰ñ“]‘¬“x
	const float LEG_RANGE = 15.0f;//‹r‚ÌÅ‘å‰ñ“]—Ê
	bool mRotDirection = false;   //‰ñ“]•ûŒü

	string tankBarrel;//–Cg‚Ì–¼‘O“o˜^
	string tankBarrel2;
	string tankTurret;//–C“ƒ‚Ì–¼‘O“o˜^
	string tankBody;  //Ô‘Ì‚Ì–¼‘O“o˜^
	string num;       //stringŒ^‚Ì¯•Ê”Ô†
	string numBarrel; //¯•Ê”Ô†+–Cg‚Ì–¼‘O
	string numBarrel2;
	string numTurret; //¯•Ê”Ô†+–C“ƒ‚Ì–¼‘O
	string numBody;	  //¯•Ê”Ô†+Ô‘Ì‚Ì–¼‘O
};
