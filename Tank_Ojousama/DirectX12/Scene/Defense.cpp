#include "Defense.h"
#include "GamePlay.h"
#include"Title.h"
#include <cstdlib>
#include"Select.h"
#include"Result.h"
#include "../Sound/Sound.h"
#include"../Actor/Castle.h"
#include "../Actor/Enemy/SniperEnemy.h"
#include "../Utility/Timer/Timer.h"
#include"../Actor/CameraEye.h"
#include "../Actor/WayPointManager.h"
#include "../Actor/BreadCrumbCreater.h"
#include "GameOver.h"

#include "../Actor/ElfTree.h"
#include "../Actor/ElfRock.h"

#include "../Actor/Enemy/SniperEnemy.h"
#include"../Actor/Enemy/BlowEnemy.h"
#include"../Actor/Enemy/BirdEnemy.h"
#include"../Actor/Enemy/CEnemy.h"
#include"../Actor/Enemy/MortarEnemy.h"
#include"../Actor/Enemy/AdvanceBorderLine.h"
#include"../Scene/GameOver.h"

Defense::Defense()
	:mSound(nullptr)
{
}

Defense::~Defense()
{
	delete objM;//重要
}


void Defense::StartScene()
{
	objM = new ObjectManager();
	objM->Claer();
	posePos = Vector3(0, 0, 0);
	selectbackPos = Vector3(180, 180, 0);
	selectposition = Vector3(180, 180, 0);
	optionPos = Vector3(180, 180, 0);
	waveMove = Vector3(-360.0f, 0, 0);

	//パンくず生成機作成
	mBreadCreator = std::make_shared<BreadCrumbCreater>(objM);
	//WayPoint生成機作成(生成位置,見た目をつけるかどうか)
	mpointManager = std::make_shared<WayPointManager>(Vector3(100.0f, 0.0f, -100.0f), objM, BaseScene::mModel, false);
	//敵AIシステム生成
	mEnemyAI = std::make_shared<EnemyAI>(mpointManager);
	BaseEnemy::SetImportantObject(objM, BaseScene::mModel, BaseScene::mParticle, mBreadCreator);
	//敵にAIセット
	BaseEnemy::SetEnemyAi(mEnemyAI.get());

	BaseEnemy::SetAttackTarget(Vector3(0.0f, 0.0f, 500.0f));
	objM->Add(new AdvanceBorderLine(Vector3(0, 0.0f, 400), Vector3(0, 0, 0), objM, BaseScene::mModel, 1, Vector3(-150.0f, -2.0f, -2.0f), Vector3(150.0f, 2.0f, 1.0f)));
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
	BaseScene::mSprite->AddTexture("Wave1", "Resouse/wave1.png");
	BaseScene::mSprite->AddTexture("Wave2", "Resouse/wave2.png");
	BaseScene::mSprite->AddTexture("Wave3", "Resouse/wave3.png");

	BaseScene::mModel->AddModel("Sora2", "Resouse/skybox.obj", "Resouse/skybox_A.png");
	BaseScene::mModel->AddModel("Ground2", "Resouse/Plane.obj", "Resouse/sougen.png");
	BaseScene::mModel->AddModel("KabeR", "Resouse/Plane.obj", "Resouse/kabe.png");
	BaseScene::mModel->AddModel("KabeL", "Resouse/Plane.obj", "Resouse/kabe.png");
	BaseScene::mModel->AddModel("KabeR2", "Resouse/Plane.obj", "Resouse/kabe.png");
	BaseScene::mModel->AddModel("KabeL2", "Resouse/Plane.obj", "Resouse/kabe.png");
	BaseScene::mModel->AddModel("KabeR3", "Resouse/Plane.obj", "Resouse/kabe.png");
	BaseScene::mModel->AddModel("KabeL3", "Resouse/Plane.obj", "Resouse/kabe.png");
	BaseScene::mModel->AddModel("KabeB", "Resouse/Plane.obj", "Resouse/kabe.png");
	BaseScene::mModel->AddModel("KabeB2", "Resouse/Plane.obj", "Resouse/kabe.png");

	BaseScene::mModel->AddModel("KabeF", "Resouse/Plane.obj", "Resouse/kabe.png");
	BaseScene::mModel->AddModel("KabeF2", "Resouse/Plane.obj", "Resouse/kabe.png");

	mSound = std::make_shared<Sound>("BGM/loop_157.mp3", false);
	mSound->setVol(BaseScene::mMasterSoundVol * BaseScene::mBGMSoundVol);
	mSE = std::make_shared<Sound>("SE/wave.mp3", false);
	mSE->setVol(BaseScene::mMasterSoundVol * BaseScene::mSESoundVol);

	ParticleBox = make_shared<ParticleEmitterBox>(BaseScene::mParticle);
	ParticleBox->LoadAndSet("Smoke", "Resouse/smoke.jpg");

	mTimer = std::make_shared<Timer>(0.01f);
	interval = 0;
	enemyDeath = 0;
	//実際の敵数よりデカくするな
	spownCount = 0;
	wave1EnemysCount = 3;
	wave2EnemysCount = 1;
	wave3EnemysCount = 1;

	spown1 = false;
	
	spown2 = false;
	
	spown3 = false;
	wave1Clear = false;
	wave2Clear = false;
	wave3Clear = false;

	mCastleCollapses = false;

	//障害物配置
	int objectCount = 0;
#pragma region Z最後列の石たち（敵側）

	objM->Add(new ElfRock(Vector3(150.0f, 4.0f, 270.0f), Vector3(0, 0.0f, 0), objM, BaseScene::mModel, objectCount++, 3));
	objM->Add(new ElfRock(Vector3(140.0f, 4.0f, 270.0f), Vector3(0, 0.0f, 0), objM, BaseScene::mModel, objectCount++, 3));
	objM->Add(new ElfRock(Vector3(130.0f, 4.0f, 270.0f), Vector3(0, 0.0f, 0), objM, BaseScene::mModel, objectCount++, 3));
	objM->Add(new ElfRock(Vector3(120.0f, 4.0f, 270.0f), Vector3(0, 0.0f, 0), objM, BaseScene::mModel, objectCount++, 3));
	objM->Add(new ElfRock(Vector3(110.0f, 4.0f, 270.0f), Vector3(0, 0.0f, 0), objM, BaseScene::mModel, objectCount++, 3));
	objM->Add(new ElfRock(Vector3(100.0f, 4.0f, 270.0f), Vector3(0, 0.0f, 0), objM, BaseScene::mModel, objectCount++, 3));
	objM->Add(new ElfRock(Vector3(90.0f, 4.0f, 270.0f), Vector3(0, 0.0f, 0), objM, BaseScene::mModel, objectCount++, 3));
	objM->Add(new ElfRock(Vector3(80.0f, 4.0f, 270.0f), Vector3(0, 0.0f, 0), objM, BaseScene::mModel, objectCount++, 3));

	objM->Add(new ElfRock(Vector3(50.0f, 4.0f, 270.0f), Vector3(0, 0.0f, 0), objM, BaseScene::mModel, objectCount++, 3));
	objM->Add(new ElfRock(Vector3(40.0f, 4.0f, 270.0f), Vector3(0, 0.0f, 0), objM, BaseScene::mModel, objectCount++, 3));
	objM->Add(new ElfRock(Vector3(30.0f, 4.0f, 270.0f), Vector3(0, 0.0f, 0), objM, BaseScene::mModel, objectCount++, 3));
	objM->Add(new ElfRock(Vector3(20.0f, 4.0f, 270.0f), Vector3(0, 0.0f, 0), objM, BaseScene::mModel, objectCount++, 3));

	objM->Add(new ElfRock(Vector3(-20.0f, 4.0f, 270.0f), Vector3(0, 0.0f, 0), objM, BaseScene::mModel, objectCount++, 3));
	objM->Add(new ElfRock(Vector3(-30.0f, 4.0f, 270.0f), Vector3(0, 0.0f, 0), objM, BaseScene::mModel, objectCount++, 3));
	objM->Add(new ElfRock(Vector3(-40.0f, 4.0f, 270.0f), Vector3(0, 0.0f, 0), objM, BaseScene::mModel, objectCount++, 3));
	objM->Add(new ElfRock(Vector3(-50.0f, 4.0f, 270.0f), Vector3(0, 0.0f, 0), objM, BaseScene::mModel, objectCount++, 3));
	objM->Add(new ElfRock(Vector3(-80.0f, 4.0f, 270.0f), Vector3(0, 0.0f, 0), objM, BaseScene::mModel, objectCount++, 3));

	objM->Add(new ElfRock(Vector3(-90.0f, 4.0f, 270.0f), Vector3(0, 0.0f, 0), objM, BaseScene::mModel, objectCount++, 3));
	objM->Add(new ElfRock(Vector3(-100.0f, 4.0f, 270.0f), Vector3(0, 0.0f, 0), objM, BaseScene::mModel, objectCount++, 3));
	objM->Add(new ElfRock(Vector3(-110.0f, 4.0f, 270.0f), Vector3(0, 0.0f, 0), objM, BaseScene::mModel, objectCount++, 3));
	objM->Add(new ElfRock(Vector3(-120.0f, 4.0f, 270.0f), Vector3(0, 0.0f, 0), objM, BaseScene::mModel, objectCount++, 3));
	objM->Add(new ElfRock(Vector3(-130.0f, 4.0f, 270.0f), Vector3(0, 0.0f, 0), objM, BaseScene::mModel, objectCount++, 3));
	objM->Add(new ElfRock(Vector3(-140.0f, 4.0f, 270.0f), Vector3(0, 0.0f, 0), objM, BaseScene::mModel, objectCount++, 3));
	objM->Add(new ElfRock(Vector3(-150.0f, 4.0f, 270.0f), Vector3(0, 0.0f, 0), objM, BaseScene::mModel, objectCount++, 3));
#pragma endregion

#pragma region Z最後列の木たち

	for (int i = -150; i < 150; i += 20)
	{
		objM->Add(new ElfTree(Vector3(i, 4.0f, 335), Vector3(0.0f, 90.0f, 0.0f), objM, BaseScene::mModel, objectCount++));
		objM->Add(new ElfTree(Vector3(i, 4.0f, 335), Vector3(0.0f, 90.0f, 0.0f), objM, BaseScene::mModel, objectCount++));
		objM->Add(new ElfTree(Vector3(i, 4.0f, 335), Vector3(0.0f, 90.0f, 0.0f), objM, BaseScene::mModel, objectCount++));
		objM->Add(new ElfTree(Vector3(i, 4.0f, 335), Vector3(0.0f, 90.0f, 0.0f), objM, BaseScene::mModel, objectCount++));
	}
#pragma endregion

#pragma region 遮蔽物
	/*objM->Add(new ElfRock(Vector3(120.0f, 4.0f, 400.0f), Vector3(0, 0.0f, 0), objM, BaseScene::mModel, objectCount++, 3));
	objM->Add(new ElfRock(Vector3(110.0f, 4.0f, 390.0f), Vector3(0, 0.0f, 0), objM, BaseScene::mModel, objectCount++, 3));

	objM->Add(new ElfRock(Vector3(60.0f, 4.0f, 410.0f), Vector3(0, 0.0f, 0), objM, BaseScene::mModel, objectCount++, 2));
	objM->Add(new ElfRock(Vector3(40.0f, 4.0f, 450.0f), Vector3(0, 0.0f, 0), objM, BaseScene::mModel, objectCount++, 3));
	objM->Add(new ElfRock(Vector3(-20.0f, 4.0f, 450.0f), Vector3(0, 0.0f, 0), objM, BaseScene::mModel, objectCount++, 2));
	objM->Add(new ElfRock(Vector3(-120.0f, 4.0f, 400.0f), Vector3(0, 0.0f, 0), objM, BaseScene::mModel, objectCount++, 3));
	objM->Add(new ElfRock(Vector3(-110.0f, 4.0f, 390.0f), Vector3(0, 0.0f, 0), objM, BaseScene::mModel, objectCount++, 2));*/
#pragma endregion


	objM->Add(new Castle(Vector3(0.0f, -10.0f, 550.0f), Vector3(0, 0, 0), objM, BaseScene::mModel, BaseScene::mSprite, BaseScene::mParticle));
	objM->Add(new Player(Vector3(0.0f, 0.0f, 500.0f), Vector3(0, 0, 0), objM, BaseScene::mModel, BaseScene::mParticle, BaseScene::mSprite, 3));
	objM->Add(new CameraEye(Vector3(0, 0.0f, 180), Vector3(0, 0, 0), objM));

}

