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

#include "../Actor/Enemy/EnemyAI.h"

class SniperEnemy;
class BlowEnemy;
class WayPointManager;
class BreadCrumbCreater;
class Sound;
class Timer;
class GamePlay:public BaseScene
{
public:
	GamePlay();
	~GamePlay();

private:


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

	ObjectManager* objM;
	Player* player;//プレイヤーの実体生成
	EnemyTank* enemyTank;
	Camera* camera;
	std::shared_ptr<Sound> mSound;
	std::shared_ptr<Sound> mHidan;
	float x, y, z;

	ItemHolder* itemHolder;

	//ポーズフラグ
	bool pose = false;
	//設定フラグ
	bool settingFlag = false;
	//リザルトフラグ
	bool resultFlag = false;
	int time = 0;
	float timer = 0;
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
	//BreadCrumbCreater* mBreadCreator;
	//WayPointManager * mpointManager;

	shared_ptr<BreadCrumbCreater>mBreadCreator;
	shared_ptr<WayPointManager> mpointManager;

	shared_ptr<EnemyAI> mEnemyAI;
	std::shared_ptr<Timer> mTimer;

	std::shared_ptr<Timer> mParticleTimer;
	std::shared_ptr<ParticleEmitterBox>ParticleBox;

	Camera* cam;
	int goleamDeath;
	bool spawnFlag;//一番目
	bool spawnFlag2;//二番
	bool spawnFlag3;//三番
};
