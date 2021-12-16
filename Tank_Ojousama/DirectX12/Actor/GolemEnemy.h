#pragma once
#include"../Render/ParticleManager.h"
#include "BaseObject.h"
#include"ObjectManager.h"
#include "../Render/ModelRenderer.h"
#include "../Render/ParticleManager.h"
#include"../Render/TexRenderer.h"

class Explosion;
class Hit;
class Sound;

//エルフの森用の中ボス
//拠点防衛型なのでプレイヤーが範囲外にでたら原点に戻る
struct Canp
{
	Vector3 CanpPoint;//原点
	float CanpRadius;//半径
};
//追跡用センサー
struct SenserPoint_G //点　検知した部分
{
	Vector3 Point;
	float SenserRadius;//半径
};
struct SenserFan_G //扇　視界
{
	Vector3 FanPoint;//位置　基準点
	float FanArea;//範囲
	float FanLength;//長さ
	float FanAngle;//視野角
};
//危険度
enum GolemBatteleStatus
{
	SAFE_G,//安全(待機)
	BATTELE_G,//戦闘
	RETURN_G//帰還（プレイヤーが拠点範囲から外れた時）
};

class Timer;
class GolemEnemy:public BaseObject
{
public:
	GolemEnemy(Vector3 position,
		Vector3 angle,
		ObjectManager * objManager,
		shared_ptr<ModelRenderer>modelRender,
		shared_ptr<TexRenderer>texRender,
		shared_ptr<ParticleManager> effect,
		int num);
	~GolemEnemy();

private:

	/*死亡アニメーションの遷移状況*/
	enum DeathAnimationStep
	{
		RISE_SKY, //上空に舞い上がる
		EXPLOSION,//消滅する
	};
	DeathAnimationStep mDeathStep;
	//センサー関連
	void Senser();//センサー更新
	bool SenserIn(SenserFan_G fan, Vector3 point);//視界内に何か入ったか？
	//行動
	bool CanpOut();//防衛地点から出たか？
	bool Distance(const Vector3 & otherPos, float dist);
	void Safe();
	void Battele();
	void PredecessorMove();//近づく動き
	void ReturnMove();//拠点に戻る動き
	void AvoidMove();//避ける動き
	void ProximityAttack();//近接
	void LangeAttack();//遠距離
	void SpecialAttack();//必殺技
	void DicideTurnAround();//振り向き
	void TurnAround(int time);
	void Guard();
	/*生存状態を監視*/
	void CheckAlive();

	/*死亡時の演出*/
	void DeathAnimation();

	/*空に上昇*/
	void DeathAnimeStep_RiseSky();

	/*爆発&数秒停止*/
	void DeathAnimeStep_Explosion();
	Vector3 GetEnemyVec(const Vector3& vec);//敵のベクトル
	Vector3 GetCampVec(const Vector3& vec);//拠点のベクトル
	// BaseObject を介して継承されました

	virtual void Init() override;

	virtual void Update() override;

	virtual void Rend() override;

	virtual void ImGuiDebug() override;

	virtual void OnCollison(BaseCollider * col) override;
private:
	bool mDeathAnimation;//死亡アニメーションを開始するか
	bool mDeadFlag;//死ぬ瞬間にtrueになるフラグ(保険)
	ObjectManager* objM;
	shared_ptr<ModelRenderer>Model;
	shared_ptr<ParticleManager>Particle;
	shared_ptr<ParticleEmitterBox>ParticleBox;
	shared_ptr<TexRenderer>Sprite;
	shared_ptr<Hit> mDamageParticle;     //ダメージ用パーティクル
	//shared_ptr<Explosion> mDeathParticle;//死亡用パーティクル
	shared_ptr<Sound> mAttackSE;  //攻撃時のSE
	shared_ptr<Sound> mDamageSE;  //被弾時のSE

	GolemBatteleStatus batteleS;
	SenserPoint_G point;
	Canp canp;
	shared_ptr<Timer> mRiseTime;  //上昇時間
	shared_ptr<Timer> mDeathTime; //完全に死ぬまでの時間
	//無敵時間のため
	bool HitFlag;
	int HitCount;
	//ステータス
	
	bool startFlag;
	float ySpeed;
	int speedTime;
	float maxSpeed;
	float speedLimitTime;
	Vector3 ArmPosR;
	Vector3 ArmPosL;
	float targetAngleX;
	//int HP;
	int ProximityPoint;//近接攻撃力
	int LangePoint;//遠距離攻撃力
	int SpecialPoint;//必殺技など
	bool AttackFlag;//共通の攻撃用フラグ
	int AttackCount;
	int attackMoveCount;//攻撃が終わるまでのカウント（近遠）
	Vector3 angleVec;//移動する向き
	//複数だす用
	int number = 0;
	string Body;//キーワード登録
	string ArmR;
	string ArmL;
	string num;//複数用ナンバー
	string numNameBody;//上二つの複合体
	string numNameArmR;
	string numNameArmL;//上二つの複合体
	int bulletStock;//弾をいっぱいだすため
	int Count;
	Vector3 bodyAngle;//Y軸//描画
	float ArmAngleR;//腕
	float zR;
	float ArmAngleL;//腕
	float zL;
	bool moveFlag;
	bool avmoveFlag;
	int avmoveCount;//ランダム移動カウント
	int moveCount;//戦闘時の接近回数制限用カウント
	int rndCount;
	bool stoneShotFlag;
	bool guardFlag;
	Vector3 hitPos;      //攻撃が当たった位置を保存
	Vector3 hitAngle;    //攻撃が当たった角度を保存
};
