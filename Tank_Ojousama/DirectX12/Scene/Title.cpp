#include "Title.h"
#include"Select.h"
#include"Opening.h"
#include "../Collision/Collision.h"
#include "../Sound/Sound.h"

Title::Title():
	mBGM(nullptr)
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
	kakudai = 0;
	kakudai2 = 0;
	kakudai3 = 0;
	kakudai4 = 0;
	ojyouY = 0.0f;
	ojyouXR = 0.0f;
	ojyouXL = 0.0f;
	camera->SetEye(Vector3(0.0f,0 ,-122));
	camera->SetTarget(Vector3(0.0f, 0,-100.0f));
	ojoP = Vector3(0, -25, -113);
	BaseScene::mSprite->AddTexture("Title", "Resouse/Title.png");
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

	mBGM = std::make_shared<Sound>("BGM/loop_157.mp3", false);
	mSE = std::make_shared<Sound>("SE/OkSE.mp3", false);
	//mSound->play();
	mBGM->setVol(BaseScene::mMasterSoundVol * BaseScene::mBGMSoundVol);
	mSE->setVol(BaseScene::mMasterSoundVol * BaseScene::mSESoundVol);
}

void Title::UpdateScene()
{
	kakudai += 70;
	kakudai2 += 103;
	kakudai3 += 130;
	kakudai4 += 197.8f;
	fade1 -= 0.01f;
	if (fade1 < 0)
	{
		fade1 = 0;
	}
	mBGM->playLoop();
	if (fade1 <= 0)
	{
		if (Input::getKeyDown(KeyCode::SPACE) || Input::getJoyDown(JoyCode::B))
		{
			mSE->play();
			fadeF = true;
		}
		if (fadeF)
		{
			fade2 += 0.01;
			if (fade2 >= 1)
			{
				NextScene(std::make_shared<Opening>());
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
	BaseScene::mSprite->Draw("Push", Vector3(550, 300, 0), 0.0f, Vector2(1, 1), Vector4(1, 1, 1, 1));
	//BaseScene::mSprite->SetSize("Heart", Vector2(1280+kakudai3, 720+kakudai));
	//BaseScene::mSprite->SetAncPoint("Heart", Vector2(640.0f+kakudai3, 360.0f+kakudai));
	//BaseScene::mSprite->Draw("Heart",Vector3(-640.0f-kakudai4, -360.0f-kakudai2, 0), 0.0f, Vector2(1, 1), Vector4(1, 1, 1, 1));
	BaseScene::mSprite->Draw("Fade1",Vector3(0, 0, 0), 0.0f, Vector2(1, 1), Vector4(1, 1, 1, fade1));
	BaseScene::mSprite->Draw("Fade2", Vector3(0, 0, 0), 0.0f, Vector2(1, 1), Vector4(1, 1, 1, fade2));
}
