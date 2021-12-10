#pragma once
#include "BaseEnemy.h"

class MemberEnemy;

//編隊の隊長
class CEnemy : public BaseEnemy
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
	CEnemy(
		const Vector3 & position,
		const Vector3 & angle,
		int num,
		bool advanceFlag = false,
		const Vector3& advanceDirection = Vector3(0.0f, 0.0f, 1.0f)
	);

	~CEnemy();

	//BaseEnemyから継承
	virtual void EnemyInit() override;
	virtual void EnemyUpdate() override;
	virtual void EnemyRend() override;
	virtual void EnemyOnCollision(BaseCollider* col) override;
	virtual void EnemyImGuiDebug() override;

private:

	/*一度しか呼び出さない処理*/
	void OnlyOnceAction();

	/*隊員の移動管理*/
	void MemberMove(int number);

	/*隊員への移動指令*/
	void MoveOrder();

	/*隊員への索敵指令*/
	void SearchOrder();

	/*生存状態を監視*/
	void CheckAlive();

	/*移動*/
	void Move();

	/*歩行アニメーション*/
	void MoveAnimation();

	/*攻撃*/
	void Attack();

	/*死亡時の演出*/
	void DeathAnimation();

	/*空に上昇*/
	void DeathAnimeStep_RiseSky();

	/*爆発&数秒停止*/
	void DeathAnimeStep_Explosion();

private:

	/*死亡アニメーションの遷移状況*/
	enum DeathAnimationStep
	{
		RISE_SKY, //上空に舞い上がる
		EXPLOSION,//消滅する
	};
	DeathAnimationStep mDeathStep;

	/*隊長の索敵指示*/
	enum SearchCommandStep
	{
		USUALLY,            //常時処理
		FIND_PLAYER,        //プレイヤーを見つけている
		SEARCH_ORDER,       //索敵開始を指示する
		RECEIVE_RESULT,     //索敵結果を受け取る
		MOVE_RESULTPOSITION,//報告位置に向かう
	};
	SearchCommandStep searchStep;

	vector<MemberEnemy*> mMemberList;   //隊員リスト
	vector<Vector3> mMemberPositionList;//隊員のための位置リスト

	shared_ptr<Timer> mSearchTimer;//メンバーがプレイヤーを検索する時間
	shared_ptr<Timer> mRiseTime;   //上昇時間
	shared_ptr<Timer> mDeathTime;  //完全に死ぬまでの時間
	shared_ptr<Sound> mAttackSE;   //攻撃時のSE
	shared_ptr<Sound> mDamageSE;   //ダメージを受けた時のSE
	shared_ptr<Sound> mDeathSE;    //死亡したときのSE
	shared_ptr<ParticleEmitterBox> mParticleEmitter;//爆発のエフェクト

	Vector3 mSearchPosition;//索敵結果位置

	const float LEG_SPEED = 1.5f; //脚の回転速度
	const float LEG_RANGE = 15.0f;//脚の最大回転量
	const float SEARCH_INTERVAL = 1.0f;//索敵指示を行う間隔
	const float MEMBER_DISTANCE = 25.0f;//メンバーとの距離

	const int MEMBER_COUNT = 5;//隊員の数

	float mLegRotate = 0.0f;//脚の現在の回転量

	int bulletNumber = 0;//弾の識別番号
	int mEnemyNumber;    //モデル描画用の番号

	bool mDeathAnimation; //死亡アニメーションを開始するか
	bool mDeadFlag;       //死ぬ瞬間にtrueになるフラグ(保険)
	bool mOnlyOnceTrigger;//一回しか呼びたくない処理用
	bool mAnyDeathFlag;   //リスト内の誰かが死んだか
	bool mSearchCommand;  //索敵指令
	bool mSearchResult;   //索敵の結果を受け取る
	bool mRotDirection;   //回転方向

	string mLeftLeg;        //左足のモデル名
	string mLLegNumber;     //左足 + 識別番号
	string mRightLeg;       //右足のモデル名
	string mRLegNumber;     //右足 + 識別番号
	string mHead;           //頭の名前
	string mHeadNumber;     //頭 + 識別番号
	string mBody;           //体の名前
	string mBodyNumber;     //体 + 識別番号
	string EXPLOSION_EFFECT;//エフェクト名
};