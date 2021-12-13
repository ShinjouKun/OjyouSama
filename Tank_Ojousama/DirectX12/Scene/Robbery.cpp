#include <cstdlib>
#include "Robbery.h"
#include "GamePlay.h"
#include "Title.h"
#include "Select.h"
#include "Result.h"
#include "../Sound/Sound.h"
#include "../Utility/Timer/Timer.h"
#include "../Actor/Enemy/BlowEnemy.h"
#include "../Actor/Enemy/SniperEnemy.h"
#include "../Actor/Enemy/CEnemy.h"
#include "../Actor/Enemy/MortarEnemy.h"
#include "../Actor/Enemy/BirdEnemy.h"
#include "../Actor/CameraEye.h"
#include "../Actor/WayPointManager.h"
#include "../Actor/BreadCrumbCreater.h"
#include "../Actor/ElfTree.h"
#include "../Actor/ElfRock.h"
#include "../Actor/Treasure.h"
#include "../Actor/BetaTestBlock.h"
#include "../Actor/Castle.h"

Robbery::Robbery()
	:mBGM(nullptr)
{
}

Robbery::~Robbery()
{
	delete mObjManager;//重要
}

void Robbery::StartWayEnemySpown()
{
	//最初の敵がすでに召喚されていたら処理しない
	if (mStartSpownFlag) return;

#pragma region 入口付近

	//岩投げの敵
	mObjManager->Add(new MortarEnemy(Vector3(-30.0f, 0.0f, 270.0f), Vector3(0.0f, 180.0f, 0.0f), objectCount++));
	mObjManager->Add(new MortarEnemy(Vector3(50.0f,  0.0f, 300.0f), Vector3(0.0f, 180.0f, 0.0f), objectCount++));

	//遠距離の敵
	mObjManager->Add(new SniperEnemy(Vector3(+40.0f, 0.0f, 350.0f), Vector3(0, 180, 0), objectCount++));
	mObjManager->Add(new SniperEnemy(Vector3(-40.0f, 0.0f, 350.0f), Vector3(0, 180, 0), objectCount++));

#pragma endregion

#pragma region 中央付近

	//遠距離の敵
	mObjManager->Add(new SniperEnemy(Vector3(+40.0f, 0, 200.0f), Vector3(0, 180, 0), objectCount++));
	mObjManager->Add(new SniperEnemy(Vector3(-40.0f, 0, 200.0f), Vector3(0, 180, 0), objectCount++));

	////近距離の敵
	//mObjManager->Add(new BlowEnemy(Vector3(45.0f, 0, 205.0f), Vector3(0, 180, 0), objectCount++));
	//mObjManager->Add(new BlowEnemy(Vector3(35.0f, 0, 205.0f), Vector3(0, 180, 0), objectCount++));
	//mObjManager->Add(new BlowEnemy(Vector3(-45.0f, 0, 205.0f), Vector3(0, 180, 0), objectCount++));
	//mObjManager->Add(new BlowEnemy(Vector3(-35.0f, 0, 205.0f), Vector3(0, 180, 0), objectCount++));

#pragma endregion

#pragma region 宝箱付近

	//岩投げの敵
	mObjManager->Add(new MortarEnemy(Vector3(+50.0f, 0.0f, 50.0f), Vector3(0.0f, 180.0f, 0.0f), objectCount++));
	//鳥の敵
	mObjManager->Add(new BirdEnemy(Vector3(-50.0f, 0.0f, 50.0f), Vector3(0.0f, 90.0f, 0.0f), objectCount++));

	//隊列の敵
	mObjManager->Add(new CEnemy(Vector3(0.0f, 0.0f, 120.0f), Vector3(0, 180, 0), objectCount++));


	//遠距離の敵
	mObjManager->Add(new SniperEnemy(Vector3(+55.0f, 0, 120.0f), Vector3(0, 180, 0), objectCount++));
	mObjManager->Add(new SniperEnemy(Vector3(+60.0f, 0, 120.0f), Vector3(0, 180, 0), objectCount++));
	mObjManager->Add(new SniperEnemy(Vector3(-55.0f, 0, 120.0f), Vector3(0, 180, 0), objectCount++));
	mObjManager->Add(new SniperEnemy(Vector3(-60.0f, 0, 120.0f), Vector3(0, 180, 0), objectCount++));

#pragma endregion

	mStartSpownFlag = true;
}

