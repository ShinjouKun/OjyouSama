#pragma once
#include<memory>
#include"BaseScene.h"
#include "../Device/Input.h"
#include "../Actor/Player.h"
#include "../Actor/EnemyTank.h"
#include "../Actor/Block.h"
#include "../Actor/ObjectManager.h"
#include "../Render/Camera.h"
#include "../Render/ModelRenderer.h"
#include "../Render/TexRenderer.h"
#include "../Render/ParticleManager.h"
#include"../Actor/GolemEnemy.h"
#include "../Items/ItemHolder.h"
#include "../Items/Repair.h"

#include "../Actor/Enemy/EnemyAI.h"

class Sound;
class Timer;
class WayPointManager;
class BreadCrumbCreater;

class Robbery :public BaseScene
{
public:
	Robbery();
	~Robbery();

private:
	void StartWayEnemySpown();//行き道
	void BackWayEnemySpown();//帰り道(帰りは複数で激し目でもいい)


	// BaseScene を介して継承されました
	virtual void StartScene() override;

	virtual void UpdateScene() override;

	virtual void DrawScene() override;

	//ポーズ
	void Pose();
	//音量設定
	void Setting();
	//リザルト画面
	void ResultF();

	Camera* camera;
	ObjectManager* objM;
	std::shared_ptr<Sound> mSound;
	std::shared_ptr<Sound> mSE;
	std::shared_ptr<Timer> mTimer;
	//ゲーム進行管理用
	bool treasureGet = false;//目標を入力しているか？
	bool goalFlag = false;//ゴールにいるか？
	float goalLine;//ゴールのZ地点
	bool startSpownFlag = false;
	bool backSpownFlag = false;
	//ポーズフラグ
	bool pose = false;
	//設定フラグ
	bool settingFlag = false;
	//リザルトフラグ
	bool resultFlag = false;
	int time = 0;
	//ボリューム用フラグ
	bool OpFlag1;
	bool OpFlag2;
	bool OpFlag3;
	float timer = 0;
	Vector3 posePos;
	Vector3 selectbackPos;
	Vector3 selectposition;
	Vector3 camerapos = Vector3(0, 0, 0);
	Vector3 setcamerapos = Vector3(10, 0, 10);
	Vector3 optionPos;

	ItemHolder* itemHolder;

	shared_ptr<BreadCrumbCreater>mBreadCreator;
	shared_ptr<WayPointManager> mpointManager;
	shared_ptr<EnemyAI> mEnemyAI;
	std::shared_ptr<ParticleEmitterBox>ParticleBox;

};