#pragma once
#include "BaseEnemy.h"

class Sound;
//近距離攻撃の敵
class BlowEnemy : public BaseEnemy
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="position">初期位置</param>
	/// <param name="angle">初期角度</param>
	/// <param name="num">識別番号</param>
	/// <param name="advanceFlag">拠点進行モードか？</param>
	/// <param name="advanceDirection">拠点進行時に進む方向</param>
	BlowEnemy(
		const Vector3 & position,
		const Vector3 & angle,
		int num,
		bool advanceFlag = false,
		const Vector3& advanceDirection = Vector3(0.0f, 0.0f, 1.0f)
	);

	/// <summary>
	/// デストラクタ
	/// </summary>
	~BlowEnemy();

private:

	//BaseEnemyから継承
	virtual void EnemyInit() override;
	virtual void EnemyUpdate() override;
	virtual void EnemyRend() override;
	virtual void EnemyOnCollision(BaseCollider* col) override;
	virtual void EnemyImGuiDebug() override;

	/*索敵状態*/
	virtual void Search() override;

	/*追跡状態*/
	virtual void Warning() override;

	/*攻撃状態*/
	virtual void Attack() override;

private:

	/*オブジェクトを一度だけ生成する*/
	void CreateOneObject();

private:
	//ObjectManager* objManager;                //オブジェクト管理者
	//shared_ptr<ModelRenderer> modelRender;	  //モデル描画
	//shared_ptr<ParticleManager> effectManager;//エフェクト描画

	AttackArea* attackArea;  //攻撃範囲クラス

	Vector3 patrolPoint[4];
	int pointCount;

	bool onTrigger;

	//歩行アニメーション関連
	float mLegRotate = 0.0f;//脚の現在の回転量
	const float LEG_SPEED = 2.0f; //脚の回転速度
	const float LEG_RANGE = 15.0f;//脚の最大回転量
	bool mRotDirection = false;   //回転方向

	string mLeftLeg;   //左足のモデル名
	string mLLegNumber;//左足 + 識別番号
	string mRightLeg;  //右足のモデル名
	string mRLegNumber;//右足 + 識別番号
	string mHead;      //頭の名前
	string mHeadNumber;//頭 + 識別番号
	string mBody;      //体の名前
	string mBodyNumber;//体 + 識別番号

	std::shared_ptr<Sound> mSound;
	std::shared_ptr<Sound> mSE;
};