void Robbery::BackWayEnemySpown()
{
	//敵がすでに召喚されていたら処理しない
	if (mBackSpownFlag) return;

	//鳥の敵
	mObjManager->Add(new BirdEnemy(Vector3(+45.0f, 0.0f, 350.0f), Vector3(0.0f, 0.0f, 0.0f), objectCount++));
	mObjManager->Add(new BirdEnemy(Vector3(-45.0f, 0.0f, 350.0f), Vector3(0.0f, 0.0f, 0.0f), objectCount++));

	//岩投げの敵
	mObjManager->Add(new MortarEnemy(Vector3(-40.0f, 0.0f, 270.0f), Vector3(0.0f, 0.0f, 0.0f), objectCount++));
	mObjManager->Add(new MortarEnemy(Vector3(40.0f, 0.0f, 300.0f), Vector3(0.0f,  0.0f, 0.0f), objectCount++));

	//遠距離の敵
	mObjManager->Add(new SniperEnemy(Vector3(+40.0f, 0, 190.0f), Vector3(0, 0, 0), objectCount++));
	mObjManager->Add(new SniperEnemy(Vector3(+40.0f, 0, 210.0f), Vector3(0, 0, 0), objectCount++));
	mObjManager->Add(new SniperEnemy(Vector3(-40.0f, 0, 190.0f), Vector3(0, 0, 0), objectCount++));
	mObjManager->Add(new SniperEnemy(Vector3(-40.0f, 0, 210.0f), Vector3(0, 0, 0), objectCount++));


	//遠距離の敵
	mObjManager->Add(new SniperEnemy(Vector3(+50.0f, 0, 120.0f), Vector3(0, 0, 0), objectCount++));
	mObjManager->Add(new SniperEnemy(Vector3(+65.0f, 0, 120.0f), Vector3(0, 0, 0), objectCount++));
	mObjManager->Add(new SniperEnemy(Vector3(-50.0f, 0, 120.0f), Vector3(0, 0, 0), objectCount++));
	mObjManager->Add(new SniperEnemy(Vector3(-65.0f, 0, 120.0f), Vector3(0, 0, 0), objectCount++));
	mObjManager->Add(new SniperEnemy(Vector3(+10.0f, 0, 120.0f), Vector3(0, 0, 0), objectCount++));
	mObjManager->Add(new SniperEnemy(Vector3(-10.0f, 0, 120.0f), Vector3(0, 0, 0), objectCount++));

	mBackSpownFlag = true;
}

