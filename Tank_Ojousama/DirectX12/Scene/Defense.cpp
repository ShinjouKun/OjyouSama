#include "Defense.h"
#include "GamePlay.h"
#include"Title.h"
#include <cstdlib>
#include"Select.h"
#include"Result.h"
#include "../Sound/Sound.h"
#include "../Actor/Enemy/SniperEnemy.h"
#include "../Utility/Timer/Timer.h"
#include"../Actor/CameraEye.h"
#include "../Actor/WayPointManager.h"
#include "../Actor/BreadCrumbCreater.h"

#include "../Actor/ElfTree.h"
#include "../Actor/ElfRock.h"

#include "../Actor/Enemy/SniperEnemy.h"

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

	//パンくず生成機作成
	mBreadCreator = std::make_shared<BreadCrumbCreater>(objM);
	//WayPoint生成機作成(生成位置,見た目をつけるかどうか)
	mpointManager = std::make_shared<WayPointManager>(Vector3(100.0f, 0.0f, -100.0f), objM, BaseScene::mModel, false);
	//敵AIシステム生成
	mEnemyAI = std::make_shared<EnemyAI>(mpointManager);

	//敵にマネージャーセット
	BaseEnemy::SetObjectManager(objM);
	//敵にパンくずセット
	BaseEnemy::SetBreadCreator(mBreadCreator.get());
	//敵にAIセット
	BaseEnemy::SetEnemyAi(mEnemyAI.get());

	itemHolder = new ItemHolder();
	itemHolder->Init();

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

	mSound = std::make_shared<Sound>("loop_157.mp3", false);
	mSound->setVol(BaseScene::mMasterSoundVol * BaseScene::mBGMSoundVol);
	
	mTimer = std::make_shared<Timer>(0.01f);
	interval = 0;
	enemyDeath = 0;
	//実際の敵数よりデカくするな
	wave1EnemysCount = 1;
	wave2EnemysCount = 20;
	wave3EnemysCount = 25;
	spown1 = false;
	spown2 = false;
	spown3 = false;
	wave1Clear = false;
	wave2Clear = false;
	wave3Clear = false;

	//障害物配置
	int objectCount = 0;
#pragma region Z最後列の石たち（敵側）
	
	objM->Add(new ElfRock(Vector3(150.0f, 4.0f, 330.0f), Vector3(0, 0.0f, 0), objM, BaseScene::mModel, objectCount++, 3));
	objM->Add(new ElfRock(Vector3(140.0f, 4.0f, 330.0f), Vector3(0, 0.0f, 0), objM, BaseScene::mModel, objectCount++, 3));
	objM->Add(new ElfRock(Vector3(130.0f, 4.0f, 330.0f), Vector3(0, 0.0f, 0), objM, BaseScene::mModel, objectCount++, 3));
	objM->Add(new ElfRock(Vector3(120.0f, 4.0f, 330.0f), Vector3(0, 0.0f, 0), objM, BaseScene::mModel, objectCount++, 3));
	objM->Add(new ElfRock(Vector3(110.0f, 4.0f, 330.0f), Vector3(0, 0.0f, 0), objM, BaseScene::mModel, objectCount++, 3));
	objM->Add(new ElfRock(Vector3(100.0f, 4.0f, 330.0f), Vector3(0, 0.0f, 0), objM, BaseScene::mModel, objectCount++, 3));
	objM->Add(new ElfRock(Vector3(90.0f, 4.0f, 330.0f), Vector3(0, 0.0f, 0), objM, BaseScene::mModel, objectCount++, 3));
	objM->Add(new ElfRock(Vector3(80.0f, 4.0f, 330.0f), Vector3(0, 0.0f, 0), objM, BaseScene::mModel, objectCount++, 3));
	
	objM->Add(new ElfRock(Vector3(50.0f, 4.0f, 330.0f), Vector3(0, 0.0f, 0), objM, BaseScene::mModel, objectCount++, 3));
	objM->Add(new ElfRock(Vector3(40.0f, 4.0f, 330.0f), Vector3(0, 0.0f, 0), objM, BaseScene::mModel, objectCount++, 3));
	objM->Add(new ElfRock(Vector3(30.0f, 4.0f, 330.0f), Vector3(0, 0.0f, 0), objM, BaseScene::mModel, objectCount++, 3));
	objM->Add(new ElfRock(Vector3(20.0f, 4.0f, 330.0f), Vector3(0, 0.0f, 0), objM, BaseScene::mModel, objectCount++, 3));
	
	objM->Add(new ElfRock(Vector3(-20.0f, 4.0f, 330.0f), Vector3(0, 0.0f, 0), objM, BaseScene::mModel, objectCount++, 3));
	objM->Add(new ElfRock(Vector3(-30.0f, 4.0f, 330.0f), Vector3(0, 0.0f, 0), objM, BaseScene::mModel, objectCount++, 3));
	objM->Add(new ElfRock(Vector3(-40.0f, 4.0f, 330.0f), Vector3(0, 0.0f, 0), objM, BaseScene::mModel, objectCount++, 3));
	objM->Add(new ElfRock(Vector3(-50.0f, 4.0f, 330.0f), Vector3(0, 0.0f, 0), objM, BaseScene::mModel, objectCount++, 3));
	objM->Add(new ElfRock(Vector3(-80.0f, 4.0f, 330.0f), Vector3(0, 0.0f, 0), objM, BaseScene::mModel, objectCount++, 3));
	
	objM->Add(new ElfRock(Vector3(-90.0f, 4.0f, 330.0f), Vector3(0, 0.0f, 0), objM, BaseScene::mModel, objectCount++, 3));
	objM->Add(new ElfRock(Vector3(-100.0f, 4.0f, 330.0f), Vector3(0, 0.0f, 0), objM, BaseScene::mModel, objectCount++, 3));
	objM->Add(new ElfRock(Vector3(-110.0f, 4.0f, 330.0f), Vector3(0, 0.0f, 0), objM, BaseScene::mModel, objectCount++, 3));
	objM->Add(new ElfRock(Vector3(-120.0f, 4.0f, 330.0f), Vector3(0, 0.0f, 0), objM, BaseScene::mModel, objectCount++, 3));
	objM->Add(new ElfRock(Vector3(-130.0f, 4.0f, 330.0f), Vector3(0, 0.0f, 0), objM, BaseScene::mModel, objectCount++, 3));
	objM->Add(new ElfRock(Vector3(-140.0f, 4.0f, 330.0f), Vector3(0, 0.0f, 0), objM, BaseScene::mModel, objectCount++, 3));
	objM->Add(new ElfRock(Vector3(-150.0f, 4.0f, 330.0f), Vector3(0, 0.0f, 0), objM, BaseScene::mModel, objectCount++, 3));