void Defense::UpdateScene()
{
	
	mTimer->update();
	if (resultFlag)
	{
		timer += 1;
		if (timer >= 60)
		{
			NextScene(std::make_shared<GameOver>());
		}
	}
	if (!mTimer->isTime()) return;
	Pose();
	Setting();
	mSound->playLoop();
	if (resultFlag)
	{
		timer += 1;
		if (timer >= 60)
		{
			NextScene(std::make_shared<GameOver>());
		}
	}
	//プレイヤーの体力が0かつ城の体力が0で無いとき
	if (objM->GetPlayer().GetHp() <= 0 && !mCastleCollapses)
	{
		resultFlag = true;
	}
	//城の体力がゼロになったら
	if (objM->GetCastle().GetHP() <= 0)
	{
		mCastleCollapses = true;
	}
	//パンくずを落とす
	mBreadCreator->DropBreadCrumb();
	mEnemyAI->Update();
	if (!wave1Clear)
	{
		if (objM->GetPlayer().GetSceneFinish())
		{
			Wave1();
		}
	}
	else if (!wave2Clear)
	{
		Wave2();
	}
	else if (!wave3Clear)
	{
		Wave3();
	}

	if (mCastleCollapses && !resultFlag)
	{
		if (objM->GetCastle().GetPosition().y <= -100.f)
		{
			resultFlag = true;
		}
	}
}