void Robbery::StartScene()
{
#pragma region 引数の初期化

	//オブジェクトマネージャー生成
	mObjManager = new ObjectManager();
	mObjManager->Claer();

	//パンくず生成機作成
	mBreadCreator = std::make_shared<BreadCrumbCreater>(mObjManager);

	//BGM設定
	mBGM = std::make_shared<Sound>("BGM/loop_157.mp3", false);
	mBGM->setVol(BaseScene::mMasterSoundVol * BaseScene::mBGMSoundVol);

	posePos = Vector3(0, 0, 0);
	selectbackPos = Vector3(180, 180, 0);
	selectposition = Vector3(180, 180, 0);
	optionPos = Vector3(180, 180, 0);
	mTimer = std::make_shared<Timer>(0.01f);
	mTimer->setTime(0.2f);

	mGoalFlag = false;
	mTreasureGet = false;
	mStartSpownFlag = false;
	mBackSpownFlag = false;

	objectCount = 0;
	mGoalLine = 500.0f;

	////敵にマネージャーセット
	//BaseEnemy::SetObjectManager(mObjManager);
	////敵にパンくずセット
	//BaseEnemy::SetBreadCreator(mBreadCreator.get());

	BaseEnemy::SetImportantObject(mObjManager, BaseScene::mModel, BaseScene::mParticle, mBreadCreator);

	////敵にAIセット
	//BaseEnemy::SetEnemyAi(mEnemyAI.get());

	////WayPoint生成機作成(生成位置,見た目をつけるかどうか)
	//mpointManager = std::make_shared<WayPointManager>(Vector3(100.0f, 0.0f, -100.0f), mObjManager, BaseScene::mModel, false);
	////敵AIシステム生成
	//mEnemyAI = std::make_shared<EnemyAI>(mpointManager);

#pragma endregion

#pragma region テクスチャの読み込み

	BaseScene::mSprite->AddTexture("Pose", "Resouse/pose.png");
	BaseScene::mSprite->AddTexture("AIM", "Resouse/AIM64.png");
	BaseScene::mSprite->AddTexture("AIM2", "Resouse/AIM64.png");
	BaseScene::mSprite->AddTexture("SelectAim", "Resouse/selectcursol.png");
	BaseScene::mSprite->AddTexture("SBack", "Resouse/selectback.png");
	BaseScene::mSprite->AddTexture("OptionP", "Resouse/option.png");
	BaseScene::mSprite->AddTexture("Ritorai", "Resouse/ritorai.png");
	BaseScene::mSprite->AddTexture("VolOp", "Resouse/voloption.png");
	BaseScene::mSprite->AddTexture("VolOpAim1", "Resouse/volAim.png");
	BaseScene::mSprite->AddTexture("VolOpAim2", "Resouse/volAim.png");
	BaseScene::mSprite->AddTexture("VolOpAim3", "Resouse/volAim.png");
	BaseScene::mSprite->AddTexture("AimA1", "Resouse/volAimA.png");
	BaseScene::mSprite->AddTexture("AimA2", "Resouse/volAimA.png");
	BaseScene::mSprite->AddTexture("AimA3", "Resouse/volAimA.png");

#pragma endregion

#pragma region モデルの読み込み

	BaseScene::mModel->AddModel("Sora2", "Resouse/skybox.obj", "Resouse/skybox_A.png");
	BaseScene::mModel->AddModel("Ground2", "Resouse/Plane.obj", "Resouse/sougen.png");
	BaseScene::mModel->AddModel("KabeR", "Resouse/Plane.obj", "Resouse/kabe.png");
	BaseScene::mModel->AddModel("KabeL", "Resouse/Plane.obj", "Resouse/kabe.png");
	BaseScene::mModel->AddModel("KabeR2", "Resouse/Plane.obj", "Resouse/kabe.png");
	BaseScene::mModel->AddModel("KabeL2", "Resouse/Plane.obj", "Resouse/kabe.png");
	BaseScene::mModel->AddModel("KabeR3", "Resouse/Plane.obj", "Resouse/kabe.png");
	BaseScene::mModel->AddModel("KabeL3", "Resouse/Plane.obj", "Resouse/kabe.png");
	BaseScene::mModel->AddModel("KabeR4", "Resouse/Plane.obj", "Resouse/kabe.png");
	BaseScene::mModel->AddModel("KabeL4", "Resouse/Plane.obj", "Resouse/kabe.png");
	BaseScene::mModel->AddModel("KabeF", "Resouse/Plane.obj", "Resouse/kabe.png");
	BaseScene::mModel->AddModel("KabeF2", "Resouse/Plane.obj", "Resouse/kabe.png");

#pragma endregion

#pragma region オブジェクト生成

	//////スタート地点の天井
	////mObjManager->Add(new BetaTestBlock(Vector3(0, 15.0f, 65.0f), Vector3(180, 0, 0), mObjManager, BaseScene::mModel, objectCount, Vector3(-10.0f, 0.0f, -15.0f), Vector3(10.0f, 1.0f, 15.0f)));
	////後ろの壁
	//mObjManager->Add(new BetaTestBlock(Vector3(0, 0.0f, 50.0f), Vector3(90, 0, 0), mObjManager, BaseScene::mModel, objectCount++, Vector3(-150.0f, 0.0f, -2.0f), Vector3(150.0f, 1.0f, 1.0f)));
	////奥
	//mObjManager->Add(new BetaTestBlock(Vector3(0, 0.0f, -250.0f), Vector3(90, 180, 0), mObjManager, BaseScene::mModel, objectCount++, Vector3(-150.0f, 0.0f, -2.0f), Vector3(150.0f, 1.0f, 1.0f)));
	////右
	//mObjManager->Add(new BetaTestBlock(Vector3(100, 0.0f, 0.0f), Vector3(90, -90, 0), mObjManager, BaseScene::mModel, objectCount++, Vector3(-2.0f, 0.0f, -150.0f), Vector3(1.0f, 1.0f, 150.0f)));
	////左
	//mObjManager->Add(new BetaTestBlock(Vector3(-100, 0.0f, 0.0f), Vector3(90, 90, 0), mObjManager, BaseScene::mModel, objectCount++, Vector3(-2.0f, 0.0f, -150.0f), Vector3(1.0f, 1.0f, 150.0f)));


	//横軸 左:-100 ～ 100:右
	//縦軸 手前:50 ～ 550:奥
	//プレイヤー生成位置(0,0,0)

	//プレイヤー位置(0, 0, 510)

	//入口エリア中央の岩
	for (int i = -20; i <= 20; i += 10)
	{
		float x = static_cast<float>(i);
		mObjManager->Add(new ElfRock(Vector3(x, 4.0f, 350.0f), Vector3(0.0f, 0.0f, 0.0f), mObjManager, BaseScene::mModel, objectCount++, 3));
	}

	//入口エリア右側の岩
	for (int i = -60; i <= 0; i += 10)
	{
		float x = static_cast<float>(i);
		mObjManager->Add(new ElfRock(Vector3(x, 4.0f, 300.0f), Vector3(0.0f, 0.0f, 0.0f), mObjManager, BaseScene::mModel, objectCount++, 3));
	}

	//入口エリア左側の岩
	for (int i = 0; i <= 60; i += 10)
	{
		float x = static_cast<float>(i);
		mObjManager->Add(new ElfRock(Vector3(x, 4.0f, 250.0f), Vector3(0.0f, 0.0f, 0.0f), mObjManager, BaseScene::mModel, objectCount++, 3));
	}

	//中央エリア右側の岩
	for (int i = 0; i <= 40; i += 10)
	{
		float z = static_cast<float>(i);
		mObjManager->Add(new ElfRock(Vector3(-40.0f, 4.0f, 100.0f + z), Vector3(0.0f, 0.0f, 0.0f), mObjManager, BaseScene::mModel, objectCount++, 3));
	}

	//中央エリア左側の岩
	for (int i = 0; i <= 40; i += 10)
	{
		float z = static_cast<float>(i);
		mObjManager->Add(new ElfRock(Vector3(+40.0f, 4.0f, 100.0f + z), Vector3(0.0f, 0.0f, 0.0f), mObjManager, BaseScene::mModel, objectCount++, 3));
	}

	//奥エリアの中央のV字の木
	mObjManager->Add(new ElfTree(Vector3(  0, 4.0f,   100), Vector3(0.0f, 90.0f, 0.0f), mObjManager, BaseScene::mModel, objectCount++));
	mObjManager->Add(new ElfTree(Vector3( +3, 4.0f,  95), Vector3(0.0f, 90.0f, 0.0f), mObjManager, BaseScene::mModel, objectCount++));
	mObjManager->Add(new ElfTree(Vector3( -3, 4.0f,  95), Vector3(0.0f, 90.0f, 0.0f), mObjManager, BaseScene::mModel, objectCount++));
	mObjManager->Add(new ElfTree(Vector3( +6, 4.0f,  90), Vector3(0.0f, 90.0f, 0.0f), mObjManager, BaseScene::mModel, objectCount++));
	mObjManager->Add(new ElfTree(Vector3( -6, 4.0f,  90), Vector3(0.0f, 90.0f, 0.0f), mObjManager, BaseScene::mModel, objectCount++));
	mObjManager->Add(new ElfTree(Vector3( +9, 4.0f,  85), Vector3(0.0f, 90.0f, 0.0f), mObjManager, BaseScene::mModel, objectCount++));
	mObjManager->Add(new ElfTree(Vector3( -9, 4.0f,  85), Vector3(0.0f, 90.0f, 0.0f), mObjManager, BaseScene::mModel, objectCount++));
	mObjManager->Add(new ElfTree(Vector3(+12, 4.0f,  80), Vector3(0.0f, 90.0f, 0.0f), mObjManager, BaseScene::mModel, objectCount++));
	mObjManager->Add(new ElfTree(Vector3(-12, 4.0f,  80), Vector3(0.0f, 90.0f, 0.0f), mObjManager, BaseScene::mModel, objectCount++));
	mObjManager->Add(new ElfTree(Vector3(+15, 4.0f,  75), Vector3(0.0f, 90.0f, 0.0f), mObjManager, BaseScene::mModel, objectCount++));
	mObjManager->Add(new ElfTree(Vector3(-15, 4.0f,  75), Vector3(0.0f, 90.0f, 0.0f), mObjManager, BaseScene::mModel, objectCount++));


#pragma endregion



	//とりあえず行きのスポーン
	StartWayEnemySpown();

	//宝箱生成
	mObjManager->Add(new Treasure(Vector3(0.0f, 0, 70.0f), Vector3(0, 180.0f, 0), mObjManager, BaseScene::mModel));

	//城モデル
	mObjManager->Add(new Castle(Vector3(0.0f, -10.0f, 500.0f), Vector3(0, 0, 0), mObjManager, BaseScene::mModel, BaseScene::mSprite, BaseScene::mParticle));


	//プレイヤー生成
	mObjManager->Add(new Player(Vector3(0.0f, 4.0f, 0.0f), Vector3(0, 0, 0), mObjManager, BaseScene::mModel, BaseScene::mParticle, BaseScene::mSprite, 3));
	mObjManager->Add(new CameraEye(Vector3(0, 0, 180), Vector3(0, 0, 0), mObjManager));
}

