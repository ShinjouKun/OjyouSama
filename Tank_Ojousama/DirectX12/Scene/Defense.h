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

#include "../Actor/EnemyAI.h"

class Sound; 
class Timer;
class WayPointManager;
class BreadCrumbCreater;

class Defense :public BaseScene
{
public:
	Defense();
	~Defense();

private:

	void Wave1EnemySpown();
	void Wave2EnemySpown();
	void Wave3EnemySpown();
	void Wave1();
	void Wave2();
	void Wave3();
	
	// BaseScene を介して継承されました
	virtual void StartScene() override;

	virtual void UpdateScene() override;

	virtual void DrawScene() override;
	//ポーズ
	void Pose();
	//音量設定
	void Setting();

	ObjectManager* objM;
	Player* player;//プレイヤーの実体生成
	EnemyTank* enemyTank;
	Camera* camera;
	std::shared_ptr<Sound> mSound;
	std::shared_ptr<Sound> mSE;
	//ウェーブ制御用
	int interval;//次のウェーブまでの待機カウント
	bool spown1;//エネミー出す用のフラグ
	bool spown2;//エネミー出す用のフラグ
	bool spown3;//エネミー出す用のフラグ
	int enemyDeath;
	int wave1EnemysCount;
	int wave2EnemysCount;
	int wave3EnemysCount;
	bool wave1Clear;
	bool wave2Clear;
	bool wave3Clear;
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
	Vector3 posePos;
	Vector3 selectbackPos;
	Vector3 selectposition;
	Vector3 camerapos = Vector3(0, 0, 0);
	Vector3 setcamerapos = Vector3(10, 0, 10);
	Vector3 optionPos;

	ItemHolder* itemHolder;

	shared_ptr<BreadCrumbCreater>mBreadCreator;
	shared_ptr<WayPointManager> mpointManager;

	std::shared_ptr<Timer> mTimer;
	shared_ptr<EnemyAI> mEnemyAI;
};