#pragma once
#include "ObjectManager.h"
#include "BaseObject.h"
#include "../Render/ModelRenderer.h"
#include "../Collision/SpherCollider.h"

//敵の攻撃判定クラス(パーティクル出せるようにしたい)
class AttackArea : public BaseObject
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="pos">位置</param>
	/// <param name="ang">角度</param>
	/// <param name="objM">オブジェクト管理者</param>
	/// <param name="modelR">モデル描画</param>
	/// <param name="num">識別番号</param>
	AttackArea(
		Vector3 pos,
		Vector3 ang,
		ObjectManager* objM,
		shared_ptr<ModelRenderer>modelR,
		int num
	);

	/// <summary>
	/// デストラクタ
	/// </summary>
	~AttackArea();

	/// <summary>
	/// 死亡するまでのカウントダウン
	/// </summary>
	void DeathCountDown();

	/// <summary>
	/// 死亡するまでのカウントダウンを設定
	/// </summary>
	/// <param name="value">死亡させるか</param>
	/// <param name="time">死亡するまでの時間</param>
	void SetDestroy(bool value ,int time);

	/// <summary>
	/// 表示状態の変更(初期化)
	/// </summary>
	/// <param name="value">表示状態</param>
	/// <param name="pos">位置</param>
	/// <param name="ang">角度</param>
	void SetActive(bool value, const Vector3& pos = Vector3().zero, const Vector3& ang = Vector3().zero, const Vector3& size = Vector3().one);

	/// <summary>
	/// 死亡状態の変更
	/// </summary>
	/// <param name="value">死亡しているか</param>
	void SetDeath(bool value);

private:
	virtual void Init() override;
	virtual void Update() override;
	virtual void Rend() override;
	virtual void OnCollison(BaseCollider* info) override;
	virtual void ImGuiDebug() override;

private:
	ObjectManager* objManager;
	shared_ptr<ModelRenderer> modelRender;

	Vector3 scale;//大きさ

	int number = 0;//識別番号
	int destroyTime;//削除時間
	int destroyCount;//削除カウント

	float radius;//半径

	bool isActive;//表示状態
	bool isDestroy;//削除フラグ

	SphereCollider* spehereCollider;

	string name;   //モデル名
	string key;	   //識別番号
	string keyname;//名前と番号の合成
};