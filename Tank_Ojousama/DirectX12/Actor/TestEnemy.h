#pragma once
#include <memory>
#include "BaseObject.h"    //オブジェクトのひな型
#include "ObjectManager.h" //オブジェクト管理者
#include "../Render/ModelRenderer.h"//モデル貼り付け
#include "../Render/TexRenderer.h"	 //ポリゴンの描画


#include "BreadCrumb.h"
#include "AttackArea.h"

/// <summary>
/// 敵クラス
/// </summary>
class TestEnemy : public BaseObject
{
private:

	//プレイヤーの情報
	struct SearchPoint
	{
		Vector3 position;//中心点
		float radius;    //半径
	}searchPoint;

	//扇の索敵範囲
	struct FanInfomation
	{
		Vector3 position;
		float fanRange;//角度
		float length;  //長さ
		float rotate;  //回転角度
	}fanInfo;

	//敵の行動状態
	enum ActionState
	{
		SEARCH,  //索敵
		WARNING, //警戒
		ATTACK,  //攻撃
	}actionState;

	//関数系
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="position">初期位置</param>
	/// <param name="angle">初期角度</param>
	/// <param name="objManager">オブジェクト管理者</param>
	/// <param name="modelRender">モデル描画</param>
	/// <param name="texRender">ポリゴン描画</param>
	/// <param name="num">識別番号</param>
	TestEnemy(
		Vector3 position,
		Vector3 angle,
		ObjectManager * objManager,
		shared_ptr<ModelRenderer>modelRender,
		shared_ptr<TexRenderer>texRender,
		int num
	);

	/// <summary>
	/// デストラクタ
	/// </summary>
	~TestEnemy();

private:

	//BaseObjectから継承
	virtual void Init() override;
	virtual void Update() override;
	virtual void Rend() override;
	virtual void OnCollison(BaseCollider* col) override;
	virtual void ImGuiDebug() override;

	/// <summary>
	/// 扇の情報を再設定
	/// </summary>
	/// <param name="range">なす角の角度</param>
	/// <param name="length">長さ</param>
	void SetFanInfo(float range = 60.0f, float length = 30.0f);

	/// <summary>
	/// 移動
	/// </summary>
	/// <param name="otherPosition">移動対象</param>
	void Move(const Vector3& otherPosition);

	/// <summary>
	/// 状態変更
	/// </summary>
	void ChangeState();

	/// <summary>
	/// オブジェクト検索
	/// </summary>
	void SearchObject();

	/// <summary>
    /// プレイヤー索敵
    /// </summary>
    /// <param name="player"></param>
	void SearchPlayer(BaseObject* player);

	void SearchPlayer();

	/// <summary>
	/// パンくず索敵
	/// </summary>
	void SearchBreadCrumb(BaseObject* breadcrumb);

	/// <summary>
	/// パンくずを辿る
	/// </summary>
	void TrackingBreadcrumb();

	/// <summary>
	/// 索敵
	/// </summary>
	void Search();

	/// <summary>
	/// 警戒
	/// </summary>
	void Warning();

	/// <summary>
	/// 攻撃
	/// </summary>
	void Attack();

	/// <summary>
	/// 無敵時間
	/// </summary>
	/// <param name="time">指定時間</param>
	void Invincible(int time);

	/// <summary>
	/// 移動方向を向くように調整
	/// </summary>
	void ChangeDirection();

	/// <summary>
	/// Y軸固定角度をベクトルに変換する
	/// </summary>
	/// <param name="angle">角度</param>
	/// <returns>角度の単位ベクトル</returns>
	Vector3& AngleToVectorY(float angle) const;

	/// <summary>
	/// Y軸固定でベクトルを角度に変換する
	/// </summary>
	/// <param name="vector"></param>
	/// <returns></returns>
	float VectorToAngleY(const Vector3& vector) const;

	/// <summary>
	/// 扇と点の当たり判定
	/// </summary>
	/// <param name="fan">扇</param>
	/// <param name="point">点</param>
	/// <returns>当たっていたらtrue</returns>
	bool IsHitFanToPoint(const FanInfomation& fan, const Vector3& point, float radius = 0.0f) const;

	//変数用
private:

	ObjectManager* objManager;
	shared_ptr<ModelRenderer> modelRender;
	//key = 識別番号　：　value = 位置
	std::unordered_map<int, Vector3> breadMap;//パンくずリスト

	int HP;        //体力
	int mapCount;  //マップの要素数
	int breadCount;//落としたパンくずの総数
	int number = 0;//識別番号

	int warningCount;   //警戒カウント
	int warningTime;    //警戒時間
	int attackCount;    //攻撃カウント
	int attackTime;	    //攻撃時間
	int invincibleCount;//無敵カウント

	float radius;         //当たり判定の半径
	float fanRotateOrigin;//扇の最初の向き
	float swingRange;     //首振り角度の範囲
	float barrelAngle;    //砲塔の向き
	float turretAngle;    //砲身の向き
	float attackLength;   //攻撃範囲

	bool isDamage;          //ダメージを受けているか
	bool isInvincible;      //無敵時間か
	bool hitSensor;         //センサーが当たったか？
	bool swingSensor;       //首振りの状態(true=左、false=右)
	bool trackingPlayer;    //プレイヤーに当たっているか
	bool trackingBreadcrumb;//パンくずを拾っているか
	bool frontMove;         //正面に移動しているか
	bool backMove;          //後ろに移動しているか
	bool oneShot;           //オブジェクトを一度だけ生成する。

	Vector3 scale;       //大きさ(1.5)
	Vector3 lastBreadPos;//パンくずの最後の位置を保存用
	Vector3 previousPos; //前フレームの位置(角度変更用)

	string tankBarrel;//砲身の名前登録
	string tankTurret;//砲塔の名前登録
	string tankBody;  //車体の名前登録
	string num;       //string型の識別番号
	string numBarrel; //識別番号+砲身の名前
	string numTurret; //識別番号+砲塔の名前
	string numBody;	  //識別番号+車体の名前

	AttackArea* area;//攻撃判定クラス(可視化できる)
};