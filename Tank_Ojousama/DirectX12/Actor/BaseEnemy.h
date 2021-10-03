#pragma once
#include <memory>
#include "BaseObject.h"             //オブジェクトのひな型
#include "ObjectManager.h"          //オブジェクト管理者
#include "BreadCrumb.h"             //パンくず
#include "../Render/ModelRenderer.h"//モデル貼り付け
#include "../Render/TexRenderer.h"	//ポリゴンの描画

#include "AttackArea.h"

/// <summary>
/// 敵の基底クラス
/// </summary>
class BaseEnemy : public BaseObject
{
protected:
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
		DESTRUCT,//自爆
	}actionState;

public:
	BaseEnemy() = default;
	virtual ~BaseEnemy();

	//BaseObjectクラスから継承
	virtual void Init() override;
	virtual void Update() override;
	virtual void Rend() override;
	virtual void OnCollison(BaseCollider* col) override;
	virtual void ImGuiDebug() override;

	//このクラスの子どもが使う用メソッド
	virtual void EnemyInit() = 0;
	virtual void EnemyUpdate() = 0;
	virtual void EnemyRend() = 0;
	virtual void EnemyOnCollision(BaseCollider* col) = 0;
	virtual void EnemyImGuiDebug() = 0;

	virtual void Search() = 0; //索敵
	virtual void Warning() = 0;//追跡
	virtual void Attack() = 0; //攻撃
	virtual void Destruct() = 0;//自爆

	/*行動状態の変更*/
	void ChangeState();

	/*オブジェクト検索まとめ(objManager)*/
	virtual void SearchObject(ObjectManager* objManager);

	/*ダメージ処理(ダメージ量,objManager)*/
	virtual void Damage(int damage, ObjectManager* objManager);

	/*無敵時間(ダメージを受けない時間)*/
	virtual void Invincible(int time);

	/*首振り索敵機能(左右何度まで回転するか)*/
	virtual void SwingDirection(float range);

	/*配列内のポイントを順番に巡回する(位置の配列,配列の要素数)*/
	virtual void PatrolPoint(const Vector3 points[],int pointCount);

	/*プレイヤー or パンくず を追いかける*/
	virtual void TrackingObject();

	/*瀕死時に自爆状態に変更する(自爆状態になる体力,自爆するかどうか)*/
	void DestructMode(int hpLine, bool destructMode);

	/*自爆行動(objManager, modelRender)(当たり判定オブジェクトをBase側で生成する)*/
	virtual void DestructAction(ObjectManager * objManager, shared_ptr<ModelRenderer> modelRender);

	/*角度をベクトルに変換(Degree角度)(Yは変更なし)*/
	virtual Vector3 AngleToVectorY(float angle)const;

	/*ベクトルを角度に変更(正規化済みのベクトル)(Yは変更なし)*/
	virtual float VectorToAngleY(const Vector3& vector) const;

	/*対象との距離を計り、範囲内ならtrue(対象の位置,指定したい距離)*/
	virtual bool InsideDistance(const Vector3& otherPos, float dist);

	/*扇の当たり判定*/
	virtual bool IsHitFanToPoint(const FanInfomation& fan, const Vector3& point, float radius = 0.0f) const;

private:
	/*移動(移動先の位置)*/
	void Move(const Vector3& otherPosition);

	/*扇の情報変更*/
	void SetFanInfo(float range = 60.0f, float length = 30.0f);

	/*プレイヤー検索(objManager)*/
	void SearchPlayer(ObjectManager* objManager);

	/*パンくず検索(objManager)*/
	void SearchBreadCrumb(BaseObject* breadcrumb);

	/*オブジェクト追跡に切り替え*/
	void StartTracking();

	/*パンくず追跡*/
	void TrackingBreadcrumb();

	/*振り向き機能準備(objManager)*/
	void DicideTurnAround(ObjectManager* objManager);

	/*振り返り機能(振り向くのにかかる時間)*/
	void TurnAround(int time);

	/*向き変更(使ってない)*/
	void ChangeDirection();


protected:

	//key = 識別番号　：　value = 位置
	std::unordered_map<int, Vector3> breadMap;//パンくずリスト

	int HP;        //体力
	int mapCount = 0;  //マップの要素数
	int breadCount = 0;//落としたパンくずの総数
	int number = 0;//識別番号

	int warningTime;     //警戒時間
	int attackTime;	     //攻撃時間
	int attackCount = 0;       //攻撃カウント
	int warningCount = 0;      //警戒カウント
	int invincibleCount = 0;   //無敵カウント
	int destructCount = 0;     //自爆カウント
	int turnCount = 0;         //振り向きカウント
	int currentPointNumber = 0;//現在の巡回番号

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
	bool isDestruct;        //自爆状態になっているかどうか。
	bool breadcrumbMode;    //パンくず追跡を行うかどうか
	bool destructMode;      //瀕死時に自爆するかどうか
	bool turnaroundMode;    //攻撃に当たった時にゆっくり振り向くかどうか

	Vector3 scale;       //大きさ
	Vector3 lastBreadPos;//パンくずの最後の位置を保存用
	Vector3 previousPos; //前フレームの位置(角度変更用)
	Vector3 hitPos = Vector3().zero;
	Vector3 hitAngle = Vector3().zero;

	string tankBarrel;//砲身の名前登録
	string tankTurret;//砲塔の名前登録
	string tankBody;  //車体の名前登録
	string num;       //string型の識別番号
	string numBarrel; //識別番号+砲身の名前
	string numTurret; //識別番号+砲塔の名前
	string numBody;	  //識別番号+車体の名前

	AttackArea* destructArea;//自爆範囲クラス
};