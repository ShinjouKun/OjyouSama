#pragma once
#include "Weapon.h"


//打ち上げ弾(迫撃砲用)やまなりな軌道
class LaunchBullet : public Weapon
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="position">生成位置</param>
	/// <param name="targetPosition">目標位置</param>
	/// <param name="objectManager">オブジェクトマネージャー</param>
	/// <param name="modelRender">モデルレンダー</param>
	/// <param name="particleManager">パーティクルマネージャー</param>
	/// <param name="objectType">タイプ</param>
	/// <param name="num">識別番号</param>
	LaunchBullet(
		const Vector3& position,
		const Vector3& targetPosition,
		ObjectManager * objectManager,
		std::shared_ptr<ModelRenderer> modelRender,
		shared_ptr<ParticleManager> particleManager,
		ObjectType objectType,
		int num,
		const bool isApple = false
	);

	~LaunchBullet();

	/*弾の種類をリンゴに変更(Initで書いてね)*/
	void ChangeApple(const bool value);

private:

	// Weapon を介して継承されました
	virtual void Init() override;
	virtual void Update() override;
	virtual void Rend() override;
	virtual void ImGuiDebug() override;
	virtual void OnCollison(BaseCollider * col) override;

	/*弾の動き*/
	void Move();

private:

	ObjectManager * mObjectManager;
	std::shared_ptr<ModelRenderer> mModelRender;
	shared_ptr<ParticleManager> mParticleManager;
	shared_ptr<ParticleEmitterBox> mParticleEmitter;

	Vector3 mTargetPosition;//射撃位置
	Vector3 mScale;         //サイズ
	Vector3 mCircleScale;

	int BULLET_DAMAGE = 10;//ダメージ
	float test = 0;

	float MOVE_SPEED = 1.0f;//移動速度
	float mVelocityY = 1.0f;//縦の移動量
	float mHalfLength;//生成位置と目標位置の距離の半分を求める

	bool mAppleFlag;
	bool mDrawArea;//警告オブジェクトを描画するか

	string PARTICLE_NAME = "Bom";//エフェクトの名前
	string EXPLOSION = "ExBom";//プレイヤーに当たった時のエフェクト

	//落下地点用オブジェクト
	string mBlockName = "BLOCK";
	string numBlock;

	//リンゴ
	string mAppleName = "Apple";
	string mAppleNum;
};