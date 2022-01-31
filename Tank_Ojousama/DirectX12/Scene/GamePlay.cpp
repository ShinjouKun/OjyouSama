#include "GamePlay.h"
#include"Title.h"
#include <cstdlib>
#include"Select.h"
#include"Result.h"
#include"GameOver.h"
#include "../Sound/Sound.h"
#include "../Actor/Enemy/SniperEnemy.h"
#include "../Actor/Enemy/BlowEnemy.h"
#include "../Actor/WayPointManager.h"
#include "../Actor/BreadCrumbCreater.h"
#include "../Utility/Timer/Timer.h"
#include "../Render/Camera.h"
#include"../Actor/CameraEye.h"
#include "../Actor/ElfTree.h"
#include "../Actor/ElfRock.h"

#include "../Actor/Enemy/CEnemy.h"
#include "../Actor/Enemy/MortarEnemy.h"

#include "../Items/Repair.h"
#include "../Items/Shield.h"
#include "../Items/Smoke.h"

#include"../Actor/Treasure.h"

GamePlay::GamePlay()
	:mSound(nullptr)
{

}

GamePlay::~GamePlay()
{
	delete objM;//重要
	//delete mBreadCreator;
	//delete mpointManager;
}

void GamePlay::StartScene()
{
	spawnFlag = false;
	spawnFlag2 = false;
	spawnFlag3 = false;
	spawnFlag4 = false;
	objM = new ObjectManager();
	objM->Claer();
	posePos = Vector3(0, 0, 0);
	selectbackPos = Vector3(180, 180, 0);
	selectposition = Vector3(180, 180, 0);
	optionPos = Vector3(180, 180, 0);
	//障害物

	//パンくず生成機作成
	//mBreadCreator = new BreadCrumbCreater(objM);
	mBreadCreator = std::make_shared<BreadCrumbCreater>(objM);
	//WayPoint生成機作成(生成位置,見た目をつけるかどうか)
	//mpointManager = new WayPointManager(Vector3(100.0f, 0.0f, -100.0f), objM, BaseScene::mModel,false);
	mpointManager = std::make_shared<WayPointManager>(Vector3(100.0f, 0.0f, -100.0f), objM, BaseScene::mModel, false);
	//敵AIシステム生成
	mEnemyAI = std::make_shared<EnemyAI>(mpointManager);


	BaseEnemy::SetImportantObject(objM, BaseScene::mModel, BaseScene::mParticle, mBreadCreator);
	//敵にAIセット
	BaseEnemy::SetEnemyAi(mEnemyAI.get());

#pragma region オブジェクト生成

	int objectCount = 0;

	////木
	//for (int i = 330; i < 400; i += 20)
	//{
	//	objM->Add(new ElfTree(Vector3(20,  4.0f, i), Vector3(0.0f, 90.0f, 0.0f), objM, BaseScene::mModel, objectCount++));
	//	objM->Add(new ElfTree(Vector3(-20, 4.0f, i), Vector3(0.0f, 90.0f, 0.0f), objM, BaseScene::mModel, objectCount++));
	//	objM->Add(new ElfTree(Vector3(-70, 4.0f, i), Vector3(0.0f, 90.0f, 0.0f), objM, BaseScene::mModel, objectCount++));
	//}

	////木
	//for (int i = 230; i < 320; i += 20)
	//{
	//	objM->Add(new ElfTree(Vector3(5,   4.0f, i), Vector3(0.0f, 90.0f, 0.0f), objM, BaseScene::mModel, objectCount++));
	//	objM->Add(new ElfTree(Vector3(30,  4.0f, i), Vector3(0.0f, 90.0f, 0.0f), objM, BaseScene::mModel, objectCount++));
	//	objM->Add(new ElfTree(Vector3(-20, 4.0f, i), Vector3(0.0f, 90.0f, 0.0f), objM, BaseScene::mModel, objectCount++));
	//	objM->Add(new ElfTree(Vector3(-50, 4.0f, i), Vector3(0.0f, 90.0f, 0.0f), objM, BaseScene::mModel, objectCount++));
	//}
	//
	////木
	//for (int i = 150; i < 180; i += 20)
	//{
	//	objM->Add(new ElfTree(Vector3(40+i,  4.0f, i), Vector3(0.0f, 90.0f, 0.0f), objM, BaseScene::mModel, objectCount++));
	//	objM->Add(new ElfTree(Vector3(-40-i, 4.0f, i), Vector3(0.0f, 90.0f, 0.0f), objM, BaseScene::mModel, objectCount++));
	//}

	////木
	//for (int i = 100; i < 200; i += 30)
	//{
	//	objM->Add(new ElfTree(Vector3(30,  4.0f, i), Vector3(0.0f, 90.0f, 0.0f), objM, BaseScene::mModel, objectCount++));
	//	objM->Add(new ElfTree(Vector3(15,  4.0f, i), Vector3(0.0f, 90.0f, 0.0f), objM, BaseScene::mModel, objectCount++));
	//	objM->Add(new ElfTree(Vector3(-15, 4.0f, i), Vector3(0.0f, 90.0f, 0.0f), objM, BaseScene::mModel, objectCount++));
	//}

	////木
	//for (int i = 10; i < 80; i += 20)
	//{
	//	objM->Add(new ElfTree(Vector3(60 -i,  4.0f, i), Vector3(0.0f, 90.0f, 0.0f), objM, BaseScene::mModel, objectCount++));
	//	objM->Add(new ElfTree(Vector3(-30 +i, 4.0f, i), Vector3(0.0f, 90.0f, 0.0f), objM, BaseScene::mModel, objectCount++));
	//	objM->Add(new ElfTree(Vector3(20+i,   4.0f, i), Vector3(0.0f, 90.0f, 0.0f), objM, BaseScene::mModel, objectCount++));
	//	objM->Add(new ElfTree(Vector3(-50-i,  4.0f, i), Vector3(0.0f, 90.0f, 0.0f), objM, BaseScene::mModel, objectCount++));
	//}

	///*スタート地点の木*/
	//objM->Add(new ElfRock(Vector3(10.0f, 4.0f, 450.0f), Vector3(0, 45.0f, 0), objM, BaseScene::mModel, objectCount++, 3));
	//objM->Add(new ElfRock(Vector3(-50.0f, 4.0f, 410.0f), Vector3().zero, objM, BaseScene::mModel, objectCount++, 3));
	//objM->Add(new ElfRock(Vector3(0.0f, 4.0f, 350.0f), Vector3(0, 90.0f, 0), objM, BaseScene::mModel, objectCount++, 2));
	//objM->Add(new ElfRock(Vector3(-15.0f, 4.0f, 320.0f), Vector3().zero, objM, BaseScene::mModel, objectCount++, 2));

#pragma region スタートのジグザグ

	//縦岩
	objM->Add(new ElfRock(Vector3(30.0f, 4.0f, 500.0f), Vector3(0, 90.0f, 0), objM, BaseScene::mModel, objectCount++, 3));
	objM->Add(new ElfTree(Vector3(30.0f, 4.0f, 490.0f), Vector3(0.0f, 90.0f, 0.0f), objM, BaseScene::mModel, objectCount++));
	objM->Add(new ElfRock(Vector3(30.0f, 4.0f, 480.0f), Vector3(0, 90.0f, 0), objM, BaseScene::mModel, objectCount++, 3));
	objM->Add(new ElfTree(Vector3(30.0f, 4.0f, 470.0f), Vector3(0.0f, 90.0f, 0.0f), objM, BaseScene::mModel, objectCount++));
	objM->Add(new ElfRock(Vector3(30.0f, 4.0f, 460.0f), Vector3(0, 90.0f, 0), objM, BaseScene::mModel, objectCount++, 3));
	objM->Add(new ElfTree(Vector3(30.0f, 4.0f, 450.0f), Vector3(0.0f, 90.0f, 0.0f), objM, BaseScene::mModel, objectCount++));
	objM->Add(new ElfRock(Vector3(30.0f, 4.0f, 440.0f), Vector3(0, 90.0f, 0), objM, BaseScene::mModel, objectCount++, 3));
	objM->Add(new ElfTree(Vector3(30.0f, 4.0f, 430.0f), Vector3(0.0f, 90.0f, 0.0f), objM, BaseScene::mModel, objectCount++));
	objM->Add(new ElfRock(Vector3(30.0f, 4.0f, 420.0f), Vector3(0, 90.0f, 0), objM, BaseScene::mModel, objectCount++, 3));
	objM->Add(new ElfTree(Vector3(30.0f, 4.0f, 410.0f), Vector3(0.0f, 90.0f, 0.0f), objM, BaseScene::mModel, objectCount++));
	objM->Add(new ElfRock(Vector3(30.0f, 4.0f, 400.0f), Vector3(0, 90.0f, 0), objM, BaseScene::mModel, objectCount++, 3));

	//縦岩
	objM->Add(new ElfRock(Vector3(-30.0f, 4.0f, 500.0f), Vector3(0, 90.0f, 0), objM, BaseScene::mModel, objectCount++, 3));
	objM->Add(new ElfTree(Vector3(-30.0f, 4.0f, 490.0f), Vector3(0.0f, 90.0f, 0.0f), objM, BaseScene::mModel, objectCount++));
	objM->Add(new ElfRock(Vector3(-30.0f, 4.0f, 480.0f), Vector3(0, 90.0f, 0), objM, BaseScene::mModel, objectCount++, 3));
	objM->Add(new ElfTree(Vector3(-30.0f, 4.0f, 470.0f), Vector3(0.0f, 90.0f, 0.0f), objM, BaseScene::mModel, objectCount++));
	objM->Add(new ElfRock(Vector3(-30.0f, 4.0f, 460.0f), Vector3(0, 90.0f, 0), objM, BaseScene::mModel, objectCount++, 3));
	objM->Add(new ElfTree(Vector3(-30.0f, 4.0f, 450.0f), Vector3(0.0f, 90.0f, 0.0f), objM, BaseScene::mModel, objectCount++));
	objM->Add(new ElfRock(Vector3(-30.0f, 4.0f, 440.0f), Vector3(0, 90.0f, 0), objM, BaseScene::mModel, objectCount++, 3));
	objM->Add(new ElfTree(Vector3(-30.0f, 4.0f, 430.0f), Vector3(0.0f, 90.0f, 0.0f), objM, BaseScene::mModel, objectCount++));
	objM->Add(new ElfRock(Vector3(-30.0f, 4.0f, 420.0f), Vector3(0, 90.0f, 0), objM, BaseScene::mModel, objectCount++, 3));
	objM->Add(new ElfTree(Vector3(-30.0f, 4.0f, 410.0f), Vector3(0.0f, 90.0f, 0.0f), objM, BaseScene::mModel, objectCount++));
	objM->Add(new ElfRock(Vector3(-30.0f, 4.0f, 400.0f), Vector3(0, 90.0f, 0), objM, BaseScene::mModel, objectCount++, 3));

	//横岩
	objM->Add(new ElfRock(Vector3(15.0f, 4.0f, 450.0f), Vector3(0, 0.0f, 0), objM, BaseScene::mModel, objectCount++, 3));
	objM->Add(new ElfRock(Vector3(0.0f, 4.0f, 450.0f), Vector3(0, 0.0f, 0), objM, BaseScene::mModel, objectCount++, 3));

	//横岩
	objM->Add(new ElfRock(Vector3(-15.0f, 4.0f, 420.0f), Vector3(0, 0.0f, 0), objM, BaseScene::mModel, objectCount++, 3));
	objM->Add(new ElfRock(Vector3(0.0f, 4.0f, 420.0f), Vector3(0, 0.0f, 0), objM, BaseScene::mModel, objectCount++, 3));

	//横岩
	objM->Add(new ElfRock(Vector3(-15.0f, 4.0f, 420.0f), Vector3(0, 0.0f, 0), objM, BaseScene::mModel, objectCount++, 3));
	objM->Add(new ElfRock(Vector3(0.0f, 4.0f, 420.0f), Vector3(0, 0.0f, 0), objM, BaseScene::mModel, objectCount++, 3));

#pragma endregion

#pragma region 中間の敵湧きポイント

	objM->Add(new ElfTree(Vector3(2.5f, 4.0f, 320.0f), Vector3(0.0f, 90.0f, 0.0f), objM, BaseScene::mModel, objectCount++));
	objM->Add(new ElfTree(Vector3(2.5f, 4.0f, 315.0f), Vector3(0.0f, 90.0f, 0.0f), objM, BaseScene::mModel, objectCount++));
	objM->Add(new ElfTree(Vector3(-2.5f, 4.0f, 320.0f), Vector3(0.0f, 90.0f, 0.0f), objM, BaseScene::mModel, objectCount++));
	objM->Add(new ElfTree(Vector3(-2.5f, 4.0f, 315.0f), Vector3(0.0f, 90.0f, 0.0f), objM, BaseScene::mModel, objectCount++));

#pragma endregion

#pragma region 二回目のジグザグ

	//最初の横岩
	objM->Add(new ElfRock(Vector3(30.0f, 4.0f, 250.0f), Vector3(0, 0.0f, 0), objM, BaseScene::mModel, objectCount++, 3));
	objM->Add(new ElfRock(Vector3(15.0f, 4.0f, 250.0f), Vector3(0, 0.0f, 0), objM, BaseScene::mModel, objectCount++, 3));
	objM->Add(new ElfRock(Vector3(0.0f, 4.0f, 250.0f), Vector3(0, 0.0f, 0), objM, BaseScene::mModel, objectCount++, 3));
	objM->Add(new ElfRock(Vector3(-15.0f, 4.0f, 250.0f), Vector3(0, 0.0f, 0), objM, BaseScene::mModel, objectCount++, 3));

	//木
	for (int i = -40; i < 10; i += 7)
	{
		float x = static_cast<float>(i);
		objM->Add(new ElfTree(Vector3(i, 4.0f, 210.0f), Vector3(0.0f, 90.0f, 0.0f), objM, BaseScene::mModel, objectCount++));
	}

	//岩
	objM->Add(new ElfRock(Vector3(40.0f, 4.0f, 150.0f), Vector3(0, 0.0f, 0), objM, BaseScene::mModel, objectCount++, 3));
	objM->Add(new ElfRock(Vector3(25.0f, 4.0f, 150.0f), Vector3(0, 0.0f, 0), objM, BaseScene::mModel, objectCount++, 3));
	objM->Add(new ElfRock(Vector3(5.0f, 4.0f, 150.0f), Vector3(0, 0.0f, 0), objM, BaseScene::mModel, objectCount++, 3));
	objM->Add(new ElfRock(Vector3(-10.0f, 4.0f, 150.0f), Vector3(0, 0.0f, 0), objM, BaseScene::mModel, objectCount++, 3));

#pragma endregion

#pragma region ボス前の分かれ道

	objM->Add(new ElfRock(Vector3(0.0f, 4.0f, 100.0f), Vector3(0, 90.0f, 0), objM, BaseScene::mModel, objectCount++, 3));
	objM->Add(new ElfTree(Vector3(0.0f, 4.0f, 90.0f), Vector3(0.0f, 90.0f, 0.0f), objM, BaseScene::mModel, objectCount++));
	objM->Add(new ElfRock(Vector3(0.0f, 4.0f, 80.0f), Vector3(0, 90.0f, 0), objM, BaseScene::mModel, objectCount++, 3));
	objM->Add(new ElfTree(Vector3(0.0f, 4.0f, 70.0f), Vector3(0.0f, 90.0f, 0.0f), objM, BaseScene::mModel, objectCount++));
	objM->Add(new ElfRock(Vector3(0.0f, 4.0f, 60.0f), Vector3(0, 90.0f, 0), objM, BaseScene::mModel, objectCount++, 3));
	objM->Add(new ElfTree(Vector3(0.0f, 4.0f, 50.0f), Vector3(0.0f, 90.0f, 0.0f), objM, BaseScene::mModel, objectCount++));
	objM->Add(new ElfRock(Vector3(0.0f, 4.0f, 40.0f), Vector3(0, 90.0f, 0), objM, BaseScene::mModel, objectCount++, 3));
	objM->Add(new ElfTree(Vector3(0.0f, 4.0f, 30.0f), Vector3(0.0f, 90.0f, 0.0f), objM, BaseScene::mModel, objectCount++));
	objM->Add(new ElfRock(Vector3(0.0f, 4.0f, 420.0f), Vector3(0, 90.0f, 0), objM, BaseScene::mModel, objectCount++, 3));

	//右木
	objM->Add(new ElfTree(Vector3(-25, 4.0f, 65.0f), Vector3(0.0f, 90.0f, 0.0f), objM, BaseScene::mModel, objectCount++));
	objM->Add(new ElfTree(Vector3(-25, 4.0f, 70.0f), Vector3(0.0f, 90.0f, 0.0f), objM, BaseScene::mModel, objectCount++));
	objM->Add(new ElfTree(Vector3(-25, 4.0f, 75.0f), Vector3(0.0f, 90.0f, 0.0f), objM, BaseScene::mModel, objectCount++));

#pragma endregion

#pragma endregion

#pragma region 画像追加

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

#pragma region モデル読み込み

	BaseScene::mModel->AddModel("Sora2", "Resouse/skybox.obj", "Resouse/skybox_A.png");
	BaseScene::mModel->AddModel("Groun", "Resouse/Plane.obj", "Resouse/Ground_Black.png");
	BaseScene::mModel->AddModel("KabeR", "Resouse/Plane.obj", "Resouse/kabe.png");
	BaseScene::mModel->AddModel("KabeL", "Resouse/Plane.obj", "Resouse/kabe.png");
	BaseScene::mModel->AddModel("KabeR2", "Resouse/Plane.obj", "Resouse/kabe.png");
	BaseScene::mModel->AddModel("KabeL2", "Resouse/Plane.obj", "Resouse/kabe.png");
	BaseScene::mModel->AddModel("KabeR3", "Resouse/Plane.obj", "Resouse/kabe.png");
	BaseScene::mModel->AddModel("KabeL3", "Resouse/Plane.obj", "Resouse/kabe.png");
	BaseScene::mModel->AddModel("KabeR4", "Resouse/Plane.obj", "Resouse/kabe.png");
	BaseScene::mModel->AddModel("KabeL4", "Resouse/Plane.obj", "Resouse/kabe.png");
	BaseScene::mModel->AddModel("KabeR5", "Resouse/Plane.obj", "Resouse/kabe.png");
	BaseScene::mModel->AddModel("KabeL5", "Resouse/Plane.obj", "Resouse/kabe.png");
	BaseScene::mModel->AddModel("KabeF", "Resouse/Plane.obj", "Resouse/kabe.png");
	BaseScene::mModel->AddModel("KabeF2", "Resouse/Plane.obj", "Resouse/kabe.png");
	BaseScene::mModel->AddModel("KabeB", "Resouse/Plane.obj", "Resouse/kabe.png");
	BaseScene::mModel->AddModel("KabeB2", "Resouse/Plane.obj", "Resouse/kabe.png");

#pragma endregion

#pragma region アイテムとプレイヤーの生成

	//プレイヤーは最後に、又はUIクラスを作る
	objM->Add(new Treasure(Vector3(30.0f, 0, 235.0f), Vector3(0, 90.0f, 0), objM, BaseScene::mModel, 0));
	objM->Add(new GolemEnemy(Vector3(0.0f, 4.0f, -110.0f), Vector3(0, 0, 0), objM, BaseScene::mModel, BaseScene::mSprite, BaseScene::mParticle, 0));
	objM->Add(new Player(Vector3(0.0f, 0.0f, 500.0f), Vector3(0, 0, 0), objM, BaseScene::mModel, BaseScene::mParticle, BaseScene::mSprite, 1));
	objM->Add(new CameraEye(Vector3(0, 0.0f, 180), Vector3(0, 0, 0), objM));
	objM->Add(new Repair(Vector3(20.0f, 0.0f, 480.0f), Vector3(0, 0, 0), objM, BaseScene::mModel, BaseScene::mSprite, ItemState::Low, 0, 50000, 40));
	objM->Add(new Repair(Vector3(30.0f, 0.0f, 120.0f), Vector3(0, 0, 0), objM, BaseScene::mModel, BaseScene::mSprite, ItemState::Low, 1, 50000, 40));
	objM->Add(new Smoke(Vector3(35.0f, 0.0f, 350.0f), Vector3(0, 0, 0), objM, BaseScene::mModel, BaseScene::mParticle, BaseScene::mSprite, ItemState::Low, 0, 5000));
	objM->Add(new Shield(Vector3(-30.0f, 0.0f, 180.0f), Vector3(0, 0, 0), objM, BaseScene::mModel, BaseScene::mSprite, ItemState::Low, 0, 40));

#pragma endregion

	mSound = std::make_shared<Sound>("BGM/loop_12.wav", false);
	mSound->setVol(BaseScene::mMasterSoundVol * BaseScene::mBGMSoundVol);
	mHidan = std::make_shared<Sound>("SE/down.mp3", false);
	mHidan->setVol(BaseScene::mMasterSoundVol * BaseScene::mSESoundVol);
	mTimer = std::make_shared<Timer>(0.01f);
	ParticleBox = make_shared<ParticleEmitterBox>(BaseScene::mParticle);
	ParticleBox->LoadAndSet("Smoke", "Resouse/smoke.jpg");
	mParticleTimer = std::make_shared<Timer>(0.01f);
	goleamDeath = 0;
}

