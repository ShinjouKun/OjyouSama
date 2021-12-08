#pragma once
#include <memory>
#include "../BaseObject.h"              //オブジェクトのひな型
#include "../ObjectManager.h"           //オブジェクト管理者
#include "../../Render/ModelRenderer.h" //モデル貼り付け
#include "../../Render/TexRenderer.h"	//ポリゴンの描画
#include "../../Render/ParticleManager.h"//パーティクル描画
#include "AttackArea.h"//子クラスで作ってるからここに書いてる

#include "EnemyAI.h"

class TestWayPoint;
class WayPointManager;
class ReportArea;
class BreadCrumbCreater;
class TestBreadCrumb;
class Timer;

/// <summary>
/// 敵の基底クラス
/// </summary>
class BaseEnemy : public BaseObject
{
protected:

	//扇の索敵範囲
	struct FanInfomation
	{
		Vector3 position;
		float fanRange;//角度
		float length;  //長さ
		float rotate;  //回転角度
	}fanInfo;

	//敵の行動状態
	enum MoveState
	{
		SEARCH,  //索敵
		WARNING, //警戒
		ATTACK,  //攻撃
		DESTRUCT,//自爆
		NOT_FIND,            //未発見状態
		CHASE_PLAYER,        //プレイヤー追跡中
		CHASE_BREADCRUMB,    //パンくず追跡中
		CHASE_ATTACKTARGET,  //攻撃拠点追跡中
		CHASE_TURRET,        //タレット追跡中
		ADVANCE_BORDWERLINE, //ボーダーラインまで直線移動中
		ADVANCE_ATTACKTARGET,//攻撃拠点まで移動中
		TURN_AROUND,//回転中
		REPORT,//報告中

	}mMoveState;

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

	virtual void Search() = 0; //未発見状態
	virtual void Warning() = 0;//発見状態
	virtual void Attack() = 0; //攻撃中

	/*行動状態の変更*/
	void ChangeState();

	/*オブジェクト検索まとめ()*/
	virtual void SearchObject();

	/*移動(移動先の位置)*/
	void MovePoint(const Vector3& otherPosition);

	/*Y軸を無視した移動(移動先の位置)*/
	void MovePointY(const Vector3& otherPosition);

	/*移動(移動方向)*/
	void MoveDirection(const Vector3& direction);

	/*無敵時間(ダメージを受けない時間)*/
	virtual void Invincible(int time);

	/*スモークによる盲目状態*/
	virtual void SmokeBlind();

	/*首振り索敵機能(左右何度まで回転するか)*/
	virtual void SwingDirection(float range);

	/*配列内のポイントを順番に巡回する(位置の配列,配列の要素数)*/
	virtual void PatrolPoint(const Vector3 points[],int pointCount);

	/*プレイヤー or パンくず を追いかける*/
	virtual void TrackingObject();

	/*瀕死時に自爆状態に変更する(自爆状態になる体力,自爆するかどうか)*/
	virtual void DestructMode(int hpLine, bool destructMode);

	/*自爆行動(objManager, modelRender)(当たり判定オブジェクトをBase側で生成する)*/
	virtual void DestructAction(shared_ptr<ModelRenderer> modelRender);

	/*WayPoint追跡開始(報告主の位置)*/
	virtual void InitSearch();

	///*WayPoint追跡を実行(必ずInitSearchが行われていないといけない)*/
	//virtual void SerachWayPoint();

	/*WayPoint変数の初期化*/
	virtual void InitWayPoint();

	/*WayPointに向かって移動開始*/
	virtual void WayPointMove();

	///*報告範囲を表示する*/
	//virtual void Report(shared_ptr<ModelRenderer> modelRender);

	/*角度をベクトルに変換(Degree角度)(Yは変更なし)*/
	virtual Vector3 AngleToVectorY(float angle)const;

	/*ベクトルを角度に変更(正規化済みのベクトル)(Yは変更なし)*/
	virtual float VectorToAngleY(const Vector3& vector) const;

	/*対象との距離を計り、範囲内ならtrue(対象の位置,指定したい距離)*/
	virtual bool InsideDistance(const Vector3& otherPos, float dist);

	/*扇の当たり判定*/
	virtual bool IsHitFanToPoint(const FanInfomation& fan, const Vector3& point, const float radius = 0.0f) const;

	/*パンくず生成器取得------------------------------------------いらない-------------------------------------*/
	static void SetBreadCreator(BreadCrumbCreater * breadCreator);

	/*体力取得*/
	int GetHP()const;

	/*死亡フラグ取得*/
	bool GetDeathFlag() const;

	/*敵AIをセットする*/
	static void SetEnemyAi(EnemyAI* enemyAI);

	/*オブジェクトマネージャーセット*/
	static void SetObjectManager(ObjectManager* manager);

	/*拠点進攻モードの攻撃対象の位置をセット*/
	static void SetAttackTarget(const Vector3& attackTarget = Vector3(0.0f, 0.0f, 0.0f));

	/*オブジェクト生成に必要なデータをセット(ObjectManager, ModelRenderer,ParticleManager)*/
	static void SetImportantObject(ObjectManager * manager, shared_ptr<ModelRenderer> modelRender, shared_ptr<ParticleManager> particleManager, shared_ptr<BreadCrumbCreater> breadCreator);


private:

	/*変数の初期化*/
	void Initialize();

