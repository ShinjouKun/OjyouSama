#include "BossScene.h"
#include"Title.h"
#include <cstdlib>
#include"Select.h"
#include"Result.h"
#include "../Sound/Sound.h"
#include "../Actor/WayPointManager.h"
#include "../Actor/BreadCrumbCreater.h"
#include "../Utility/Timer/Timer.h"
#include"../Actor/CameraEye.h"

#include "../Actor/ElfRock.h"
#include "../Actor/ElfTree.h"
#include "../Actor/Enemy/SniperEnemy.h"
#include "../Actor/Enemy/ElfTreeBoss.h"
#include "../Actor/Enemy/MortarEnemy.h"
#include "../Actor/Enemy/BirdEnemy.h"


#include "../Actor/Block.h"
#include "../Actor/BetaTestBlock.h"


BossScene::BossScene()
	:mSound(nullptr)
{
}

BossScene::~BossScene()
{
	delete mObjManager;//重要
}

void BossScene::StartScene()
{
	mObjManager = new ObjectManager();
	mObjManager->Claer();
	posePos = Vector3(0, 0, 0);
	selectbackPos = Vector3(180, 180, 0);
	selectposition = Vector3(180, 180, 0);
	optionPos = Vector3(180, 180, 0);
	mBossDeadFlag = false;

	//パンくず生成機作成
	mBreadCreator = std::make_shared<BreadCrumbCreater>(mObjManager);
	//WayPoint生成機作成(生成位置,見た目をつけるかどうか)
	mpointManager = std::make_shared<WayPointManager>(Vector3(100.0f, 0.0f, -100.0f), mObjManager, BaseScene::mModel, false);
	//敵AIシステム生成
	mEnemyAI = std::make_shared<EnemyAI>(mpointManager);
	//敵にマネージャーセット
	BaseEnemy::SetObjectManager(mObjManager);
	//敵にパンくずセット
	BaseEnemy::SetBreadCreator(mBreadCreator.get());
	//敵にAIセット
	BaseEnemy::SetEnemyAi(mEnemyAI.get());

	itemHolder = new ItemHolder();
	itemHolder->Init();

#pragma region 画像の読み込み

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

#pragma endregion

	mSound = std::make_shared<Sound>("loop_157.mp3", false);
	mSound->setVol(BaseScene::mMasterSoundVol * BaseScene::mBGMSoundVol);

#pragma region ステージ内モデルの作成

	int objectCount = 0;

	////奥の木
	//for (int i = -20; i <= 20; i += 10)
	//{
	//	float x = static_cast<float>(i);
	//	mObjManager->Add(new ElfTree(Vector3(x, 4.0f, -100.0f), Vector3(0.0f, 90.0f, 0.0f), mObjManager, BaseScene::mModel, objectCount++));
	//}

	////手前の木
	//for (int i = -20; i <= 20; i += 10)
	//{
	//	float x = static_cast<float>(i);
	//	mObjManager->Add(new ElfTree(Vector3(x, 4.0f, 150.0f), Vector3(0.0f, 90.0f, 0.0f), mObjManager, BaseScene::mModel, objectCount++));
	//}

	////左右の木1
	//for (int i = 50; i < 80; i += 5)
	//{
	//	float x = static_cast<float>(i);
	//	mObjManager->Add(new ElfTree(Vector3(-x, 4.0f, 0.0f), Vector3(0.0f, 90.0f, 0.0f), mObjManager, BaseScene::mModel, objectCount++));
	//	mObjManager->Add(new ElfTree(Vector3(x, 4.0f, 0.0f), Vector3(0.0f, 90.0f, 0.0f), mObjManager, BaseScene::mModel, objectCount++));
	//}

	//左右の木1.5
	for (int i = 50; i < 80; i += 5)
	{
		float x = static_cast<float>(i);
		mObjManager->Add(new ElfTree(Vector3(-x, 4.0f, 0.0f), Vector3(0.0f, 90.0f, 0.0f), mObjManager, BaseScene::mModel, objectCount++));
		mObjManager->Add(new ElfTree(Vector3(x, 4.0f, 0.0f), Vector3(0.0f, 90.0f, 0.0f), mObjManager, BaseScene::mModel, objectCount++));
	}

	////左右の木2
	//for (int i = 100; i < 130; i += 5)
	//{
	//	float x = static_cast<float>(i);
	//	mObjManager->Add(new ElfTree(Vector3(-x, 4.0f, -50.0f), Vector3(0.0f, 90.0f, 0.0f), mObjManager, BaseScene::mModel, objectCount++));
	//	mObjManager->Add(new ElfTree(Vector3(x, 4.0f, -50.0f), Vector3(0.0f, 90.0f, 0.0f), mObjManager, BaseScene::mModel, objectCount++));
	//}

	////左右の木3
	//for (int i = 100; i < 130; i += 5)
	//{
	//	float z = static_cast<float>(i);
	//	mObjManager->Add(new ElfTree(Vector3(-80, 4.0f, -z), Vector3(0.0f, 90.0f, 0.0f), mObjManager, BaseScene::mModel, objectCount++));
	//	mObjManager->Add(new ElfTree(Vector3(80, 4.0f, -z), Vector3(0.0f, 90.0f, 0.0f), mObjManager, BaseScene::mModel, objectCount++));
	//}

	//左右の木3.5
	for (int i = 80; i < 120; i += 5)
	{
		float z = static_cast<float>(i);
		mObjManager->Add(new ElfTree(Vector3(-80, 4.0f, -z), Vector3(0.0f, 90.0f, 0.0f), mObjManager, BaseScene::mModel, objectCount++));
		mObjManager->Add(new ElfTree(Vector3(80, 4.0f, -z), Vector3(0.0f, 90.0f, 0.0f), mObjManager, BaseScene::mModel, objectCount++));
	}

	////左右の木4
	//for (int i = 150; i < 180; i += 5)
	//{
	//	float z = static_cast<float>(i);
	//	mObjManager->Add(new ElfTree(Vector3(-50, 4.0f, -z), Vector3(0.0f, 90.0f, 0.0f), mObjManager, BaseScene::mModel, objectCount++));
	//	mObjManager->Add(new ElfTree(Vector3(50, 4.0f, -z), Vector3(0.0f, 90.0f, 0.0f), mObjManager, BaseScene::mModel, objectCount++));
	//}

	//奥の木
	for (int i = -10; i <= 10; i += 5)
	{
		float x = static_cast<float>(i);
		mObjManager->Add(new ElfTree(Vector3(x, 4.0f, -180.0f), Vector3(0.0f, 90.0f, 0.0f), mObjManager, BaseScene::mModel, objectCount++));
	}

	////入口の横岩
	//for (int i = -150; i < -20; i += 10)
	//{
	//	float x = static_cast<float>(i);
	//	mObjManager->Add(new ElfRock(Vector3(x, 4.0f, 50.0f), Vector3(0.0f, 0.0f, 0.0f), mObjManager, BaseScene::mModel, objectCount++, 3));
	//	mObjManager->Add(new ElfRock(Vector3(-x, 4.0f, 50.0f), Vector3(0.0f, 0.0f, 0.0f), mObjManager, BaseScene::mModel, objectCount++, 3));
	//}

	//入口の縦岩
	for (int i = 50; i <= 80; i += 10)
	{
		float z = static_cast<float>(i);
		mObjManager->Add(new ElfRock(Vector3(+20, 4.0f, z), Vector3(0, 90.0f, 0), mObjManager, BaseScene::mModel, objectCount++, 3));
		mObjManager->Add(new ElfRock(Vector3(-20, 4.0f, z), Vector3(0, 90.0f, 0), mObjManager, BaseScene::mModel, objectCount++, 3));
	}

	//岩投げの敵を守る岩Right
	mObjManager->Add(new ElfRock(Vector3(+120, 4.0f, -170.0f), Vector3(0, 0.0f, 0), mObjManager, BaseScene::mModel, objectCount++, 3));
	mObjManager->Add(new ElfRock(Vector3(+110, 4.0f, -170.0f), Vector3(0, 0.0f, 0), mObjManager, BaseScene::mModel, objectCount++, 3));
	mObjManager->Add(new ElfRock(Vector3(+100, 4.0f, -170.0f), Vector3(0, 0.0f, 0), mObjManager, BaseScene::mModel, objectCount++, 3));
	mObjManager->Add(new ElfRock(Vector3(+90, 4.0f, -170.0f), Vector3(0, 0.0f, 0), mObjManager, BaseScene::mModel, objectCount++, 3));
	mObjManager->Add(new ElfRock(Vector3(+130, 4.0f, -180.0f), Vector3(0, 90.0f, 0), mObjManager, BaseScene::mModel, objectCount++, 3));
	mObjManager->Add(new ElfRock(Vector3(+70, 4.0f, -180.0f), Vector3(0, 90.0f, 0), mObjManager, BaseScene::mModel, objectCount++, 3));

	//岩投げの敵を守る岩Left
	mObjManager->Add(new ElfRock(Vector3(-120, 4.0f, -170.0f), Vector3(0, 0.0f, 0), mObjManager, BaseScene::mModel, objectCount++, 3));
	mObjManager->Add(new ElfRock(Vector3(-110, 4.0f, -170.0f), Vector3(0, 0.0f, 0), mObjManager, BaseScene::mModel, objectCount++, 3));
	mObjManager->Add(new ElfRock(Vector3(-100, 4.0f, -170.0f), Vector3(0, 0.0f, 0), mObjManager, BaseScene::mModel, objectCount++, 3));
	mObjManager->Add(new ElfRock(Vector3(-90, 4.0f, -170.0f), Vector3(0, 0.0f, 0), mObjManager, BaseScene::mModel, objectCount++, 3));
	mObjManager->Add(new ElfRock(Vector3(-130, 4.0f, -180.0f), Vector3(0, 90.0f, 0), mObjManager, BaseScene::mModel, objectCount++, 3));
	mObjManager->Add(new ElfRock(Vector3(-70, 4.0f, -180.0f), Vector3(0, 90.0f, 0), mObjManager, BaseScene::mModel, objectCount++, 3));


	//スタート地点の天井
	mObjManager->Add(new BetaTestBlock(Vector3(0, 15.0f, 65.0f), Vector3(180, 0, 0), mObjManager, BaseScene::mModel, objectCount, Vector3(-10.0f, 0.0f, -15.0f), Vector3(10.0f, 1.0f, 15.0f)));



	//後ろの壁
	mObjManager->Add(new BetaTestBlock(Vector3(0, 0.0f, 80.0f), Vector3(90, 0, 0), mObjManager, BaseScene::mModel, objectCount++, Vector3(-150.0f, 0.0f, -2.0f), Vector3(150.0f, 1.0f, 1.0f)));
	//奥
	mObjManager->Add(new BetaTestBlock(Vector3(0, 0.0f, -250.0f), Vector3(90, 180, 0), mObjManager, BaseScene::mModel, objectCount++, Vector3(-150.0f, 0.0f, -2.0f), Vector3(150.0f, 1.0f, 1.0f)));
	//右
	mObjManager->Add(new BetaTestBlock(Vector3(150, 0.0f, 0.0f), Vector3(90, -90, 0), mObjManager, BaseScene::mModel, objectCount++, Vector3(-2.0f, 0.0f, -150.0f), Vector3(1.0f, 1.0f, 150.0f)));
	//左
	mObjManager->Add(new BetaTestBlock(Vector3(-150, 0.0f, 0.0f), Vector3(90, 90, 0), mObjManager, BaseScene::mModel, objectCount++, Vector3(-2.0f, 0.0f, -150.0f), Vector3(1.0f, 1.0f, 150.0f)));

	////岩投げの敵
	//mObjManager->Add(new MortarEnemy(Vector3(+100.0f, 0.0f, -200.0f), Vector3(0.0f, 180.0f, 0.0f), mObjManager, BaseScene::mModel, BaseScene::mParticle, objectCount++));
	//mObjManager->Add(new MortarEnemy(Vector3(-100.0f, 0.0f, -200.0f), Vector3(0.0f, 180.0f, 0.0f), mObjManager, BaseScene::mModel, BaseScene::mParticle, objectCount++));

	//mObjManager->Add(new SniperEnemy(Vector3(0,0,-50), Vector3(0,0,0), mObjManager, BaseScene::mModel, BaseScene::mSprite, BaseScene::mParticle, objectCount++));

	mObjManager->Add(new BirdEnemy(Vector3(100, 0.0f, -200.0f), Vector3(0.0f, 180.0f, 0.0f), mObjManager, BaseScene::mModel, BaseScene::mParticle, objectCount++));


	//mObjManager->Add(new Block(Vector3(0, 0, -50), Vector3(0, 0, 0), mObjManager, BaseScene::mModel, objectCount++));

	////ボス
	mBoss = new ElfTreeBoss(Vector3(0.0f, 0.0f, -100.0f), Vector3(0.0f, 180.0f, 0.0f), mObjManager, BaseScene::mModel, BaseScene::mParticle, objectCount++);
	mObjManager->Add(mBoss);

#pragma endregion

	mObjManager->Add(new Player(Vector3(0.0f, 0.0f, 70.0f), Vector3(0, 0, 0), mObjManager, BaseScene::mModel, BaseScene::mParticle, BaseScene::mSprite));
	mObjManager->Add(new CameraEye(Vector3(0, 0, 180), Vector3(0, 0, 0), mObjManager));
	mTimer = std::make_shared<Timer>(0.01f);
}

