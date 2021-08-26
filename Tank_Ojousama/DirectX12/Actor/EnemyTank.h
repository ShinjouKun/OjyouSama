#pragma once
#include "BaseObject.h"
#include"ObjectManager.h"
#include<memory>
#include "../Render/ModelRenderer.h"
#include "../Render/ParticleManager.h"
#include"../Render/TexRenderer.h"

//追跡用センサー
struct SenserPoint //点　検知した部分
{
	Vector3 Point;
	float SenserRadius;//半径
};
struct SenserFan //扇　視界
{
	Vector3 FanPoint;//位置　基準点
	float FanArea;//範囲
	float FanLength;//長さ
	float FanAngle;//視野角
};
//危険度
enum BatteleStatus
{
	SAFE,//安全
	WARNING,//警戒
	DANGER,//危険
	BATTELE//戦闘
};

class EnemyTank :public BaseObject
{
public:
	EnemyTank(Vector3 pos, Vector3 ang, ObjectManager * obj, shared_ptr<ModelRenderer> m, shared_ptr<ParticleManager>p, shared_ptr<TexRenderer>s, int num);
	~EnemyTank();
	void Shot();
	void Move();
private:


	// BaseObject を介して継承されました
	virtual void Init() override;

	virtual void Update() override;

	virtual void Rend() override;

	void Senser();//センサー更新
	void Attack();
	void Escape();
	//ステータス別（いらないかも）
	void Safe();
	void Warning();
	void Danger();
	void StatusChack();
	bool SenserIn(SenserFan fan, Vector3 point);//視界内に何か入ったか？

	Vector3 GetEnemyVec(Vector3 vec);//敵のベクトル
	Vector3 GetFriendPos(int id);//味方のIDを獲得しGetPosを実行

	ObjectManager* objM;
	shared_ptr<ModelRenderer>tankModel;
	shared_ptr<ParticleManager>tankParticle;
	shared_ptr<ParticleEmitterBox>tankParticleBox;
	shared_ptr<TexRenderer>tankSprite;
	int HP;
	bool ShotFlag;
	int ShotCount;
	bool HitFlag;//無敵時間のため
	int HitCount;
	int bulletStock;//弾をいっぱいだすため

	Vector3 angleVec;
	//状態
	BatteleStatus batteleS;
	SenserPoint point;

	//複数だす用
	int number = 0;
	string tankAtk;//キーワード登録
	string tankHou;
	string tankBtm;
	string num;//複数用ナンバー
	string numNameAtk;//上二つの複合体
	string numNameHou;
	string numNameBtm;//上二つの複合体
	int Count;
	float atkAngle;//砲塔用//描画
	float fireAngle;//砲身
	bool moveFlag;
	bool FrontMove;
	bool BackMove;

	// BaseObject を介して継承されました
	virtual void OnCollison(BaseCollider* col) override;

	// BaseObject を介して継承されました
	virtual void ImGuiDebug() override;
};