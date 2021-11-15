#include "GamePlay.h"
#include"Title.h"
#include <cstdlib>
#include"Select.h"
#include"Result.h"
#include "../Sound/Sound.h"
#include "../Actor/SniperEnemy.h"
#include "../Actor/BlowEnemy.h"
#include "../Actor/WayPointManager.h"
#include "../Actor/BreadCrumbCreater.h"
#include "../Utility/Timer/Timer.h"
#include"../Actor/CameraEye.h"
#include "../Actor/ElfTree.h"
#include "../Actor/ElfRock.h"

#include "../Actor/CEnemy.h"
#include "../Actor/MortarEnemy.h"

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
	objM = new ObjectManager();
	objM->Claer();
	posePos = Vector3(0, 0, 0);
	selectbackPos = Vector3(180, 180, 0);
	selectposition = Vector3(180, 180, 0);
	optionPos = Vector3(180, 180, 0);
	//障害物

//int s = 0;
//float x_dis = 10.0f;
//float z_dis = 0.0f;
//for (int i = 0; i <= 49; i++)
//{
//	if (s >= 5)
//	{
//		z_dis += 50.0f;
//		x_dis = 0.0f;
//		s = 0;
//	}
//  objM->Add(new EnemyTank(Vector3(90.0f+(s*x_dis), 0.0f, -800.0f + z_dis), Vector3(0, 0, 0), objM, BaseScene::mModel, BaseScene::mParticle, BaseScene::mSprite, i));
//  //objM->Add(new TestEnemy(Vector3(90.0f + (s*x_dis), 0.0f, -800.0f + z_dis), Vector3(0, 0, 0), objM, BaseScene::mModel, BaseScene::mSprite, i));
//  x_dis = 10.0f;
//  s++;
//}

	//パンくず生成機作成
	//mBreadCreator = new BreadCrumbCreater(objM);
	mBreadCreator = std::make_shared<BreadCrumbCreater>(objM);
	//WayPoint生成機作成(生成位置,見た目をつけるかどうか)
	//mpointManager = new WayPointManager(Vector3(100.0f, 0.0f, -100.0f), objM, BaseScene::mModel,false);
	mpointManager = std::make_shared<WayPointManager>(Vector3(100.0f, 0.0f, -100.0f), objM, BaseScene::mModel, false);
	//敵AIシステム生成
	mEnemyAI = std::make_shared<EnemyAI>(mpointManager);


	////マネージャーセット
	//mEnemyAI->SetWayPointManager(mpointManager.get());

	//敵にマネージャーセット
	BaseEnemy::SetObjectManager(objM);
	//敵にパンくずセット
	BaseEnemy::SetBreadCreator(mBreadCreator.get());
	//敵にAIセット
	BaseEnemy::SetEnemyAi(mEnemyAI.get());

