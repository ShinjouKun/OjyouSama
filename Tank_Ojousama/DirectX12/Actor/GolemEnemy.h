#pragma once
#include"../Render/ParticleManager.h"
#include "BaseObject.h"
#include"ObjectManager.h"
#include "../Render/ModelRenderer.h"
#include "../Render/ParticleManager.h"
#include"../Render/TexRenderer.h"
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
	Vector3 GetEnemyVec(const Vector3& vec);//敵のベクトル
	Vector3 GetCampVec(const Vector3& vec);//拠点のベクトル
	// BaseObject を介して継承されました

	virtual void Init() override;

	virtual void Update() override;

	virtual void Rend() override;

	virtual void ImGuiDebug() override;

	virtual void OnCollison(BaseCollider * col) override;
private:
	ObjectManager* objM;
	shared_ptr<ModelRenderer>Model;
	shared_ptr<ParticleManager>Particle;
	shared_ptr<ParticleEmitterBox>ParticleBox;
	shared_ptr<TexRenderer>Sprite;

	GolemBatteleStatus batteleS;
	SenserPoint_G point;
	Canp canp;

	//無敵時間のため
	bool HitFlag;
	int HitCount;
	//ステータス
	float ySpeed;
	int speedTime;
	float maxSpeed;
	float speedLimitTime;
	Vector3 ArmPosR;
	Vector3 ArmPosL;
	int HP;
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
};
