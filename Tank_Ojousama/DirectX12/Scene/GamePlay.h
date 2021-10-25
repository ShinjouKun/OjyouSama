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

class SniperEnemy;
class BlowEnemy;
class WayPointManager;
class BreadCrumbCreater;
class Sound;
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
	float x, y, z;

	ItemHolder* itemHolder;

	//ポーズフラグ
	bool pose = false;
	//設定フラグ
	bool settingFlag = false;
	//リザルトフラグ
	bool resultFlag = false;
	int time = 0;
	Vector3 posePos;
	Vector3 selectbackPos;
	Vector3 selectposition;
	Vector3 camerapos = Vector3(0, 0, 0);
	Vector3 setcamerapos = Vector3(10, 0, 10);
	//BreadCrumbCreater* mBreadCreator;
	//WayPointManager * mpointManager;

	shared_ptr<BreadCrumbCreater>mBreadCreator;
	shared_ptr<WayPointManager> mpointManager;

	shared_ptr<EnemyAI> mEnemyAI;
};