#pragma region オブジェクト生成

	int objectCount = 0;

	///*スタート地点の木*/
	for (int i = 130; i < 210; i += 20)
	{
		objM->Add(new ElfTree(Vector3(20, 4.0f, i), Vector3(0.0f, 90.0f, 0.0f), objM, BaseScene::mModel, objectCount++));
		objM->Add(new ElfTree(Vector3(-20, 4.0f, i), Vector3(0.0f, 90.0f, 0.0f), objM, BaseScene::mModel, objectCount++));
	}

	///*中央の木*/
	//for (int i = 40; i < 100; i += 20)
	//{
	//	objM->Add(new ElfTree(Vector3(i, 0.0f, 0), Vector3(0.0f, 90.0f, 0.0f), objM, BaseScene::mModel, objectCount++));
	//	objM->Add(new ElfTree(Vector3(-i, 0.0f, 0), Vector3(0.0f, 90.0f, 0.0f), objM, BaseScene::mModel, objectCount++));
	//}

	///*中央奥の木*/
	//for (int i = -50; i < 51; i += 25)
	//{
	//	objM->Add(new ElfTree(Vector3(i, 0.0f, -80), Vector3(0.0f, 90.0f, 0.0f), objM, BaseScene::mModel, objectCount++));
	//}

	///*中央の岩*/
	objM->Add(new ElfRock(Vector3(0.0f, 4.0f, 0.0f), Vector3().zero, objM, BaseScene::mModel, objectCount++, 2));
	objM->Add(new ElfRock(Vector3(+5.0f, 4.0f, 0.0f), Vector3().zero, objM, BaseScene::mModel, objectCount++, 2));
	objM->Add(new ElfRock(Vector3(-5.0f, 4.0f, 0.0f), Vector3().zero, objM, BaseScene::mModel, objectCount++, 2));
	objM->Add(new ElfRock(Vector3(+10.0f, 4.0f, 5.0f), Vector3(0.0f, +45.0f, 0.0f), objM, BaseScene::mModel, objectCount++, 2));
	objM->Add(new ElfRock(Vector3(-10.0f, 4.0f, 5.0f), Vector3(0.0f, -45.0f, 0.0f), objM, BaseScene::mModel, objectCount++, 2));

	///*左側の岩*/
	//objM->Add(new ElfRock(Vector3(100.0f, 0.0f, 100.0f), Vector3(0.0f, -45.0f, 0.0f), objM, BaseScene::mModel, objectCount++, 2));
	///*右側の岩*/
	//objM->Add(new ElfRock(Vector3(-100.0f, 0.0f, 100.0f), Vector3(0.0f, -45.0f, 0.0f), objM, BaseScene::mModel, objectCount++, 2));

#pragma endregion

#pragma region 敵の生成

	int enemyCount = 0;

	/////*遠距離攻撃の敵*/
	//objM->Add(new SniperEnemy(Vector3(0.0f, 4.0f, 10.0f), Vector3(0.0f, 180.0f, 0.0f), objM, BaseScene::mModel, BaseScene::mSprite, BaseScene::mParticle, enemyCount++));
	//objM->Add(new SniperEnemy(Vector3(+10.0f, 4.0f, 20.0f), Vector3(0.0f, 225.0f, 0.0f), objM, BaseScene::mModel, BaseScene::mSprite, BaseScene::mParticle, enemyCount++));
	//objM->Add(new SniperEnemy(Vector3(-10.0f, 4.0f, 20.0f), Vector3(0.0f, 135.0f, 0.0f), objM, BaseScene::mModel, BaseScene::mSprite, BaseScene::mParticle, enemyCount++));
	//objM->Add(new SniperEnemy(Vector3(+50.0f, 4.0f, 50.0f), Vector3(0.0f, 90.0f, 0.0f), objM, BaseScene::mModel, BaseScene::mSprite, BaseScene::mParticle, enemyCount++));
	//objM->Add(new SniperEnemy(Vector3(-50.0f, 4.0f, 50.0f), Vector3(0.0f, 270.0f, 0.0f), objM, BaseScene::mModel, BaseScene::mSprite, BaseScene::mParticle, enemyCount++));
	/*中央奥の木*/
	/*for (int i = -35; i < 50; i += 25)
	{
		objM->Add(new SniperEnemy(Vector3(i, 4.0f, -75.0f), Vector3(0.0f, 180.0f, 0.0f), objM, BaseScene::mModel, BaseScene::mSprite, BaseScene::mParticle, enemyCount++));
	}
*/

	///*近距離攻撃の敵*/
 //   objM->Add(new BlowEnemy(Vector3(+90.0f, 0.0f, 100.0f), Vector3(0.0f, 90.0f, 0.0f), objM, BaseScene::mModel, BaseScene::mSprite, BaseScene::mParticle, enemyCount++));
	//objM->Add(new BlowEnemy(Vector3(100.0f, 0.0f, 110.0f), Vector3(0.0f, 180.0f, 0.0f), objM, BaseScene::mModel, BaseScene::mSprite, BaseScene::mParticle, enemyCount++));
	//objM->Add(new BlowEnemy(Vector3(-90.0f, 0.0f, 100.0f), Vector3(0.0f, 90.0f, 0.0f), objM, BaseScene::mModel, BaseScene::mSprite, BaseScene::mParticle, enemyCount++));
	//objM->Add(new BlowEnemy(Vector3(-100.0f, 0.0f, 110.0f), Vector3(0.0f, 180.0f, 0.0f), objM, BaseScene::mModel, BaseScene::mSprite, BaseScene::mParticle, enemyCount++));
	//objM->Add(new BlowEnemy(Vector3(-70.0f, 0.0f, 30.0f), Vector3(0.0f, 90.0f, 0.0f), objM, BaseScene::mModel, BaseScene::mSprite, BaseScene::mParticle, enemyCount++));
	//objM->Add(new BlowEnemy(Vector3(-70.0f, 0.0f, 50.0f), Vector3(0.0f, 90.0f, 0.0f), objM, BaseScene::mModel, BaseScene::mSprite, BaseScene::mParticle, enemyCount++));
	//objM->Add(new BlowEnemy(Vector3(+70.0f, 0.0f, 30.0f), Vector3(0.0f, 180.0f, 0.0f), objM, BaseScene::mModel, BaseScene::mSprite, BaseScene::mParticle, enemyCount++));
	//objM->Add(new BlowEnemy(Vector3(+70.0f, 0.0f, 50.0f), Vector3(0.0f, 180.0f, 0.0f), objM, BaseScene::mModel, BaseScene::mSprite, BaseScene::mParticle, enemyCount++));

