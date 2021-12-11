#pragma once
#include "BaseEnemy.h"

class Timer;

class Explosion;

//ŒÅ’è–C‘ä‚Ì“G
class MortarEnemy :public BaseEnemy
{
public:

	MortarEnemy(
		const Vector3& pos,
		const Vector3& ang,
		int num
	);

	~MortarEnemy();

private:

	//BaseEnemy‚©‚çŒp³
	virtual void EnemyInit() override;
	virtual void EnemyUpdate() override;
	virtual void EnemyRend() override;
	virtual void EnemyOnCollision(BaseCollider* col) override;
	virtual void EnemyImGuiDebug() override;

	void AttackStep_AIMING();
	void AttackStep_FIRE();
	void AttackStep_RELOAD();

	/*¶‘¶ó‘Ô‚ğŠÄ‹*/
	void CheckAlive();

	/*UŒ‚*/
	void Attack();

	/*€–S‚Ì‰‰o*/
	void DeathAnimation();

	/*‹ó‚Éã¸*/
	void DeathAnimeStep_RiseSky();

	/*”š”­&”•b’â~*/
	void DeathAnimeStep_Explosion();

private:

	/*€–SƒAƒjƒ[ƒVƒ‡ƒ“‚Ì‘JˆÚó‹µ*/
	enum DeathAnimationStep
	{
		RISE_SKY, //ã‹ó‚É•‘‚¢ã‚ª‚é
		EXPLOSION,//Á–Å‚·‚é
	};
	DeathAnimationStep mDeathStep;

	/*UŒ‚‚Ì‘JˆÚó‹µ*/
	enum AttackStep
	{
		AIMING,//Æ€‡‚í‚¹’†
		FIRE,  //’e”­ËI
		RELOAD,//ƒŠƒ[ƒh’†
	};
	AttackStep mAttackStep;//UŒ‚—pó‘Ô

	shared_ptr<Timer> mAimingTime;//‘_‚¤ŠÔ
	shared_ptr<Timer> mReloadTime;//ƒŠƒ[ƒhŠÔ
	shared_ptr<Timer> mRiseTime;  //ã¸ŠÔ
	shared_ptr<Timer> mDeathTime; //Š®‘S‚É€‚Ê‚Ü‚Å‚ÌŠÔ
	shared_ptr<Sound> mAttackSE;  //UŒ‚‚ÌSE
	shared_ptr<Sound> mDamageSE;  //UŒ‚‚ÌSE
	shared_ptr<ParticleEmitterBox> mParticleEmitter;

	std::shared_ptr<Explosion> mExplosion;//¬”š”­

	Vector3 mScale;         //‘å‚«‚³
	Vector3 mTargetPosition;//’e‚ğ’e‚Ì—‰º’n“_

	float mRadius;    //”¼Œa
	float barrelAngle;//UŒ‚‚ÌŒü‚«
	float mHandAngle; //˜r‚ÌŠp“x

	int mBulletNumber;//”­Ë‚µ‚½’e”

	bool mFireFlag;//’e‚ğ”­Ë‚µ‚½‚©
	bool mDeathAnimation;//€–SƒAƒjƒ[ƒVƒ‡ƒ“‚ğŠJn‚·‚é‚©
	bool mDeadFlag;//€‚ÊuŠÔ‚Étrue‚É‚È‚éƒtƒ‰ƒO(•ÛŒ¯)

	string mHandRight;      //–Cg‚Ì–¼‘O“o˜^
	string mHandLeft;       //–C“ƒ‚Ì–¼‘O“o˜^
	string mBody;           //Ô‘Ì‚Ì–¼‘O“o˜^
	string num;             //stringŒ^‚Ì¯•Ê”Ô†
	string mHandRNum;       //¯•Ê”Ô†+–Cg‚Ì–¼‘O
	string mHandLNum;       //¯•Ê”Ô†+–C“ƒ‚Ì–¼‘O
	string mBodyNum;        //¯•Ê”Ô†+Ô‘Ì‚Ì–¼‘O
	string EXPLOSION_EFFECT;//ƒGƒtƒFƒNƒg–¼
};