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

private:

	Camera* mCamera;           //カメラ
	ItemHolder* mItemHolder;   //アイテムホルダー
	ObjectManager* mObjManager;//オブジェクトマネージャー

	std::shared_ptr<Sound> mBGM;  //BGM再生
	std::shared_ptr<Sound> mSE;	  //※ポーズ開いた時
	std::shared_ptr<Timer> mTimer;//ポーズ用タイマー
	std::shared_ptr<BreadCrumbCreater>mBreadCreator;//パンくず生成
	//std::shared_ptr<WayPointManager> mpointManager; //WayPoint生成
	//std::shared_ptr<EnemyAI> mEnemyAI;              //敵AI生成

	Vector3 posePos;
	Vector3 selectbackPos;
	Vector3 selectposition;
	Vector3 camerapos = Vector3(0, 0, 0);
	Vector3 setcamerapos = Vector3(10, 0, 10);
	Vector3 optionPos;

	int objectCount;//オブジェクト数
	int mTime = 0;

	//ゲーム進行管理用
	bool mTreasureGet = false;   //目標を入力しているか？
	bool mGoalFlag = false;      //ゴールにいるか？
	float mGoalLine;             //ゴールのZ地点
	bool mStartSpownFlag = false;//最初から生成されている敵
	bool mBackSpownFlag = false; //宝箱を取ったら出現する敵
	bool mPose = false;          //ポーズフラグ
	bool mSettingFlag = false;   //設定フラグ
	bool mResultFlag = false;    //リザルトフラグ
	bool OpFlag1;                //ボリューム用フラグ01
	bool OpFlag2;                //ボリューム用フラグ02
	bool OpFlag3;                //ボリューム用フラグ03
};