#pragma endregion

#pragma region Z最後列の木たち

	for (int i = -150; i < 150; i += 20)
	{
		objM->Add(new ElfTree(Vector3(i, 4.0f, 320), Vector3(0.0f, 90.0f, 0.0f), objM, BaseScene::mModel, objectCount++));
		objM->Add(new ElfTree(Vector3(i, 4.0f, 320), Vector3(0.0f, 90.0f, 0.0f), objM, BaseScene::mModel, objectCount++));
		objM->Add(new ElfTree(Vector3(i, 4.0f, 320), Vector3(0.0f, 90.0f, 0.0f), objM, BaseScene::mModel, objectCount++));
		objM->Add(new ElfTree(Vector3(i, 4.0f, 320), Vector3(0.0f, 90.0f, 0.0f), objM, BaseScene::mModel, objectCount++));
	}
#pragma endregion

#pragma region 遮蔽物
	objM->Add(new ElfRock(Vector3(120.0f, 4.0f, 400.0f), Vector3(0, 0.0f, 0), objM, BaseScene::mModel, objectCount++, 3));
	objM->Add(new ElfRock(Vector3(110.0f, 4.0f, 390.0f), Vector3(0, 0.0f, 0), objM, BaseScene::mModel, objectCount++, 3));

	objM->Add(new ElfRock(Vector3(60.0f, 4.0f, 410.0f), Vector3(0, 0.0f, 0), objM, BaseScene::mModel, objectCount++, 2));
	objM->Add(new ElfRock(Vector3(40.0f, 4.0f, 450.0f), Vector3(0, 0.0f, 0), objM, BaseScene::mModel, objectCount++, 3));
	objM->Add(new ElfRock(Vector3(-20.0f, 4.0f, 450.0f), Vector3(0, 0.0f, 0), objM, BaseScene::mModel, objectCount++, 2));
	objM->Add(new ElfRock(Vector3(-120.0f, 4.0f, 400.0f), Vector3(0, 0.0f, 0), objM, BaseScene::mModel, objectCount++, 3));
	objM->Add(new ElfRock(Vector3(-110.0f, 4.0f, 390.0f), Vector3(0, 0.0f, 0), objM, BaseScene::mModel, objectCount++, 2));
#pragma endregion



	objM->Add(new Player(Vector3(0.0f, 0.0f, 500.0f), Vector3(0, 0, 0), objM, BaseScene::mModel, BaseScene::mParticle, BaseScene::mSprite,3));
	objM->Add(new CameraEye(Vector3(0, 0.0f, 180), Vector3(0, 0, 0), objM));

}

