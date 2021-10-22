#include "Result.h"
#include "GamePlay.h"
#include"Title.h"
#include"Select.h"

Result::Result()
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
	selectbackPos = Vector3(180, 180, 0);
	selectposition = Vector3(180, 180, 0);
	camerapos = Vector3(-80,0,15);
	setcamerapos = Vector3(0,16,0);
	BaseScene::mModel->AddModel("TankPlayerA", "Resouse/head.obj", "Resouse/sensha.png");
	BaseScene::mModel->AddModel("TankPlayerB", "Resouse/body.obj", "Resouse/sensha.png");
}

void Result::UpdateScene()
{
	camera->GetEye();
	camera->GetTarget();
	camera->SetEye(camerapos);
	camera->SetTarget(setcamerapos);
	//camerapos.x -=1;
	time += 1;
	if (time >= 300)
	{
		//camerapos.x += 1;
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
	BaseScene::mModel->Draw("Sora2", Vector3(0, 2.0f, -90.0f), Vector3(0, 0, 0), Vector3(7, 7, 7));
	BaseScene::mModel->Draw("Ground2", Vector3(-20.0f, 0.0f, -90.0f), Vector3(0, 0, 0), Vector3(15, 15, 15));
	objM->Draw();
	DirectXManager::GetInstance()->SetData2D();
	if (time >= 300)
	{
		BaseScene::mSprite->Draw("Pose", posePos, 0.0f, Vector2(0.25f, 0.5f), Vector4(1, 1, 1, 1));
		BaseScene::mSprite->Draw("SBack", selectbackPos, 0.0f, Vector2(0, 0), Vector4(1, 1, 1, 1));
		BaseScene::mSprite->Draw("Ritorai", Vector3(820, 180, 0), 0.0f, Vector2(0, 0), Vector4(1, 1, 1, 1));
		BaseScene::mSprite->Draw("SelectAim", selectposition, 0.0f, Vector2(0, 0), Vector4(1, 1, 1, 1));
	}
}