#pragma endregion

	itemHolder = new ItemHolder();
	itemHolder->Init();


	/*30体表示(この数をベースに考える)*/
	int test = 0;
	//for (int i = 50; i < 150; i += 20)
	//{
	//	for (int j = 100; j < 220; j += 20)
	//	{
	//		auto t = new SniperEnemy(Vector3(i, 0.0f, -j), Vector3(0, 0, 0), objM, BaseScene::mModel, BaseScene::mSprite, BaseScene::mParticle, test++);

	//		objM->Add(t);

	//		//objM->Add(new EnemyTank(Vector3(i, 0.0f, -j), Vector3(0, 0, 0), objM, BaseScene::mModel, BaseScene::mParticle, BaseScene::mSprite, test++));
	//	}
	//}


	/*auto t = new SniperEnemy(Vector3(0.0f, 0.0f, 50.0f), Vector3(0, 180.0f, 0), objM, BaseScene::mModel, BaseScene::mSprite, BaseScene::mParticle, test++);
	mEnemyAI->AddEnemyList(t);
	objM->Add(t);*/

	//objM->Add(new CEnemy(Vector3(0.0f, 0.0f, 0.0f), Vector3(0.0f, 180.0f, 0.0f), objM, BaseScene::mModel, BaseScene::mParticle, test++));
	//objM->Add(new MortarEnemy(Vector3(0.0f, 0.0f, 0.0f), Vector3(0.0f, 180.0f, 0.0f), objM, BaseScene::mModel, BaseScene::mParticle, test++));

	//objM->Add(new BlowEnemy(Vector3(100.0f, 0.0f, -100.0f), Vector3(0, 0, 0), objM, BaseScene::mModel, BaseScene::mSprite, BaseScene::mParticle, 0));
	//objM->Add(new BlowEnemy(Vector3(150.0f, 0.0f, -100.0f), Vector3(0, 0, 0), objM, BaseScene::mModel, BaseScene::mSprite, BaseScene::mParticle, 1));
	//objM->Add(new BlowEnemy(Vector3(50.0f, 0.0f,  -100.0f), Vector3(0, 0, 0), objM, BaseScene::mModel, BaseScene::mSprite, BaseScene::mParticle, 2));
	//objM->Add(new BlowEnemy(Vector3(200.0f, 0.0f, -100.0f), Vector3(0, 0, 0), objM, BaseScene::mModel, BaseScene::mSprite, BaseScene::mParticle, 3));
	//objM->Add(new BlowEnemy(Vector3( 80.0f, 0.0f, -200.0f), Vector3(0, 0, 0), objM, BaseScene::mModel, BaseScene::mSprite, BaseScene::mParticle, 4));


	objM->Add(new GolemEnemy(Vector3(0.0f, 4.0f, -150.0f), Vector3(0, 0, 0), objM, BaseScene::mModel, BaseScene::mSprite, BaseScene::mParticle, 0));
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
	BaseScene::mModel->AddModel("Ground2", "Resouse/ground.obj", "Resouse/sougen.png");
	mSound = std::make_shared<Sound>("loop_157.mp3", false);
	mSound->setVol(BaseScene::mMasterSoundVol * BaseScene::mBGMSoundVol);
	//プレイヤーは最後に、又はUIクラスを作る

	objM->Add(new Player(Vector3(0.0f, 0.0f, 180.0f), Vector3(0, 0, 0), objM, BaseScene::mModel, BaseScene::mParticle, BaseScene::mSprite));
	objM->Add(new CameraEye(Vector3(0, 0, 180), Vector3(0, 0, 0), objM));
	objM->Add(new Repair(Vector3(50.0f, 0.0f, -100.0f), Vector3(0, 0, 0), objM, BaseScene::mModel, itemHolder, ItemState::Normal, 0, 500, 20));

	mTimer = std::make_shared<Timer>(0.01f);
}