void BossScene::UpdateScene()
{
	mSound->playLoop();
	//ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	mTimer->update();
	if (!mTimer->isTime()) return;
	Pose();
	Setting();

	if (mBoss->GetDeadFlag())
	{
		mBossDeadFlag = true;
		NextScene(std::make_shared<Result>());
	}

	if (mObjManager->GetPlayer().GetHp() <= 0)
	{
		if (mBossDeadFlag) return;
		NextScene(std::make_shared<Title>());
	}
}

void BossScene::DrawScene()
{
	DirectXManager::GetInstance()->SetData3D();
	BaseScene::mModel->Draw("Sora2", Vector3(0, 2.0f, 0.0f), Vector3(0, 0, 0), Vector3(50, 50, 50));
	//BaseScene::mModel->Draw("Ground2", Vector3(-20.0f, 0.0f, -90.0f), Vector3(0, 0, 0), Vector3(500, 500, 500));
	mObjManager->Draw();
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

void BossScene::Pose()
{

	{
		//ポーズ
		if (pose == false && settingFlag == false)
		{
			mObjManager->Update();
			if (Input::KeyDown(DIK_E) || Input::KeyDown(BUTTON_A))
			{
				itemHolder->UseItem(ItemNames::heal);
			}

			if (Input::KeyDown(DIK_RETURN) || Input::pad_data.rgbButtons[11])
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
			if (Input::KeyDown(DIK_A) || Input::pad_data.lX < 0)
			{
				if (selectposition.y == 360)
				{
					return;
				}
				selectposition.x -= 640;
				mTimer->setTime(0.2f);
			}

			if (Input::KeyDown(DIK_D) || Input::pad_data.lX > 0)
			{
				if (selectposition.y == 360)
				{
					return;
				}
				selectposition.x += 640;
				mTimer->setTime(0.2f);
			}
			if (Input::KeyDown(DIK_S) || Input::pad_data.lY > 0)
			{
				selectposition.y = 360;
				selectposition.x = 500;
				mTimer->setTime(0.2f);
			}
			if (Input::KeyDown(DIK_W) || Input::pad_data.lY < 0)
			{
				selectposition.y = 180;
				selectposition.x = 180;
				mTimer->setTime(0.2f);
			}
			if (selectposition.x == 180)
			{
				if (Input::KeyDown(DIK_SPACE) || Input::pad_data.rgbButtons[2])
				{
					NextScene(std::make_shared<Result>());
				}
			}
			if (selectposition.x == 820)
			{
				if (Input::KeyDown(DIK_SPACE) || Input::pad_data.rgbButtons[2])
				{
					settingFlag = true;
					pose = false;
				}
			}
			if (selectposition.y == 360 && selectposition.x == 500)
			{
				if (Input::KeyDown(DIK_SPACE) || Input::pad_data.rgbButtons[2])
				{
					NextScene(std::make_shared<GamePlay>());
				}
			}
			if (Input::KeyDown(DIK_RETURN) || Input::KeyDown(DIK_NUMPADENTER))
			{
				pose = false;
			}
		}
	}
}

void BossScene::Setting()
{

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
			if (Input::KeyDown(DIK_S) || Input::pad_data.lY > 0)
			{
				optionPos += Vector3(0, 180.0f, 0);
				mTimer->setTime(0.2f);
			}
			if (Input::KeyDown(DIK_W) || Input::pad_data.lY < 0)
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
				if (Input::KeyDown(DIK_D) || Input::pad_data.lX > 0)
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
				if (Input::KeyDown(DIK_A) || Input::pad_data.lX < 0)
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
				if (Input::KeyDown(DIK_D) || Input::pad_data.lX > 0)
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
				if (Input::KeyDown(DIK_A) || Input::pad_data.lX < 0)
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
				if (Input::KeyDown(DIK_D) || Input::pad_data.lX > 0)
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
				if (Input::KeyDown(DIK_A) || Input::pad_data.lX < 0)
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
			if (Input::KeyDown(DIK_RETURN) || Input::KeyDown(DIK_NUMPADENTER))
			{
				settingFlag = false;
			}

			mSound->setVol(BaseScene::mMasterSoundVol * BaseScene::mBGMSoundVol);
		}
	}
}

void BossScene::ResultF()
{

	{

		if (resultFlag == false)
		{
			if (mObjManager->GetGolem().GetHp() <= 0)
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
				if (Input::KeyDown(DIK_A) || Input::pad_data.lY < 0)
				{
					if (selectposition.y == 360)
					{
						return;
					}
					selectposition.x -= 640;
				}

				if (Input::KeyDown(DIK_D) || Input::pad_data.lY > 0)
				{
					if (selectposition.y == 360)
					{
						return;
					}
					selectposition.x += 640;
				}

				if (selectposition.x == 180)
				{
					if (Input::KeyDown(DIK_SPACE) || Input::pad_data.rgbButtons[2])
					{
						NextScene(std::make_shared<Select>());
					}
				}
				if (selectposition.x == 820)
				{
					if (Input::KeyDown(DIK_SPACE) || Input::pad_data.rgbButtons[2])
					{
						NextScene(std::make_shared<GamePlay>());
					}
				}
			}
		}
	}
}