void Defense::UpdateScene()
{
	mSound->playLoop();
	//パンくずを落とす
	mBreadCreator->DropBreadCrumb();
	mEnemyAI->Update();
	if (!wave1Clear)
	{
		Wave1();
	}
	if (wave1Clear && !wave2Clear)
	{
		Wave2();
	}
	if (wave1Clear&&wave2Clear && !wave3Clear)
	{
		Wave3();
	}
	mTimer->update();
	if (!mTimer->isTime()) return;
	Pose();
	Setting();
}

void Defense::DrawScene()
{
	DirectXManager::GetInstance()->SetData3D();
	BaseScene::mModel->Draw("Sora2", Vector3(0, 2.0f, 200.0f), Vector3(0, 0, 0), Vector3(50, 50, 50));
	BaseScene::mModel->Draw("Ground2", Vector3(-20.0f, 0.0f, -90.0f), Vector3(0, 0, 0), Vector3(1000, 1000, 1000));
	
	
	BaseScene::mModel->Draw("KabeR2", Vector3(-160.0f, 60.0f, 200.0f), Vector3(90.0f, 180.0f, 90.0f), Vector3(100, 80, 50));
	BaseScene::mModel->Draw("KabeL2", Vector3(160.0f, 60.0f, 200.0f), Vector3(90.0f, -180.0f, -90.0f), Vector3(100, 80, 50));
	BaseScene::mModel->Draw("KabeR", Vector3(-160.0f, 60.0f, 400.0f), Vector3(90.0f, 180.0f, 90.0f), Vector3(100, 80, 50));
	BaseScene::mModel->Draw("KabeL", Vector3(160.0f, 60.0f, 400.0f), Vector3(90.0f, -180.0f, -90.0f), Vector3(100, 80, 50));
	BaseScene::mModel->Draw("KabeF", Vector3(100.0f, 60.0f, 180.0f), Vector3(90.0f, -180.0f, 0.0f), Vector3(100, 80, 50));
	BaseScene::mModel->Draw("KabeF2", Vector3(-100.0f, 60.0f, 180.0f), Vector3(90.0f, -180.0f, 0.0f), Vector3(100, 80, 50));

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
	if (resultFlag && time >= 300)
	{
		BaseScene::mSprite->Draw("Pose", posePos, 0.0f, Vector2(0.25f, 0.5f), Vector4(1, 1, 1, 1));
		BaseScene::mSprite->Draw("SBack", selectbackPos, 0.0f, Vector2(0, 0), Vector4(1, 1, 1, 1));
		BaseScene::mSprite->Draw("Ritorai", Vector3(820, 180, 0), 0.0f, Vector2(0, 0), Vector4(1, 1, 1, 1));
		BaseScene::mSprite->Draw("SelectAim", selectposition, 0.0f, Vector2(0, 0), Vector4(1, 1, 1, 1));
	}
}



void Defense::Wave1EnemySpown()
{
	enemyDeath = 0;
	//ここの下からadd
	objM->Add(new SniperEnemy(Vector3(0.0f, 0.0f, 450.0f), Vector3(0.0f, 180.0f, 0.0f), objM, BaseScene::mModel, BaseScene::mSprite, BaseScene::mParticle, 0));
	spown1 = true;
}

void Defense::Wave2EnemySpown()
{
	enemyDeath = 0;
	//ここの下からadd
	objM->Add(new SniperEnemy(Vector3(6.0f, 0.0f, 450.0f), Vector3(0.0f, 180.0f, 0.0f), objM, BaseScene::mModel, BaseScene::mSprite, BaseScene::mParticle, 0));
	spown2 = true;
}

void Defense::Wave3EnemySpown()
{
	enemyDeath = 0;
	//ここの下からadd
	spown3 = true;
}

void Defense::Wave1()
{
	
	//テキスト表示
	//インターバル
	
	//敵のadd
	if (!spown1)
	{
		Wave1EnemySpown();
	}
	else
	{
		//場にエネミーが残っているか？

		if (objM->GetEnemy().GetHP() <= 0)
		{
			enemyDeath++;
		}
		if (enemyDeath >= wave1EnemysCount)
		{
			wave1Clear = true;
		}
	}
	
}

void Defense::Wave2()
{
	//テキスト表示
	//インターバル

	//敵のadd
 	if (!spown2)
	{
		Wave2EnemySpown();
	}
	
	////場にエネミーが残っているか？
	if (objM->GetEnemy().GetHP() <= 0)
	{
		enemyDeath++;
	}
	if (enemyDeath >= wave1EnemysCount)
	{
		wave2Clear = true;
	}
}

void Defense::Wave3()
{
	//テキスト表示
	//インターバル

	//敵のadd
	if(!spown3)
	{
		Wave3EnemySpown();
	}

	//ボスを倒した？
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
				NextScene(std::make_shared<GamePlay>());
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