void Defense::DrawScene()
{
	DirectXManager::GetInstance()->SetData3D();
	BaseScene::mModel->Draw("Sora2", Vector3(0, 2.0f, 200.0f), Vector3(0, 0, 0), Vector3(50, 50, 50));
	BaseScene::mModel->Draw("Ground2", Vector3(-20.0f, 0.0f, -90.0f), Vector3(0, 0, 0), Vector3(1000, 1000, 1000));

	BaseScene::mModel->Draw("KabeR3", Vector3(-160.0f, 60.0f, 600.0f), Vector3(90.0f, 180.0f, 90.0f), Vector3(100, 80, 50));
	BaseScene::mModel->Draw("KabeL3", Vector3(160.0f, 60.0f, 600.0f), Vector3(90.0f, -180.0f, -90.0f), Vector3(100, 80, 50));
	BaseScene::mModel->Draw("KabeR2", Vector3(-160.0f, 60.0f, 200.0f), Vector3(90.0f, 180.0f, 90.0f), Vector3(100, 80, 50));
	BaseScene::mModel->Draw("KabeL2", Vector3(160.0f, 60.0f, 200.0f), Vector3(90.0f, -180.0f, -90.0f), Vector3(100, 80, 50));
	BaseScene::mModel->Draw("KabeR", Vector3(-160.0f, 60.0f, 400.0f), Vector3(90.0f, 180.0f, 90.0f), Vector3(100, 80, 50));
	BaseScene::mModel->Draw("KabeL", Vector3(160.0f, 60.0f, 400.0f), Vector3(90.0f, -180.0f, -90.0f), Vector3(100, 80, 50));
	BaseScene::mModel->Draw("KabeB", Vector3(100.0f, 60.0f, 600.0f), Vector3(90.0f, 0.0f, 0.0f), Vector3(100, 80, 50));
	BaseScene::mModel->Draw("KabeB2", Vector3(-100.0f, 60.0f, 600.0f), Vector3(90.0f, 0.0f, 0.0f), Vector3(100, 80, 50));
	BaseScene::mModel->Draw("KabeF", Vector3(100.0f, 60.0f, 180.0f), Vector3(90.0f, -180.0f, 0.0f), Vector3(100, 80, 50));
	BaseScene::mModel->Draw("KabeF2", Vector3(-100.0f, 60.0f, 180.0f), Vector3(90.0f, -180.0f, 0.0f), Vector3(100, 80, 50));


	objM->Draw();
	DirectXManager::GetInstance()->SetData2D();
	if (spown1&&interval <= 160)
	{
		BaseScene::mSprite->Draw("Wave1", Vector3(waveMove.x, 0, 0), 0.0f, Vector2(1, 1), Vector4(1, 1, 1, 1));
	}
	if (spown2&&interval <= 290)
	{
		BaseScene::mSprite->Draw("Wave2", Vector3(waveMove.x, 0, 0), 0.0f, Vector2(1, 1), Vector4(1, 1, 1, 1));
	}
	if (spown3&&interval <= 420)
	{
		BaseScene::mSprite->Draw("Wave3", Vector3(waveMove.x, 0, 0), 0.0f, Vector2(1, 1), Vector4(1, 1, 1, 1));
	}
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



void Defense::Wave1EnemySpown()
{
	enemyDeath = 0;
	int enemyCount = 0;
	//ここの下からadd
	objM->Add(new SniperEnemy(Vector3(0.0f, 0.0f, 340.0f), Vector3(0.0f, 180.0f, 0.0f), enemyCount++, true));
	objM->Add(new SniperEnemy(Vector3(30.0f, 0.0f, 340.0f), Vector3(0.0f, 180.0f, 0.0f), enemyCount++, true));
	objM->Add(new SniperEnemy(Vector3(-30.0f, 0.0f, 340.0f), Vector3(0.0f, 180.0f, 0.0f), enemyCount++, true));

	objM->Add(new SniperEnemy(Vector3(60.0f, 0.0f, 340.0f), Vector3(0.0f, 180.0f, 0.0f), enemyCount++, true));
	objM->Add(new SniperEnemy(Vector3(20.0f, 0.0f, 320.0f), Vector3(0.0f, 180.0f, 0.0f), enemyCount++, true));
	objM->Add(new SniperEnemy(Vector3(-20.0f, 0.0f, 320.0f), Vector3(0.0f, 180.0f, 0.0f), enemyCount++, true));
	objM->Add(new SniperEnemy(Vector3(-60.0f, 0.0f, 340.0f), Vector3(0.0f, 180.0f, 0.0f), enemyCount++, true));

	objM->Add(new SniperEnemy(Vector3(40.0f, 0.0f, 330.0f), Vector3(0.0f, 180.0f, 0.0f), enemyCount++, true));
	objM->Add(new SniperEnemy(Vector3(20.0f, 0.0f, 340.0f), Vector3(0.0f, 180.0f, 0.0f), enemyCount++, true));
	objM->Add(new SniperEnemy(Vector3(0.0f, 0.0f, 340.0f), Vector3(0.0f, 180.0f, 0.0f), enemyCount++, true));
	objM->Add(new SniperEnemy(Vector3(-20.0f, 0.0f, 340.0f), Vector3(0.0f, 180.0f, 0.0f), enemyCount++, true));
	objM->Add(new SniperEnemy(Vector3(-40.0f, 0.0f, 330.0f), Vector3(0.0f, 180.0f, 0.0f), enemyCount++, true));

	spown1 = true;
}


void Defense::Wave2EnemySpown()
{
	enemyDeath = 0;
	int enemyCount = 0;
	interval = 160;
	waveMove = Vector3(-360.0f, 0, 0);
	//ここの下からadd
	objM->Add(new SniperEnemy(Vector3(-40.0f, 0.0f, 330.0f), Vector3(0.0f, 180.0f, 0.0f), enemyCount++, true));
	objM->Add(new BlowEnemy(Vector3(-20.0f, 0.0f, 350.0f), Vector3(0.0f, 180.0f, 0.0f), enemyCount++, true));
	objM->Add(new BlowEnemy(Vector3(20.0f, 0.0f, 350.0f), Vector3(0.0f, 180.0f, 0.0f), enemyCount++, true));
	objM->Add(new SniperEnemy(Vector3(-40.0f, 0.0f, 330.0f), Vector3(0.0f, 180.0f, 0.0f), enemyCount++, true));

	objM->Add(new BlowEnemy(Vector3(-30.0f, 0.0f, 340.0f), Vector3(0.0f, 180.0f, 0.0f), enemyCount++, true));
	objM->Add(new BlowEnemy(Vector3(30.0f, 0.0f, 340.0f), Vector3(0.0f, 180.0f, 0.0f), enemyCount++, true));

	objM->Add(new SniperEnemy(Vector3(-60.0f, 0.0f, 300.0f), Vector3(0.0f, 180.0f, 0.0f), enemyCount++, true));
	objM->Add(new SniperEnemy(Vector3(60.0f, 0.0f, 300.0f), Vector3(0.0f, 180.0f, 0.0f), enemyCount++, true));
	objM->Add(new SniperEnemy(Vector3(-40.0f, 0.0f, 300.0f), Vector3(0.0f, 180.0f, 0.0f), enemyCount++, true));
	objM->Add(new SniperEnemy(Vector3(40.0f, 0.0f, 300.0f), Vector3(0.0f, 180.0f, 0.0f), enemyCount++, true));

	objM->Add(new BlowEnemy(Vector3(25.0f, 0.0f, 300.0f), Vector3(0.0f, 180.0f, 0.0f), enemyCount++, true));
	objM->Add(new BlowEnemy(Vector3(-25.0f, 0.0f, 300.0f), Vector3(0.0f, 180.0f, 0.0f), enemyCount++, true));

	objM->Add(new BirdEnemy(Vector3(50.0f, 0.0f, 250), Vector3(0.0f, 90.0f, 0.0f), BaseScene::mSprite, enemyCount++));
	objM->Add(new BirdEnemy(Vector3(-50.0f, 0.0f, 250), Vector3(0.0f, 90.0f, 0.0f), BaseScene::mSprite, enemyCount++));

	objM->Add(new SniperEnemy(Vector3(-80.0f, 0.0f, 320.0f), Vector3(0.0f, 180.0f, 0.0f), enemyCount++, true));
	objM->Add(new SniperEnemy(Vector3(80.0f, 0.0f, 320.0f), Vector3(0.0f, 180.0f, 0.0f), enemyCount++, true));

	objM->Add(new CEnemy(Vector3(0.0f, 0.0f, 340.0f), Vector3(0.0f, 180.0f, 0.0f), enemyCount++, true));
	spown2 = true;
}


void Defense::Wave3EnemySpown()
{
	enemyDeath = 0;
	int enemyCount = 0;
	interval = 290;
	waveMove = Vector3(-360.0f, 0, 0);
	//ここの下からadd
	objM->Add(new MortarEnemy(Vector3(-70.0f, 0.0f, 350.0f), Vector3(0.0f, 180.0f, 0.0f), enemyCount++));
	objM->Add(new MortarEnemy(Vector3(70.0f, 0.0f, 350.0f), Vector3(0.0f, 180.0f, 0.0f), enemyCount++));

	objM->Add(new BlowEnemy(Vector3(25.0f, 0.0f, 300.0f), Vector3(0.0f, 180.0f, 0.0f), enemyCount++, true));
	objM->Add(new BlowEnemy(Vector3(-25.0f, 0.0f, 300.0f), Vector3(0.0f, 180.0f, 0.0f), enemyCount++, true));

	objM->Add(new SniperEnemy(Vector3(30.0f, 0.0f, 340.0f), Vector3(0.0f, 180.0f, 0.0f), enemyCount++, true));
	objM->Add(new SniperEnemy(Vector3(-30.0f, 0.0f, 340.0f), Vector3(0.0f, 180.0f, 0.0f), enemyCount++, true));
	objM->Add(new SniperEnemy(Vector3(40.0f, 0.0f, 340.0f), Vector3(0.0f, 180.0f, 0.0f), enemyCount++, true));
	objM->Add(new SniperEnemy(Vector3(-40.0f, 0.0f, 340.0f), Vector3(0.0f, 180.0f, 0.0f), enemyCount++, true));

	objM->Add(new BirdEnemy(Vector3(40.0f, 0.0f, 250), Vector3(0.0f, 90.0f, 0.0f), BaseScene::mSprite, enemyCount++));
	objM->Add(new BirdEnemy(Vector3(-70.0f, 0.0f, 250), Vector3(0.0f, 90.0f, 0.0f), BaseScene::mSprite, enemyCount++));

	objM->Add(new CEnemy(Vector3(40.0f, 0.0f, 320.0f), Vector3(0.0f, 180.0f, 0.0f), enemyCount++, true));
	objM->Add(new CEnemy(Vector3(-40.0f, 0.0f, 320.0f), Vector3(0.0f, 180.0f, 0.0f), enemyCount++, true));

	objM->Add(new SniperEnemy(Vector3(50.0f, 0.0f, 340.0f), Vector3(0.0f, 180.0f, 0.0f), enemyCount++, true));
	objM->Add(new SniperEnemy(Vector3(-50.0f, 0.0f, 340.0f), Vector3(0.0f, 180.0f, 0.0f), enemyCount++, true));
	objM->Add(new SniperEnemy(Vector3(10.0f, 0.0f, 330.0f), Vector3(0.0f, 180.0f, 0.0f), enemyCount++, true));
	objM->Add(new SniperEnemy(Vector3(10.0f, 0.0f, 330.0f), Vector3(0.0f, 180.0f, 0.0f), enemyCount++, true));
	spown3 = true;
}


void Defense::Wave1()
{
	//テキスト表示
	//インターバル
	interval++;
	waveMove.x += 8.0f;
	//敵のadd
	
	if (!spown1)
	{
		mSE->play();
		Wave1EnemySpown();
	}
	else
	{
		int i = objM->GetEnemyList().size();
		
		//場にエネミーが残っているか？
		////場にエネミーが残っているか？
	
		if(i <= 0)
		{
			wave1Clear = true;
		}
	}
	
	
	

}

void Defense::Wave2()
{
	//テキスト表示
	//インターバル
	interval++;
	waveMove.x += 8.0f;
	//敵のadd
	if (!spown2)
	{
		mSE->play();
		Wave2EnemySpown();
	}
	else
	{
		int i = objM->GetEnemyList().size();

		//場にエネミーが残っているか？
		////場にエネミーが残っているか？

		if (i <= 0)
		{
			wave2Clear = true;
		}
	}


}

void Defense::Wave3()
{
	//テキスト表示
	//インターバル
	interval++;
	waveMove.x += 8.0f;
	//敵のadd
	if (!spown3)
	{
		mSE->play();
		Wave3EnemySpown();
	}
	else
	{
		int i = objM->GetEnemyList().size();
		
		if (i <= 0)
		{
			BaseScene::mStageFlag3 = true;
			NextScene(std::make_shared<Result>());
		}
	}

}

void Defense::Pose()
{
	//ポーズ
	if (pose == false && settingFlag == false)
	{
		objM->Update();
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
				NextScene(std::make_shared<Defense>());
			}
		}
		if (Input::getKeyDown(KeyCode::Enter) || Input::getJoyDown(JoyCode::MenuButton))
		{
			pose = false;
		}
	}
}

void Defense::Setting()
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