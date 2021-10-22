#include "GamePlay.h"
#include"Title.h"
#include <cstdlib>
#include"Select.h"
#include "../Sound/Sound.h"
#include "../Actor/SniperEnemy.h"
#include "../Actor/BlowEnemy.h"
#include "../Actor/WayPointManager.h"
#include "../Actor/BreadCrumbCreater.h"

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
	//障害物
	/*objM->Add(new Block(Vector3(0.0f, -4.0f, -130.0f), Vector3(0, 0, 0), objM, BaseScene::mModel, 0));
	objM->Add(new Block(Vector3(45.0f, -4.0f, -130.0f), Vector3(0, 0, 0), objM, BaseScene::mModel, 1));
	objM->Add(new Block(Vector3(10.0f, -4.0f, -180.0f), Vector3(0, 0, 0), objM, BaseScene::mModel, 2));
	objM->Add(new Block(Vector3(80.0f, -4.0f, -110.0f), Vector3(0, 0, 0), objM, BaseScene::mModel, 3));
	objM->Add(new Block(Vector3(120.0f, -4.0f, -200.0f), Vector3(0, 0, 0), objM, BaseScene::mModel, 4));
	objM->Add(new Block(Vector3(120.0f, -4.0f, -130.0f), Vector3(0, 0, 0), objM, BaseScene::mModel,5));
	objM->Add(new Block(Vector3(160.0f, -4.0f, -160.0f), Vector3(0, 0, 0), objM, BaseScene::mModel, 6));
	objM->Add(new Block(Vector3(180.0f, -4.0f, -170.0f), Vector3(0, 0, 0), objM, BaseScene::mModel, 7));
	objM->Add(new Block(Vector3(140.0f, -4.0f, -210.0f), Vector3(0, 0, 0), objM, BaseScene::mModel, 8));
	objM->Add(new Block(Vector3(200.0f, -4.0f, -190.0f), Vector3(0, 0, 0), objM, BaseScene::mModel, 9));
	objM->Add(new Block(Vector3(150.0f, -4.0f, -120.0f), Vector3(0, 0, 0), objM, BaseScene::mModel, 10));
	objM->Add(new Block(Vector3(130.0f, -4.0f, -160.0f), Vector3(0, 0, 0), objM, BaseScene::mModel, 11));
*/
//objM->Add(new T_REX(Vector3(60.0f, -4.0f, -250.0f), Vector3(0, 0, 0), objM, BaseScene::mModel, BaseScene::mParticle));
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


	/*30体表示(この数をベースに考える)*/
	int test = 0;
	for (int i = 50; i < 150; i += 20)
	{
		for (int j = 100; j < 220; j += 20)
		{
			auto t = new SniperEnemy(Vector3(i, 0.0f, -j), Vector3(0, 0, 0), objM, BaseScene::mModel, BaseScene::mSprite, BaseScene::mParticle, test++);

			objM->Add(t);

			//objM->Add(new EnemyTank(Vector3(i, 0.0f, -j), Vector3(0, 0, 0), objM, BaseScene::mModel, BaseScene::mParticle, BaseScene::mSprite, test++));
		}
	}

	//objM->Add(new BlowEnemy(Vector3(100.0f, 0.0f, -100.0f), Vector3(0, 0, 0), objM, BaseScene::mModel, BaseScene::mSprite, BaseScene::mParticle, 0));
	//objM->Add(new BlowEnemy(Vector3(150.0f, 0.0f, -100.0f), Vector3(0, 0, 0), objM, BaseScene::mModel, BaseScene::mSprite, BaseScene::mParticle, 1));
	//objM->Add(new BlowEnemy(Vector3(50.0f, 0.0f,  -100.0f), Vector3(0, 0, 0), objM, BaseScene::mModel, BaseScene::mSprite, BaseScene::mParticle, 2));
	//objM->Add(new BlowEnemy(Vector3(200.0f, 0.0f, -100.0f), Vector3(0, 0, 0), objM, BaseScene::mModel, BaseScene::mSprite, BaseScene::mParticle, 3));
	//objM->Add(new BlowEnemy(Vector3( 80.0f, 0.0f, -200.0f), Vector3(0, 0, 0), objM, BaseScene::mModel, BaseScene::mSprite, BaseScene::mParticle, 4));



	//objM->Add(new GolemEnemy(Vector3(100.0f, 0.0f, -120.0f), Vector3(0, 0, 0), objM, BaseScene::mModel, BaseScene::mSprite, BaseScene::mParticle, 0));
	BaseScene::mSprite->AddTexture("Pose", "Resouse/pose.png");
	BaseScene::mSprite->AddTexture("SelectAim", "Resouse/selectcursol.png");
	BaseScene::mSprite->AddTexture("SBack", "Resouse/selectback.png");
	BaseScene::mSprite->AddTexture("OptionP", "Resouse/option.png");
	BaseScene::mSprite->AddTexture("Ritorai", "Resouse/ritorai.png");
	BaseScene::mModel->AddModel("Sora2", "Resouse/skydome.obj", "Resouse/skydome.jpg");
	BaseScene::mModel->AddModel("Ground2", "Resouse/ground.obj", "Resouse/sougen.jpg");

	mSound = std::make_shared<Sound>("boss01.mp3", false);
	//プレイヤーは最後に、又はUIクラスを作る

	objM->Add(new Player(Vector3(0.0f, 0.0f, -50.0f), Vector3(0, 0, 0), objM, BaseScene::mModel, BaseScene::mParticle, BaseScene::mSprite));
}

void GamePlay::UpdateScene()
{
	ImGui::Begin("pose");
	ImGui::Checkbox("selectflag", &pose);
	ImGui::SliderFloat("soundSize", &BaseScene::mMasterSoundVol, 0, 1);
	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	ImGui::End();
	if (Input::KeyDown(DIK_1))
	{
		NextScene(std::make_shared<Title>());
	}

	//パンくずを落とす
	mBreadCreator->DropBreadCrumb();


	Pose();
	Setting();
}

void GamePlay::DrawScene()
{
	DirectXManager::GetInstance()->SetData3D();
	BaseScene::mModel->Draw("Sora2", Vector3(0, 2.0f, -90.0f), Vector3(0, 0, 0), Vector3(7, 7, 7));
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
	}
}


void GamePlay::Pose()
{
	//ポーズ
	if (pose == false && settingFlag == false)
	{
		objM->Update();

		if (Input::KeyDown(DIK_RETURN) || Input::KeyDown(DIK_NUMPADENTER))
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
		if (Input::KeyDown(DIK_S) || Input::pad_data.lY > 0)
		{
			selectposition.y = 360;
			selectposition.x = 500;
		}
		if (Input::KeyDown(DIK_W) || Input::pad_data.lY > 0)
		{
			selectposition.y = 180;
			selectposition.x = 180;
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
	mSound->playLoop();
	mSound->setVol(BaseScene::mMasterSoundVol);
	if (settingFlag == true)
	{
		if (BaseScene::mMasterSoundVol > 1.1f)
		{
			return;
			if (Input::KeyDown(DIK_D) || Input::pad_data.lY > 0)
			{
				BaseScene::mMasterSoundVol += 0.1f;
			}
		}
		if (BaseScene::mMasterSoundVol < 0.0f)
		{
			return;
			if (Input::KeyDown(DIK_A) || Input::pad_data.lY < 0)
			{
				BaseScene::mMasterSoundVol -= 0.1f;
			}
		}

		if (Input::KeyDown(DIK_RETURN) || Input::KeyDown(DIK_NUMPADENTER))
		{
			settingFlag = false;
		}
	}
}