void GamePlay::UpdateScene()
{
	mSound->playLoop();
	ImGui::Begin("pose");
	ImGui::Checkbox("selectflag", &pose);
	ImGui::SliderFloat("soundSize", &BaseScene::mMasterSoundVol, 0, 1);
	ImGui::SliderFloat("soundSize", &BaseScene::mBGMSoundVol, 0, 1);
	ImGui::SliderFloat("soundSize", &BaseScene::mSESoundVol, 0, 1);
	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	ImGui::End();
	if (Input::KeyDown(DIK_1))
	{

		NextScene(std::make_shared<Title>());
	}

	//パンくずを落とす
	mBreadCreator->DropBreadCrumb();
	mEnemyAI->Update();


	mTimer->update();
	if (!mTimer->isTime()) return;
	Pose();
	Setting();
	if (objM->GetGolem().GetHp() <= 0)
	{
		BaseScene::mMoney += 20000000;
		NextScene(std::make_shared<Result>());
	}
	if (objM->GetPlayer().GetHp() <= 0)
	{
		NextScene(std::make_shared<Title>());
	}
}

void GamePlay::DrawScene()
{
	DirectXManager::GetInstance()->SetData3D();
	BaseScene::mModel->Draw("Sora2", Vector3(0, 2.0f, 0.0f), Vector3(0, 0, 0), Vector3(50, 50, 50));
	BaseScene::mModel->Draw("Ground2", Vector3(-20.0f, 0.0f, -90.0f), Vector3(0, 0, 0), Vector3(15, 15, 15));
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


void GamePlay::Pose()
{
	//ポーズ
	if (pose == false && settingFlag == false)
	{
		objM->Update();
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
			mTimer->setTime(0.5f);
		}

		if (Input::KeyDown(DIK_D) || Input::pad_data.lX > 0)
		{
			if (selectposition.y == 360)
			{
				return;
			}
			selectposition.x += 640;
			mTimer->setTime(0.5f);
		}
		if (Input::KeyDown(DIK_S) || Input::pad_data.lY > 0)
		{
			selectposition.y = 360;
			selectposition.x = 500;
			mTimer->setTime(0.5f);
		}
		if (Input::KeyDown(DIK_W) || Input::pad_data.lY < 0)
		{
			selectposition.y = 180;
			selectposition.x = 180;
			mTimer->setTime(0.5f);
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
		if (Input::KeyDown(DIK_S) || Input::pad_data.lY > 0)
		{
			optionPos += Vector3(0, 180.0f, 0);
			mTimer->setTime(0.5f);
		}
		if (Input::KeyDown(DIK_W) || Input::pad_data.lY < 0)
		{
			optionPos -= Vector3(0, 180.0f, 0);
			mTimer->setTime(0.5f);
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
				mTimer->setTime(0.5f);
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
				mTimer->setTime(0.5f);
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
				mTimer->setTime(0.5f);
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
				mTimer->setTime(0.5f);
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
				mTimer->setTime(0.5f);
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
				mTimer->setTime(0.5f);
			}
		}
		if (Input::KeyDown(DIK_RETURN) || Input::KeyDown(DIK_NUMPADENTER))
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
