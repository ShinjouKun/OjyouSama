#pragma once
#include<memory>
#include"BaseScene.h"
#include "../Device/Input.h"
#include "../Actor/Player.h"
#include "../Actor/ObjectManager.h"
#include "../Render/Camera.h"
#include "../Render/ModelRenderer.h"
#include "../Render/TexRenderer.h"
#include "../Render/ParticleManager.h"
#include "../Items/ItemHolder.h"
#include "../Items/Repair.h"
#include "../Actor/Enemy/EnemyAI.h"

class WayPointManager;
class BreadCrumbCreater;
class Sound;
class Timer;
class ElfTreeBoss;

class BossScene :public BaseScene
{
public:
	BossScene();
	~BossScene();

private:
	virtual void StartScene() override;

	virtual void UpdateScene() override;

	virtual void DrawScene() override;

	virtual void FinalizeScene() override;

	/*敵を生成する*/
	void EnemySpawn();

	/*アニメーションが終了しているかを監視*/
	void CheckAnimation();

	//ポーズ
	void Pose();
	//音量設定
	void Setting();
	//リザルト画面
	void ResultF();

private:

	ObjectManager* mObjManager;//オブジェクトマネージャー
	Player* mPlayer;           //プレイヤーの実体生成
	Camera* mCamera;           //カメラ生成
	ElfTreeBoss * mBoss;       //ボス生成
	ItemHolder* itemHolder;    //アイテムホルダー
	shared_ptr<Sound> mBGM;    //BGM生成
	shared_ptr<BreadCrumbCreater>mBreadCreator;//パンくず生成
	shared_ptr<WayPointManager> mpointManager; //使ってなーい
	shared_ptr<EnemyAI> mEnemyAI;              //使ってなーい
	shared_ptr<ParticleEmitterBox>ParticleBox; //使ってなーい
	shared_ptr<Timer> mTimer;                  //入力用タイマー

	Vector3 camerapos = Vector3(0, 0, 0);
	Vector3 setcamerapos = Vector3(10, 0, 10);
	Vector3 posePos;
	Vector3 selectbackPos;
	Vector3 selectposition;
	Vector3 optionPos;
	
	bool pose = false;       //ポーズフラグ	
	bool settingFlag = false;//設定フラグ	
	bool resultFlag = false; //リザルトフラグ
	bool OpFlag1 = false;    //ボリューム用フラグ01
	bool OpFlag2 = false;    //ボリューム用フラグ02
	bool OpFlag3 = false;    //ボリューム用フラグ03
	bool mBossDeadFlag;      //ボスが死んだかどうか
	bool mCameraAnimation;   //カメラアニメーション中か？
	bool mSpawn;             //敵を生成したかどうか

	float timer = 0;
	float x, y, z;

	int time = 0;
};