void GamePlay::UpdateScene()
{
	mSound->playLoop();

#ifdef _DEBUG
	ImGui::Begin("pose");
	ImGui::Checkbox("selectflag", &pose);
	ImGui::SliderFloat("soundSize", &BaseScene::mMasterSoundVol, 0, 1);
	ImGui::SliderFloat("soundSize", &BaseScene::mBGMSoundVol, 0, 1);
	ImGui::SliderFloat("soundSize", &BaseScene::mSESoundVol, 0, 1);
	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	ImGui::End();

#endif  _DEBUG

	if (Input::getKeyDown(KeyCode::Alpha1))
	{
		NextScene(std::make_shared<Title>());
	}

	//パンくずを落とす
	mBreadCreator->DropBreadCrumb();
	mEnemyAI->Update();

	if (resultFlag)
	{
		timer += 1;
		if (timer >= 60)
		{
			NextScene(std::make_shared<GameOver>());
		}
	}

	mTimer->update();
	if (!mTimer->isTime()) return;
	Pose();
	Setting();

#pragma region 敵生成

	//ファーストインパクト
	if (!spawnFlag && objM->GetPlayer().GetPosition().z <= 400.0f && objM->GetPlayer().GetSceneFinish())
	{
		int enemyCount = 0;
		objM->Add(new SniperEnemy(Vector3(+25.0f, 0.0f, 320.0f), Vector3(0.0f, 180.0f, 0.0f), enemyCount++));
		objM->Add(new SniperEnemy(Vector3(-25.0f, 0.0f, 320.0f), Vector3(0.0f, 180.0f, 0.0f), enemyCount++));
		objM->Add(new SniperEnemy(Vector3(  0.0f, 0.0f, 290.0f), Vector3(0.0f, 180.0f, 0.0f), enemyCount++));
		objM->Add(new BlowEnemy(Vector3(+5.0f, 0.0f, 330.0f), Vector3(0.0f, 180.0f, 0.0f), enemyCount++));
		objM->Add(new BlowEnemy(Vector3(-5.0f, 0.0f, 330.0f), Vector3(0.0f, 180.0f, 0.0f), enemyCount++));

		spawnFlag = true;
	}

	//セカンドインパクト
	if (!spawnFlag2 && objM->GetPlayer().GetPosition().z <= 250.0f && objM->GetPlayer().GetSceneFinish())
	{
		int enemyCount = 0;
		objM->Add(new SniperEnemy(Vector3(30.0f, 0.0f, 230.0f), Vector3(0.0f, 180.0f, 0.0f), enemyCount++));

		spawnFlag2 = true;
	}

	//サードインパクト
	if (!spawnFlag3 && objM->GetPlayer().GetPosition().z <= 210.0f && objM->GetPlayer().GetSceneFinish())
	{
		int enemyCount = 0;
		objM->Add(new SniperEnemy(Vector3(-30.0f, 0.0f, 170.0f), Vector3(0.0f,180.0f, 0.0f), enemyCount++));
		objM->Add(new SniperEnemy(Vector3(-30.0f, 0.0f, 190.0f), Vector3(0.0f,180.0f, 0.0f), enemyCount++));

		spawnFlag3 = true;
	}

	//フォースインパクト
	if (!spawnFlag4 && objM->GetPlayer().GetPosition().z <= 150.0f && objM->GetPlayer().GetSceneFinish())
	{
		int enemyCount = 0;
		//右
		objM->Add(new SniperEnemy(Vector3(-15.0f, 0.0f, 70.0f), Vector3(0.0f, 180.0f, 0.0f), enemyCount++));
		objM->Add(new SniperEnemy(Vector3(-35.0f, 0.0f, 70.0f), Vector3(0.0f, 180.0f, 0.0f), enemyCount++));
		objM->Add(new SniperEnemy(Vector3(-25.0f, 0.0f, 40.0f), Vector3(0.0f, 180.0f, 0.0f), enemyCount++));
		objM->Add(new BlowEnemy(Vector3(-25.0f, 0.0f, 80.0f), Vector3(0.0f, 180.0f, 0.0f), enemyCount++));

		//左
		objM->Add(new SniperEnemy(Vector3(15.0f, 0.0f, 50.0f), Vector3(0.0f, 180.0f, 0.0f), enemyCount++));
		objM->Add(new SniperEnemy(Vector3(25.0f, 0.0f, 50.0f), Vector3(0.0f, 180.0f, 0.0f), enemyCount++));
		objM->Add(new SniperEnemy(Vector3(35.0f, 0.0f, 50.0f), Vector3(0.0f, 180.0f, 0.0f), enemyCount++));

		spawnFlag4 = true;
	}

	///*遠距離攻撃の敵*/
	//if (!spawnFlag && objM->GetPlayer().GetSceneFinish())
	//{
	//	int i = 0;
	//	objM->Add(new SniperEnemy(Vector3(0.0f, 0.0f, 400.0f), Vector3(0.0f, 180.0f, 0.0f), i++));
	//	objM->Add(new SniperEnemy(Vector3(+10.0f, 0.0f, 420.0f), Vector3(0.0f, 225.0f, 0.0f), i++));
	//	objM->Add(new SniperEnemy(Vector3(-60.0f, 0.0f, 380.0f), Vector3(0.0f, 135.0f, 0.0f), i++));
	//	objM->Add(new SniperEnemy(Vector3(+50.0f, 0.0f, 360.0f), Vector3(0.0f, 90.0f, 0.0f), i++));
	//	objM->Add(new SniperEnemy(Vector3(-50.0f, 0.0f, 380.0f), Vector3(0.0f, 270.0f, 0.0f), i++));
	//	objM->Add(new SniperEnemy(Vector3(2.0f, 0.0f, 340.0f), Vector3(0.0f, 180.0f, 0.0f), i++));
	//	objM->Add(new SniperEnemy(Vector3(+60.0f, 0.0f, 350.0f), Vector3(0.0f, 225.0f, 0.0f), i++));
	//	objM->Add(new SniperEnemy(Vector3(-40.0f, 0.0f, 310.0f), Vector3(0.0f, 135.0f, 0.0f), i++));
	//	objM->Add(new BlowEnemy(Vector3(20.0f, 0.0f, 400.0f), Vector3(0, 0, 0), i++));
	//	objM->Add(new BlowEnemy(Vector3(-20.0f, 0.0f, 400.0f), Vector3(0, 180, 0), i++));
	//	spawnFlag = true;
	//}

	//if (!spawnFlag2&&objM->GetPlayer().GetPosition().z <= 320.0f&&objM->GetPlayer().GetSceneFinish())
	//{
	//	int i = 0;
	//	objM->Add(new BlowEnemy(Vector3(20.0f, 0.0f, 230.0f), Vector3(0, 180, 0), i++));
	//	objM->Add(new BlowEnemy(Vector3(-20.0f, 0.0f, 230.0f), Vector3(0, 180, 0), i++));
	//	objM->Add(new SniperEnemy(Vector3(+10.0f, 0.0f, 250.0f), Vector3(0.0f, 225.0f, 0.0f), i++));
	//	objM->Add(new SniperEnemy(Vector3(-20.0f, 0.0f, 210.0f), Vector3(0.0f, 135.0f, 0.0f), i++));
	//	objM->Add(new SniperEnemy(Vector3(+40.0f, 0.0f, 220.0f), Vector3(0.0f, 90.0f, 0.0f), i++));
	//	objM->Add(new SniperEnemy(Vector3(-30.0f, 0.0f, 230.0f), Vector3(0.0f, 270.0f, 0.0f), i++));
	//	spawnFlag2 = true;
	//}

	//if (!spawnFlag3&&objM->GetPlayer().GetPosition().z <= 200.0f&&objM->GetPlayer().GetSceneFinish())
	//{
	//	int i = 0;
	//	objM->Add(new SniperEnemy(Vector3(+60.0f, 0.0f, 150.0f), Vector3(0.0f, 225.0f, 0.0f), i++));
	//	objM->Add(new SniperEnemy(Vector3(-40.0f, 0.0f, 110.0f), Vector3(0.0f, 135.0f, 0.0f), i++));
	//	objM->Add(new SniperEnemy(Vector3(+10.0f, 0.0f, 120.0f), Vector3(0.0f, 90.0f, 0.0f), i++));
	//	objM->Add(new SniperEnemy(Vector3(-30.0f, 0.0f, 130.0f), Vector3(0.0f, 270.0f, 0.0f), i++));

	//	objM->Add(new SniperEnemy(Vector3(+70.0f, 0.0f, 150.0f), Vector3(0.0f, 225.0f, 0.0f), i++));
	//	objM->Add(new BlowEnemy(Vector3(+60.0f, 0.0f, 120.0f), Vector3(0.0f, 90.0f, 0.0f), i++));
	//	objM->Add(new SniperEnemy(Vector3(-50.0f, 0.0f, 130.0f), Vector3(0.0f, 270.0f, 0.0f), i++));
	//	objM->Add(new SniperEnemy(Vector3(+20.0f, 0.0f, 80.0f), Vector3(0.0f, 135.0f, 0.0f), i++));
	//	objM->Add(new SniperEnemy(Vector3(-20.0f, 0.0f, 60.0f), Vector3(0.0f, 270.0f, 0.0f), i++));
	//	objM->Add(new BlowEnemy(Vector3(40.0f, 0.0f, 70.0f), Vector3(0, 90, 0), i++));
	//	objM->Add(new BlowEnemy(Vector3(0.0f, 0.0f, 70.0f), Vector3(0, 180, 0), i++));

	//	spawnFlag3 = true;
	//}

#pragma endregion

	//シーン遷移処理
	if (objM->GetGolem().GetHp() <= 0)
	{
		goleamDeath++;
		BaseScene::mStageFlag1 = true;
		if (goleamDeath >= 120)
		{
			NextScene(std::make_shared<Result>());
		}
	}

	//体力が0になったら
	if (objM->GetPlayer().GetHp() <= 0)
	{
		mParticleTimer->update();
		resultFlag = true;
	}

}

