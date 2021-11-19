#include "Title.h"
#include"Select.h"
//#include"Collision.h"
#include "../Collision/Collision.h"
#include "../Sound/Sound.h"

Title::Title():
	mSound(nullptr)
{

}

Title::~Title()
{
	
}

void Title::StartScene()
{
	fade1 = 1;
	fade2 = 0;
	fadeF = false;
	ojyouY = 0.0f;
	ojyouXR = 0.0f;
	ojyouXL = 0.0f;
	camera->SetEye(Vector3(0.0f,0 ,-122));
	camera->SetTarget(Vector3(0.0f, 0,-100.0f));
	ojoP = Vector3(0, -25, -113);
	BaseScene::mSprite->AddTexture("Title", "Resouse/title_rogo.png");
	BaseScene::mSprite->AddTexture("Push", "Resouse/start.png");
	BaseScene::mSprite->AddTexture("Heart", "Resouse/heart.png");
	BaseScene::mSprite->AddTexture("Fade1", "Resouse/fade.png");
	BaseScene::mSprite->AddTexture("Fade2", "Resouse/fade.png");
	BaseScene::mModel->AddModel("Sora", "Resouse/skybox.obj", "Resouse/skybox_A.png");
	BaseScene::mModel->AddModel("Ground", "Resouse/ground.obj", "Resouse/ground.png");

	BaseScene::mModel->AddModel("TankPlayerA", "Resouse/houtou.obj", "Resouse/sensha_A.png");
	BaseScene::mModel->AddModel("TankPlayerB", "Resouse/sensha_body.obj", "Resouse/sensha_A.png");
	BaseScene::mModel->AddModel("ArmR", "Resouse/R_hands.obj", "Resouse/hands_one.png");
	BaseScene::mModel->SetAncPoint("ArmR", Vector3(0.0f, -1.9f, -0.1f));
	BaseScene::mModel->AddModel("OjyouSama", "Resouse/ojosama_body.obj", "Resouse/ojosama_one.png");
	BaseScene::mModel->SetAncPoint("OjyouSama", Vector3(0.0f, 0.0f, 0.0f));
	BaseScene::mModel->AddModel("ArmL", "Resouse/L_hands.obj", "Resouse/hands_one.png");
	BaseScene::mModel->SetAncPoint("ArmL", Vector3(0.0f, -1.9f, -0.1f));

	mSound = std::make_shared<Sound>("loop_157.mp3", false);
	mSE = std::make_shared<Sound>("OkSE.mp3", false);
	//mSound->play();
	mSound->setVol(BaseScene::mMasterSoundVol * BaseScene::mBGMSoundVol);
	mSE->setVol(BaseScene::mMasterSoundVol * BaseScene::mSESoundVol);
}

void Title::UpdateScene()
{
	fade1 -= 0.01f;
	if (fade1 < 0)
	{
		fade1 = 0;
	}
	mSound->playLoop();
	if (fade1 <= 0)
	{
		if (Input::KeyDown(DIK_SPACE) || Input::pad_data.rgbButtons[2])
		{
			mSE->play();
			fadeF = true;
		}
		if (fadeF)
		{
			fade2 += 0.01;
			if (fade2 >= 1)
			{
				NextScene(std::make_shared<Select>());
			}
		}
	}
}

void Title::DrawScene()
{
	ojyouXR = 180.0f;
	ojyouXL = 180.0f;
	DirectXManager::GetInstance()->SetData3D();
	//BaseScene::mModel->Draw("Sora", Vector3(0, 0, -90.0f), Vector3(0, 0, 0), Vector3(5, 5, 5));
	BaseScene::mModel->Draw("TankPlayerA", ojoP, Vector3(0, 0, 0), Vector3(1.5f, 1.5f, 1.5f));
	//playerModel->Draw("TankPlayerHou", Vector3(position.x, position.y, position.z), Vector3(fireAngle, -atkAngle, 0), Vector3(1.5f, 1.5f, 1.5f));
	BaseScene::mModel->Draw("TankPlayerB", ojoP, Vector3(0, 0, 0), Vector3(1.5f, 1.5f, 1.5f));
	BaseScene::mModel->Draw("ArmR", Vector3(ojoP.x, ojoP.y+25.0f, ojoP.z), Vector3(ojyouXR, -ojyouY, 0), Vector3(10.0f, 10.0f, 10.0f));
	BaseScene::mModel->Draw("OjyouSama", ojoP, Vector3(0, -ojyouY, 0), Vector3(10.0f, 10.0f, 10.0f));
	BaseScene::mModel->Draw("ArmL", Vector3(ojoP.x, ojoP.y+25.0f, ojoP.z), Vector3(ojyouXL, -ojyouY,0), Vector3(10.0f, 10.0f, 10.0f));
	//BaseScene::mModel->Draw("Ground", Vector3(-20.0f, 4.0f, -90.0f), Vector3(0, 0, 0), Vector3(5, 5, 5));
	DirectXManager::GetInstance()->SetData2D();
	BaseScene::mSprite->Draw("Title", Vector3(100, 100, 0), 0.0f, Vector2(1, 1), Vector4(1, 1, 1, 1));
	BaseScene::mSprite->Draw("Push", Vector3(600, 300, 0), 0.0f, Vector2(1, 1), Vector4(1, 1, 1, 1));
	//BaseScene::mSprite->SetSize("Fade", Vector2(560-fade, 0-fade));
	BaseScene::mSprite->Draw("Fade1",Vector3(0, 0, 0), 0.0f, Vector2(1, 1), Vector4(1, 1, 1, fade1));
	BaseScene::mSprite->Draw("Fade2", Vector3(0, 0, 0), 0.0f, Vector2(1, 1), Vector4(1, 1, 1, fade2));
	//BaseScene::mSprite->Draw("Heart", Vector3(0, 0, 0), 0.0f, Vector2(100, 1), Vector4(1, 1, 1, 1));
}
