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

	/*死亡したかどうか*/
	bool GetDeadFlag() const;

	/*目標位置を隊長から受け取る*/
	void ReceivePosition(const Vector3& targetPosition);

	/*攻撃対象の位置と、攻撃指令を隊長から受け取る*/
	void ReceiveAttackCommand(const Vector3& attackTarget, const bool attackCommand);

	/*索敵対象の位置を隊長から受け取る*/
	void ReceiveSearchCommand(const Vector3& searchTarget, const bool searchCommand);

	/*隊長の死を受け取る*/
	void ReceiveCaptainLost(const bool captainLost);

	/*索敵結果を隊長に知らせる*/
	bool SendSearchResult() const;

	/*索敵結果の位置を隊長に知らせる*/
	Vector3 SendSearchPosition() const;

private:

	virtual void Init() override;
	virtual void Update() override;
	virtual void Rend() override;
	virtual void ImGuiDebug() override;
	virtual void OnCollison(BaseCollider * col) override;

	/*目標位置に移動&近かったら停止*/
	void MoveTarget(const Vector3& targetPosition, const float distance);

	/*指定距離以内かどうかを調べる*/
	bool WithinDistance(const Vector3& targetPosition, const float distance);

	/*角度をベクトルに変換*/
	Vector3 AngleToVectorY(float angle)const;


#pragma region 攻撃のステップ(別のいい方法がありそう)

	void AttackStep_NONE();
	void AttackStep_AIMING();
	void AttackStep_FIRE();
	void AttackStep_RELOAD();

#pragma endregion

private:

	enum AttackStep
	{
		NONE,  //攻撃してない
		AIMING,//照準合わせ中
		FIRE,  //弾発射！
		RELOAD,//リロード中
	};
	AttackStep mAttackStep;

	ObjectManager * mObjManager;
	shared_ptr<ModelRenderer> mModelRender;
	shared_ptr<ParticleManager> mEffectManager;

	shared_ptr<Timer> mAimingTime;//狙う時間
	shared_ptr<Timer> mReloadTime;//リロード時間
	shared_ptr<Timer> mRandomMoveTimer;//ちりじりになる

	Vector3 scale;
	Vector3 mFixedPosition; //固定位置
	Vector3 mAttackTarget;  //攻撃位置
	Vector3 mSearchTarget;  //索敵位置
	Vector3 mSearchPosition;//索敵結果位置
	Vector3 mRandomDirection;//ランダム移動方向
	Vector3 mPreviousPosition;

	float radius;//半径
	float barrelAngle;
	float turretAngle;
	const float SEARCH_RANGE = 30.0f;//索敵範囲(円)

	int mBulletNumber = 0;//発射した弾数
	int testStep = 0;

	bool mDeadFlag;     //死亡したかどうか
	bool mAttackCommand;//攻撃指令
	bool mSearchCommand;//索敵指令
	bool mSearchResult; //プレイヤーを発見したか
	bool mCaptainLost;  //隊長を失ったかどうか

	string tankBarrel;//砲身の名前登録
	string tankTurret;//砲塔の名前登録
	string tankBody;  //車体の名前登録
	string num;       //string型の識別番号
	string numBarrel; //識別番号+砲身の名前
	string numTurret; //識別番号+砲塔の名前
	string numBody;	  //識別番号+車体の名前
};