	/*生存状態の監視*/
	void AliveSurveillance();

	/*扇の情報変更*/
	void SetFanInfo(float range = 60.0f, float length = 30.0f);

	/*プレイヤー検索*/
	void SearchPlayer();

	/*攻撃拠点検索*/
	void SearchAttackTarget();

	/*タレット検索*/
	void SearchTurret();

	/*パンくず検索*/
	void SearchBreadCrumb(const TestBreadCrumb& breadCrumb);

	/*オブジェクト追跡に切り替え*/
	void StartTracking();

	/*パンくず追跡*/
	void TrackingBreadcrumb();

	/*振り向き機能準備*/
	void DicideTurnAround();

	/*振り返り機能(振り向くのにかかる時間)*/
	void TurnAround(int time);

	///*シーン上のWayPointを取得し、リストに格納*/
	//void GetAllWayPoint();

	///*指定位置から最も近いポイントを取得(指定位置)*/
	//shared_ptr<TestWayPoint> NearWayPointStartTest(const Vector3 & point) const;

	///*自身から最も近いポイントn個を配列に格納(格納する個数)*/
	//void SearchPointToArrayTest(int length);

	///*配列内で最も対象に近いWayPointを取得(指定配列,対象位置)*/
	//shared_ptr<TestWayPoint> NearWayPointArrayTest(const vector<shared_ptr<TestWayPoint>>& trans, const Vector3 & goal);

	///*検索済みと移動不可能フラグを初期化*/
	//void ClearFlag();

protected:

	int mOriginNumber;//識別番号
	int warningTime;  //警戒時間
	int attackTime;	  //攻撃時間
	int attackCount;  //攻撃カウント

	float mRadius;         //当たり判定の半径
	float mFanRotateOrigin;//扇の最初の向き
	float mSwingRange;     //首振り角度の範囲
	float mFireAngle;     //攻撃する向き
	float mAttackLength;   //攻撃範囲
	
	bool mTrackingPlayer;    //プレイヤーを追跡中か
	bool mTrackingBreadcrumb;//パンくずを拾っているか
	bool mTrackingAttackArea;//拠点進攻中か
	bool mTrackingTurret;    //タレット追跡中か
	bool breadcrumbMode;     //パンくず追跡を行うかどうか
	bool DESTRUCT_MODE;      //瀕死時に自爆するかどうか
	bool TURNAROUND_MODE;    //攻撃に当たった時にゆっくり振り向くかどうか
	bool moveWayPoint;       //WayPoint移動中か
	bool RECEIVEREPORT_MODE; //報告を受け取るかどうか
	bool moveFlag;           //移動しているかどうか
	bool mAdvanceFlag = false;       //拠点進攻モードか？
	bool mHitBorderLine = false;     //拠点進攻モードのターゲット追跡開始
	bool mAttackFlag = false;//攻撃信号フラグ

	bool mHitSmokeFlag = false;//スモークに触れたか

	Vector3 scale;       //大きさ
	Vector3 mPlayerPosition;//プレイヤーの位置
	Vector3 mPreviousPosition;//前フレームの位置
	Vector3 mAdvanceDirection;//拠点進攻時の進行方向

	//key = 識別番号　：　value = 位置
	std::unordered_map<int, Vector3> breadMap;//パンくずリスト

	string mMyNumber;       //string型の識別番号

	static ObjectManager* mManager;
	static std::shared_ptr<ModelRenderer> mRend;
	static std::shared_ptr<ParticleManager> mPart;

	//static std::shared_ptr<ParticleManager> mEffect;

	static Vector3 mAttackTarget;//拠点進攻モードのターゲット

private:
	int warningCount;   //警戒カウント
	int invincibleCount;//無敵カウント
	int destructCount;  //自爆カウント
	int breadCount;     //落としたパンくずの総数
	int patrolCount;    //現在の巡回番号
	int searchCount;    //使用するルートの総数
	int loopCount;      //ルートの総検索回数
	int moveCount;      //向かうべきオブジェクトの番号
	int arrayCount;     //配列の要素数

	bool isInvincible;   //無敵時間か
	bool mHitSensor;      //センサーが当たったか？
	bool swingSensor;    //首振りの状態(true=左、false=右)
	bool isDestruct;     //自爆状態になっているかどうか。
	bool finishSearchWay;//ルート検索が完了したか
	bool hitReportArea;  //報告範囲に当たったか
	//bool moveWayPoint;   //WayPoint移動中か
	bool goalFlag;       //目的地に到着したか
	bool isActive;       //行動状態かどうか

	Vector3 lastBreadPos;//パンくずの最後の位置を保存用
	Vector3 previousPos; //前フレームの位置(角度変更用)
	Vector3 hitPos;      //攻撃が当たった位置を保存
	Vector3 hitAngle;    //攻撃が当たった角度を保存
	Vector3 goalPoint;   //報告を出したオブジェクトの位置
	Vector3 resultPoint; //向かうべきWayPointの位置

	static std::shared_ptr<BreadCrumbCreater> mBreadCreator;    //パンくず作成クラス
	AttackArea* destructArea;                   //自爆範囲クラス
	std::vector<Vector3> moveList;              //実際に移動する位置を管理
	int mIntervalCount = 0;
	bool mDeathFlag = false;

	std::shared_ptr<Timer> mSmokeTimer;//目が見えない時間

	static EnemyAI * mEnemyAI;
};