void Robbery::UpdateScene()
{
	mBGM->playLoop();
	//パンくずを落とす
	mBreadCreator->DropBreadCrumb();
	//mEnemyAI->Update();
	mTimer->update();

	if (!mTimer->isTime()) return;

	Pose();
	Setting();

	//プレイヤーが宝に触れたか？
	if (mObjManager->GetPlayer().GetTreasure())
	{
		mTreasureGet = true;//宝をゲット
	}

	//プレイヤーが宝を持っている & 二回目のスポーンが行われていない
	if (mTreasureGet && !mBackSpownFlag)
	{
		BackWayEnemySpown();//一度だけ生成する
	}

	//宝を持っている & ゴールに到着していたら
	if (mTreasureGet&&mObjManager->GetPlayer().GetPosition().z >= mGoalLine)
	{
		mGoalFlag = true;
	}

	//シーン処理
	if (mGoalFlag)
	{
		//クリア
		BaseScene::mStageFlag2 = true;
		NextScene(std::make_shared<Result>());
	}
}

void Robbery::DrawScene()
{
	DirectXManager::GetInstance()->SetData3D();
	BaseScene::mModel->Draw("Sora2", Vector3(0, 2.0f, 200.0f), Vector3(0, 0, 0), Vector3(50, 50, 50));
	BaseScene::mModel->Draw("Ground2", Vector3(-20.0f, 0.0f, -90.0f), Vector3(0, 0, 0), Vector3(1000, 1000, 1000));

	BaseScene::mModel->Draw("KabeR4", Vector3(-170.0f, 60.0f, -200.0f), Vector3(90.0f, 180.0f, 90.0f), Vector3(100, 80, 50));
	BaseScene::mModel->Draw("KabeL4", Vector3(170.0f, 60.0f, -200.0f), Vector3(90.0f, -180.0f, -90.0f), Vector3(100, 80, 50));
	BaseScene::mModel->Draw("KabeR3", Vector3(-170.0f, 60.0f, 0.0f), Vector3(90.0f, 180.0f, 90.0f), Vector3(100, 80, 50));
	BaseScene::mModel->Draw("KabeL3", Vector3(170.0f, 60.0f, 0.0f), Vector3(90.0f, -180.0f, -90.0f), Vector3(100, 80, 50));
	BaseScene::mModel->Draw("KabeR2", Vector3(-170.0f, 60.0f, 200.0f), Vector3(90.0f, 180.0f, 90.0f), Vector3(100, 80, 50));
	BaseScene::mModel->Draw("KabeL2", Vector3(170.0f, 60.0f, 200.0f), Vector3(90.0f, -180.0f, -90.0f), Vector3(100, 80, 50));
	BaseScene::mModel->Draw("KabeR", Vector3(-170.0f, 60.0f, 400.0f), Vector3(90.0f, 180.0f, 90.0f), Vector3(100, 80, 50));
	BaseScene::mModel->Draw("KabeL", Vector3(170.0f, 60.0f, 400.0f), Vector3(90.0f, -180.0f, -90.0f), Vector3(100, 80, 50));
	BaseScene::mModel->Draw("KabeF", Vector3(100.0f, 60.0f, -200.0f), Vector3(90.0f, -180.0f, 0.0f), Vector3(100, 80, 50));
	BaseScene::mModel->Draw("KabeF2", Vector3(-100.0f, 60.0f, -200.0f), Vector3(90.0f, -180.0f, 0.0f), Vector3(100, 80, 50));

	mObjManager->Draw();
	DirectXManager::GetInstance()->SetData2D();

	if (mPose)
	{
		BaseScene::mSprite->Draw("Pose", posePos, 0.0f, Vector2(0.25f, 0.5f), Vector4(1, 1, 1, 1));
		BaseScene::mSprite->Draw("SBack", selectbackPos, 0.0f, Vector2(0, 0), Vector4(1, 1, 1, 1));
		BaseScene::mSprite->Draw("OptionP", Vector3(820, 180, 0), 0.0f, Vector2(0, 0), Vector4(1, 1, 1, 1));
		BaseScene::mSprite->Draw("Ritorai", Vector3(500, 360, 0), 0.0f, Vector2(0, 0), Vector4(1, 1, 1, 1));
		BaseScene::mSprite->Draw("SelectAim", selectposition, 0.0f, Vector2(0, 0), Vector4(1, 1, 1, 1));
	}
	if (mSettingFlag)
	{
		BaseScene::mSprite->Draw("Pose", posePos, 0.0f, Vector2(0.25f, 0.5f), Vector4(1, 1, 1, 1));
		BaseScene::mSprite->Draw("VolOp", posePos, 0.0f, Vector2(1, 1), Vector4(1, 1, 1, 1));
		BaseScene::mSprite->Draw("VolOpAim1", OpAim1, 0.0f, Vector2(1, 1), Vector4(1, 1, 1, 1));
		BaseScene::mSprite->Draw("VolOpAim2", OpAim2, 0.0f, Vector2(1, 1), Vector4(1, 1, 1, 1));
		BaseScene::mSprite->Draw("VolOpAim3", OpAim3, 0.0f, Vector2(1, 1), Vector4(1, 1, 1, 1));
		if (OpFlag1)
		{
			BaseScene::mSprite->Draw("AimA1", OpAimA1, 0.0f, Vector2(1, 1), Vector4(1, 1, 1, 0.5f));
		}
		else if (OpFlag2)
		{
			BaseScene::mSprite->Draw("AimA2", OpAimA2, 0.0f, Vector2(1, 1), Vector4(1, 1, 1, 0.5f));
		}
		else if (OpFlag3)
		{
			BaseScene::mSprite->Draw("AimA3", OpAimA3, 0.0f, Vector2(1, 1), Vector4(1, 1, 1, 0.5f));
		}
	}
	if (mResultFlag && mTime >= 300)
	{
		BaseScene::mSprite->Draw("Pose", posePos, 0.0f, Vector2(0.25f, 0.5f), Vector4(1, 1, 1, 1));
		BaseScene::mSprite->Draw("SBack", selectbackPos, 0.0f, Vector2(0, 0), Vector4(1, 1, 1, 1));
		BaseScene::mSprite->Draw("Ritorai", Vector3(820, 180, 0), 0.0f, Vector2(0, 0), Vector4(1, 1, 1, 1));
		BaseScene::mSprite->Draw("SelectAim", selectposition, 0.0f, Vector2(0, 0), Vector4(1, 1, 1, 1));
	}
}