void GamePlay::DrawScene()
{
	DirectXManager::GetInstance()->SetData3D();
	BaseScene::mModel->Draw("Sora2", Vector3(0, 2.0f, 200.0f), Vector3(0, 0, 0), Vector3(50, 50, 50));
	BaseScene::mModel->Draw("Groun", Vector3(-20.0f, 0.0f, -90.0f), Vector3(0, 0, 0), Vector3(800, 800, 800));

	BaseScene::mModel->Draw("KabeR5", Vector3(-45.0f, 60.0f, 600.0f), Vector3(90.0f, 180.0f, 90.0f), Vector3(100, 80, 50));
	BaseScene::mModel->Draw("KabeR4", Vector3(-45.0f, 60.0f, -200.0f), Vector3(90.0f, 180.0f, 90.0f), Vector3(100, 80, 50));
	BaseScene::mModel->Draw("KabeR3", Vector3(-45.0f, 60.0f, 0.0f), Vector3(90.0f, 180.0f, 90.0f), Vector3(100, 80, 50));
	BaseScene::mModel->Draw("KabeR2", Vector3(-45.0f, 60.0f, 200.0f), Vector3(90.0f, 180.0f, 90.0f), Vector3(100, 80, 50));
	BaseScene::mModel->Draw("KabeR", Vector3(-45.0f, 60.0f, 400.0f), Vector3(90.0f, 180.0f, 90.0f), Vector3(100, 80, 50));
	BaseScene::mModel->Draw("KabeL5", Vector3(+45.0f, 60.0f, 600.0f), Vector3(90.0f, -180.0f, -90.0f), Vector3(100, 80, 50));
	BaseScene::mModel->Draw("KabeL4", Vector3(+45.0f, 60.0f, -200.0f), Vector3(90.0f, -180.0f, -90.0f), Vector3(100, 80, 50));
	BaseScene::mModel->Draw("KabeL3", Vector3(+45.0f, 60.0f, 0.0f), Vector3(90.0f, -180.0f, -90.0f), Vector3(100, 80, 50));
	BaseScene::mModel->Draw("KabeL2", Vector3(+45.0f, 60.0f, 200.0f), Vector3(90.0f, -180.0f, -90.0f), Vector3(100, 80, 50));
	BaseScene::mModel->Draw("KabeL", Vector3(+45.0f, 60.0f, 400.0f), Vector3(90.0f, -180.0f, -90.0f), Vector3(100, 80, 50));

	BaseScene::mModel->Draw("KabeF", Vector3(100.0f, 60.0f, -240.0f), Vector3(90.0f, -180.0f, 0.0f), Vector3(100, 80, 50));
	BaseScene::mModel->Draw("KabeF2", Vector3(-100.0f, 60.0f, -240.0f), Vector3(90.0f, -180.0f, 0.0f), Vector3(100, 80, 50));
	BaseScene::mModel->Draw("KabeB", Vector3(100.0f, 60.0f, 520.0f), Vector3(90.0f, 0.0f, 0.0f), Vector3(100, 80, 50));
	BaseScene::mModel->Draw("KabeB2", Vector3(-100.0f, 60.0f, 520.0f), Vector3(90.0f, 0.0f, 0.0f), Vector3(100, 80, 50));

	objM->Draw();
	DirectXManager::GetInstance()->SetData2D();

	if (pose)
	{
		BaseScene::mSprite->Draw("Pose", posePos, 0.0f, Vector2(0.25f, 0.5f), Vector4(1, 1, 1, 1));
		BaseScene::mSprite->Draw("SBack", selectbackPos, 0.0f, Vector2(0, 0), Vector4(1, 1, 1, 1));
		BaseScene::mSprite->Draw("OptionP", Vector3(820, 180, 0), 0.0f, Vector2(0, 0), Vector4(1, 1, 1, 1));
		BaseScene::mSprite->Draw("Ritorai", Vector3(500, 360, 0), 0.0f, Vector2(0, 0), Vector4(1, 1, 1, 1));
		BaseScene::mSprite->Draw("SelectAim", selectposition, 0.0f, Vector2(0, 0), Vector4(1, 1, 1, 1));
	}
	if (settingFlag)
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
	if (resultFlag)
	{
		ParticleBox->EmitterUpdate("Smoke", Vector3(objM->GetPlayer().GetPosition().x, 0, objM->GetPlayer().GetPosition().z), Vector3(0, 0, 0));
	}
}

void GamePlay::Pose()
{
	//ポーズ
	if (pose == false && settingFlag == false)
	{
		objM->Update();
		/*if (Input::getKey(KeyCode::E) || Input::getJoyDown(JoyCode::A))
		{
			itemHolder->UseItem();
		}*/

		if (Input::getKeyDown(KeyCode::Enter) || Input::getJoyDown(JoyCode::MenuButton))
		{
			pose = true;
		}
	}
	else if (pose)
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
				settingFlag = true;
				pose = false;
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
			pose = false;
		}
	}
}

void GamePlay::Setting()
{
	if (settingFlag == true)
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
			settingFlag = false;
		}

		mSound->setVol(BaseScene::mMasterSoundVol * BaseScene::mBGMSoundVol);
	}
}

void GamePlay::ResultF()
{

	if (resultFlag == false)
	{
		if (objM->GetGolem().GetHp() <= 0)
		{
			BaseScene::mMoney += 20000000;
			NextScene(std::make_shared<Result>());
		}
	}
	else if (resultFlag)
	{

		camera->GetEye();
		camera->GetTarget();
		camera->SetEye(camerapos);
		camera->SetTarget(setcamerapos);
		camerapos.x += 1;
		time += 1;
		if (time >= 300)
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
