#pragma once
#include"ObjectManager.h"
#include"BaseObject.h"
#include<memory>
#include "../Render/Camera.h"
#include "../Render/TexRenderer.h"
#include "../Render/ModelRenderer.h"
#include "../Render/ParticleManager.h"
#include "../Collision/CollisonManager.h"
#include"../Utility/ModelChanger.h"
#include"../Items/ItemHolder.h"


class Listener;
class Sound;
class Timer;
class NormalAttackParticle;
class MachineGunAttackParticle;
class TankTrajectory;
class Hit;

using namespace std;
class Player :public BaseObject
{
public:

	Player(Vector3 pos, Vector3 angle, ObjectManager* obj, shared_ptr<ModelRenderer>m, shared_ptr<ParticleManager>p, shared_ptr<TexRenderer>s, int sceneEffect);
	~Player();
	void StartCamScene();
	void SceneCamMove1();//殲滅
	void SceneCamMove2();//強奪
	void SceneCamMove3();//防衛
	void SceneCamMove4();//ボス

	void UseWeapon1();//兵装１
	void UseWeapon2();//兵装２
	void UseULT();//アルティメット
	void Item();//サポートアイテム
	void AngleReset();//数値のオーバーフロウを防ぎたい
	// BaseObject を介して継承されました
	virtual void Init() override;
	virtual void Update() override;
	virtual void Rend() override;
	void SetHP(int value);//体力変更
	int GetHP() { return HP; }

private:
	void TrajectoryPlay();

private:
	ObjectManager* objM;
	shared_ptr<ModelRenderer>playerModel;
	shared_ptr<ParticleManager>playerParticle;
	shared_ptr<ParticleEmitterBox>playerParticleBox;
	shared_ptr<TexRenderer>playerSprite;
	shared_ptr<Listener> listener;
	Camera* camera;
	std::shared_ptr<Sound> mNormalAttackSE;
	std::shared_ptr<Sound> mDamageSE01;
	std::shared_ptr<Sound> mDamageSE02;
	std::shared_ptr<Sound> mDeathSE;
	std::shared_ptr<Timer> mTimer;
	ModelChanger* modelChanger;

	//カメラ用
	Vector3 CameraPos;
	float CamPos_Y;
	Vector3 TargetPos;
	Vector3 CamVelocity;//カメラの移動量
	//制御用フラグ

	bool sniperShotFlag = false;
	bool shotFlag1 = false;
	bool shotFlag2 = false;
	bool masingunShot = false;
	int shotcnt1 = 0;
	int shotcnt2 = 0;
	bool moveFlag;
	bool FrontMove;
	bool BackMove;
	Vector3 AIMPos;
	//ステータス
	int weaponMoveNum;
	int UpDamage;//追加攻撃力
	ItemHolder* item;
	bool GameOver = false;
	int bulletStock;//弾をいっぱいだすため
	float atkAngle;//砲塔用//描画
	float fireAngle;//砲身
	float cameraSpeed;
	int speedTime;
	int maxHP;//最高HP
	float maxSpeed;//最高速度
	float speedLimitTime;
	bool HitFlag;//無敵時間のため
	int HitCount;
	float ojyouY;
	float ojyouZ;
	float ojyouXR;
	float ojyouZR;
	float ojyouXL;
	float ojyouZL;
	float aimPos_Y;
	int sceneEffectNum;
	int sceneCount;
	bool sceneCamOk;
	Vector3 sceneCamPos;
	bool sceneCamPlayerOk;
	float damageFade;
	float damageFadeYpos;
	//サウンド

	// BaseObject を介して継承されました
	virtual void OnCollison(BaseCollider* col) override;

	// BaseObject を介して継承されました
	virtual void ImGuiDebug() override;

	//パーティクル
	std::shared_ptr<NormalAttackParticle> mNormalAtkParticle;//Weapon1
	std::shared_ptr<MachineGunAttackParticle> mMGAParticle;//Weapon2
	std::shared_ptr<TankTrajectory> mTankTraL;//軌跡
	std::shared_ptr<TankTrajectory> mTankTraR;//軌跡
	std::shared_ptr<Hit> mHit;//小爆発
};