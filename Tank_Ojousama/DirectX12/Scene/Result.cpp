#include "Result.h"
#include "GamePlay.h"
#include"Title.h"
#include"Select.h"
#include "../Sound/Sound.h"
#include "../Utility/Timer/Timer.h"

Result::Result()
	:mSound(nullptr)
{
}

Result::~Result()
{
	delete objM;
}

void Result::StartScene()
{
	objM = new ObjectManager();
	objM->Claer();
	posePos = Vector3(0, 0, 0);
	BaseScene::mSprite->AddTexture("Pose", "Resouse/pose.png");
	BaseScene::mSprite->AddTexture("SelectAim", "Resouse/selectcursol.png");
	BaseScene::mSprite->AddTexture("SBack", "Resouse/selectback.png");
	BaseScene::mSprite->AddTexture("OptionP", "Resouse/option.png");
	BaseScene::mSprite->AddTexture("Ritorai", "Resouse/ritorai.png");
	BaseScene::mSprite->AddTexture("Kakutoku", "Resouse/kakutoku.png");
	BaseScene::mModel->AddModel("Sora2", "Resouse/skydome.obj", "Resouse/skydome.jpg");
	BaseScene::mModel->AddModel("Ground2", "Resouse/ground.obj", "Resouse/sougen.png");
	selectbackPos = Vector3(180, 180, 0);
	selectposition = Vector3(180, 180, 0);
	camerapos = Vector3(0,5,10);
	setcamerapos = Vector3(0,5,0);
	BaseScene::mModel->AddModel("TankPlayerA", "Resouse/houtou.obj", "Resouse/sensha_A.png");
	BaseScene::mModel->AddModel("TankPlayerB", "Resouse/sensha_body.obj", "Resouse/sensha_A.png");
	BaseScene::mModel->AddModel("ArmR", "Resouse/R_hands.obj", "Resouse/hands_one.png");
	BaseScene::mModel->SetAncPoint("ArmR", Vector3(0.0f, -2.1f, -0.1f));
	BaseScene::mModel->AddModel("OjyouSama", "Resouse/ojosama_body.obj", "Resouse/ojosama_one.png");
	BaseScene::mModel->SetAncPoint("OjyouSama", Vector3(0.0f, 0.0f, -0.1f));
	BaseScene::mModel->AddModel("ArmL", "Resouse/L_hands.obj", "Resouse/hands_one.png");
	BaseScene::mModel->SetAncPoint("ArmL", Vector3(0.0f, -2.1f, -0.1f));
	mSound = std::make_shared<Sound>("loop_157.mp3", false);

	speed = 0;
	ojyouY = 0.0f;
	ojyouZ = 0.0f;
	ojyouXR = 7;
	ojyouXL = 7;
	mTimer = std::make_shared<Timer>(0.01f);
}

void Result::UpdateScene()
{
	mTimer->update();
	speed += 1;
	mSound->playLoop();
	mSound->setVol(BaseScene::mBGMSoundVol);
	ojyouY = ojyouY + speed;
	camera->SetEye(camerapos);
	camera->SetTarget(setcamerapos);
	camerapos.z -=0.15;
	camerapos.x -= 0.05;
	camerapos.y -= 0.01;
	time += 1;

	if (time >= 120)
	{
		camerapos.z += 0.15;
		camerapos.x += 0.05;
		camerapos.y += 0.01;
		if (selectposition.x <= 0)
		{
			selectposition.x = 820;
		}
		if (selectposition.x > 820)
		{
			selectposition.x = 180;
		}

		if (!mTimer->isTime()) return;
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
	if (Input::KeyDown(DIK_1))
	{
		//BaseScene::mMoney += 20000000;
		NextScene(std::make_shared<Title>());
	}
}

void Result::DrawScene()
{


	DirectXManager::GetInstance()->SetData3D();
	BaseScene::mModel->Draw("TankPlayerA", Vector3(0,0,0), Vector3(0, 0, 0), Vector3(1.5f, 1.5f, 1.5f));
	//playerModel->Draw("TankPlayerHou", Vector3(position.x, position.y, position.z), Vector3(fireAngle, -atkAngle, 0), Vector3(1.5f, 1.5f, 1.5f));
	BaseScene::mModel->Draw("TankPlayerB", Vector3(0,0,0 ), Vector3(0, 0, 0), Vector3(1.5f, 1.5f, 1.5f));
	BaseScene::mModel->Draw("ArmR", Vector3(0,3.2f + ojyouZ,0 ), Vector3(150, ojyouY, 0), Vector3(1.5f, 1.5f, 1.5f));
	BaseScene::mModel->Draw("OjyouSama", Vector3(0,0 + ojyouZ,0 ), Vector3(0, ojyouY, 0), Vector3(1.5f, 1.5f, 1.5f));
	BaseScene::mModel->Draw("ArmL", Vector3(0,3.2f + ojyouZ,0 ), Vector3(150, ojyouY, 0), Vector3(1.5f, 1.5f, 1.5f));
	BaseScene::mModel->Draw("Sora2", Vector3(0, 2.0f, -90.0f), Vector3(0, 0, 0), Vector3(7, 7, 7));
	BaseScene::mModel->Draw("Ground2", Vector3(-20.0f, 0.0f, -90.0f), Vector3(0, 0, 0), Vector3(15, 15, 15));
	objM->Draw();
	DirectXManager::GetInstance()->SetData2D();
	if (time >= 120)
	{
		ojyouZ += 0.1f;
		BaseScene::mSprite->Draw("Pose", posePos, 0.0f, Vector2(0.25f, 0.5f), Vector4(1, 1, 1, 1));
		BaseScene::mSprite->Draw("SBack", selectbackPos, 0.0f, Vector2(0, 0), Vector4(1, 1, 1, 1));
		BaseScene::mSprite->Draw("Ritorai", Vector3(820, 180, 0), 0.0f, Vector2(0, 0), Vector4(1, 1, 1, 1));
		BaseScene::mSprite->Draw("SelectAim", selectposition, 0.0f, Vector2(0, 0), Vector4(1, 1, 1, 1));
		BaseScene::mSprite->Draw("Kakutoku", Vector3(0,0,0), 0.0f, Vector2(0, 0), Vector4(1, 1, 1, 1));
	}
}