void Robbery::Pose()
{
	//ポーズ
	if (mPose == false && mSettingFlag == false)
	{
		mObjManager->Update();


		if (Input::getKeyDown(KeyCode::Enter) || Input::getJoyDown(JoyCode::MenuButton))
		{
			mPose = true;
		}
	}
	else if (mPose)
	{
		if (selectposition.x <= 0)
		{
			selectposition.x = 820;
		}
		if (selectposition.x > 820)
		{
			selectposition.x = 180;
		}
		if (Input::getKeyDown(KeyCode::A) || Input::joyHorizontal() < 0)
		{
			if (selectposition.y == 360)
			{
				return;
			}
			selectposition.x -= 640;
			mTimer->setTime(0.2f);
		}

		if (Input::getKeyDown(KeyCode::D) || Input::joyHorizontal() > 0)
		{
			if (selectposition.y == 360)
			{
				return;
			}
			selectposition.x += 640;
			mTimer->setTime(0.2f);
		}
		if (Input::getKeyDown(KeyCode::S) || Input::joyVertical() < 0)
		{
			selectposition.y = 360;
			selectposition.x = 500;
			mTimer->setTime(0.2f);
		}
		if (Input::getKeyDown(KeyCode::W) || Input::joyVertical() > 0)
		{
			selectposition.y = 180;
			selectposition.x = 180;
			mTimer->setTime(0.2f);
		}
		if (selectposition.x == 180)
		{
			if (Input::getKeyDown(KeyCode::SPACE) || Input::getJoyDown(JoyCode::A))
			{
				NextScene(std::make_shared<Select>());
			}
		}
		if (selectposition.x == 820)
		{
			if (Input::getKeyDown(KeyCode::SPACE) || Input::getJoyDown(JoyCode::A))
			{
				mSettingFlag = true;
				mPose = false;
			}
		}
		if (selectposition.y == 360 && selectposition.x == 500)
		{
			if (Input::getKeyDown(KeyCode::SPACE) || Input::getJoyDown(JoyCode::A))
			{
				NextScene(std::make_shared<GamePlay>());
			}
		}
		if (Input::getKeyDown(KeyCode::Enter) || Input::getJoyDown(JoyCode::MenuButton))
		{
			mPose = false;
		}
	}
}

