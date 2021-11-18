#pragma once
#include "BaseEnemy.h"
#include "../Render/ParticleManager.h"

class SniperEnemy : public BaseEnemy
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="position">初期位置</param>
	/// <param name="angle">初期角度</param>
	/// <param name="objManager">オブジェクト管理者</param>
	/// <param name="modelRender">モデル描画</param>
	/// <param name="texRender">画像描画</param>
	/// <param name="effect">エフェクト描画</param>
	/// <param name="num">識別番号</param>
	SniperEnemy(
		const Vector3& position,
		const Vector3& angle,
		ObjectManager * objManager,
		shared_ptr<ModelRenderer>modelRender,
		shared_ptr<TexRenderer>texRender,
		shared_ptr<ParticleManager> effect,
		int num
	);

	/// <summary>
	/// デストラクタ
	/// </summary>
	~SniperEnemy();

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

	/*自爆状態*/
	virtual void Destruct() override;

private:
	ObjectManager* objManager;                //オブジェクト管理者
	shared_ptr<ModelRenderer> modelRender;	  //モデル描画
	shared_ptr<ParticleManager> effectManager;//エフェクト描画

	/*弾の識別番号*/
	int bulletNumber = 0;

	//歩行アニメーション関連
	float mLegRotate = 0.0f;//脚の現在の回転量
	const float LEG_SPEED = 1.5f; //脚の回転速度
	const float LEG_RANGE = 15.0f;//脚の最大回転量
	bool mRotDirection = false;   //回転方向

	string tankBarrel2;
	string numBarrel2;
};