void Robbery::Setting()
{
	if (mSettingFlag == true)
	{
		if (optionPos.y >= 541)
		{
			optionPos.y = 180;
		}
		if (optionPos.y <= 179)
		{
			optionPos.y = 540;
		}
		if (Input::getKeyDown(KeyCode::S) || Input::joyVertical() < 0)
		{
			optionPos += Vector3(0, 180.0f, 0);
			mTimer->setTime(0.2f);
		}
		if (Input::getKeyDown(KeyCode::S) || Input::joyVertical() > 0)
		{
			optionPos -= Vector3(0, 180.0f, 0);
			mTimer->setTime(0.2f);
		}
		//マスターボリューム
		if (optionPos.y == 180)
		{
			OpFlag1 = true;
			OpFlag2 = false;
			OpFlag3 = false;
			if (Input::getKeyDown(KeyCode::D) || Input::joyHorizontal() > 0)
			{
				BaseScene::mMasterSoundVol += 0.1f;
				OpAim1.x += 48.0f;
				OpAimA1.x += 48.0f;
				if (OpAim1.x > 880 && OpAimA1.x > 880)
				{
					OpAim1.x = 880;
					OpAimA1.x = 880;
				}
				if (BaseScene::mMasterSoundVol >= 1.0f)
				{
					BaseScene::mMasterSoundVol = 1.0f;
				}
				mTimer->setTime(0.2f);
			}
			if (Input::getKeyDown(KeyCode::A) || Input::joyHorizontal() < 0)
			{
				BaseScene::mMasterSoundVol -= 0.1f;
				OpAim1.x -= 48.0f;
				OpAimA1.x -= 48.0f;
				if (OpAim1.x < 352 && OpAimA1.x < 352)
				{
					OpAim1.x = 352.0f;
					OpAimA1.x = 352.0f;
				}
				if (BaseScene::mMasterSoundVol <= 0.0f)
				{
					BaseScene::mMasterSoundVol = 0.0f;
				}
				mTimer->setTime(0.2f);
			}
		}
		//BGMボリューム
		if (optionPos.y == 360)
		{
			OpFlag1 = false;
			OpFlag2 = true;
			OpFlag3 = false;
			if (Input::getKeyDown(KeyCode::D) || Input::joyHorizontal() > 0)
			{
				BaseScene::mBGMSoundVol += 0.1f;
				OpAim2.x += 48.0f;
				OpAimA2.x += 48.0f;
				if (OpAim2.x > 880 && OpAimA2.x > 880)
				{
					OpAim2.x = 880.0f;
					OpAimA2.x = 880.0f;
				}
				if (BaseScene::mBGMSoundVol >= 1.0f)
				{
					BaseScene::mBGMSoundVol = 1.0f;
				}
				mTimer->setTime(0.2f);
			}
			if (Input::getKeyDown(KeyCode::A) || Input::joyHorizontal() < 0)
			{
				BaseScene::mBGMSoundVol -= 0.1f;
				OpAim2.x -= 48.0f;
				OpAimA2.x -= 48.0f;
				if (OpAim2.x < 352 && OpAim2.x < 352)
				{
					OpAim2.x = 352.0f;
					OpAimA2.x = 352.0f;
				}
				if (BaseScene::mBGMSoundVol <= 0.0f)
				{
					BaseScene::mBGMSoundVol = 0.0f;
				}
				mTimer->setTime(0.2f);
			}
		}
		//SEボリューム
		if (optionPos.y == 540)
		{
			OpFlag1 = false;
			OpFlag2 = false;
			OpFlag3 = true;
			if (Input::getKeyDown(KeyCode::D) || Input::joyHorizontal() > 0)
			{
				BaseScene::mSESoundVol += 0.1f;
				OpAim3.x += 48.0f;
				OpAimA3.x += 48.0f;
				if (OpAim3.x > 880 && OpAimA3.x > 880)
				{
					OpAim3.x = 880.0f;
					OpAimA3.x = 880.0f;
				}
				if (BaseScene::mSESoundVol >= 1.0f)
				{

					BaseScene::mSESoundVol = 1.0f;
				}
				mTimer->setTime(0.2f);
			}
			if (Input::getKeyDown(KeyCode::A) || Input::joyHorizontal() < 0)
			{
				BaseScene::mSESoundVol -= 0.1f;
				OpAim3.x -= 48.0f;
				OpAimA3.x -= 48.0f;
				if (OpAim3.x < 352 && OpAimA3.x < 352)
				{
					OpAim3.x = 352.0f;
					OpAimA3.x = 352.0f;
				}
				if (BaseScene::mSESoundVol <= 0.0f)
				{

					BaseScene::mSESoundVol = 0.0f;
				}
				mTimer->setTime(0.2f);
			}
		}
		if (Input::getKeyDown(KeyCode::Enter) || Input::getJoyDown(JoyCode::MenuButton))
		{
			mSettingFlag = false;
		}

		mBGM->setVol(BaseScene::mMasterSoundVol * BaseScene::mBGMSoundVol);
	}
}

void Robbery::ResultF()
{
	if (mResultFlag == false)
	{
		if (mGoalFlag)
		{
			NextScene(std::make_shared<Result>());
		}
	}
	else if (mResultFlag)
	{

		mCamera->GetEye();
		mCamera->GetTarget();
		mCamera->SetEye(camerapos);
		mCamera->SetTarget(setcamerapos);
		camerapos.x += 1;
		mTime += 1;
		if (mTime >= 300)
		{
			camerapos.x -= 1;
			if (selectposition.x <= 0)
			{
				selectposition.x = 820;
			}
			if (selectposition.x > 820)
			{
				selectposition.x = 180;
			}
			if (Input::getKeyDown(KeyCode::A) || Input::joyHorizontal() < 0)
			{
				if (selectposition.y == 360)
				{
					return;
				}
				selectposition.x -= 640;
			}

			if (Input::getKeyDown(KeyCode::D) || Input::joyHorizontal() > 0)
			{
				if (selectposition.y == 360)
				{
					return;
				}
				selectposition.x += 640;
			}

			if (selectposition.x == 180)
			{
				if (Input::getKeyDown(KeyCode::SPACE) || Input::getJoyDown(JoyCode::A))
				{
					NextScene(std::make_shared<Select>());
				}
			}
			if (selectposition.x == 820)
			{
				if (Input::getKeyDown(KeyCode::SPACE) || Input::getJoyDown(JoyCode::A))
				{
					NextScene(std::make_shared<GamePlay>());
				}
			}
		}
	}
}
