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
	fade = 0;
	fadeF = false;
	ojyouY = 0.0f;
	ojyouXR = 0.0f;
	ojyouXL = 0.0f;
	camera->SetEye(Vector3(0.0f,0 ,-120));
	camera->SetTarget(Vector3(0.0f, 0,-100.0f));
	ojoP = Vector3(0, -5, -110);
	BaseScene::mSprite->AddTexture("Title", "Resouse/titlerogo.png");
	BaseScene::mSprite->AddTexture("Push", "Resouse/start.png");
	BaseScene::mSprite->AddTexture("Heart", "Resouse/heart.png");
	BaseScene::mSprite->AddTexture("Fade", "Resouse/fade.png");
	BaseScene::mModel->AddModel("Sora", "Resouse/skybox.obj", "Resouse/skybox_A.png");
	BaseScene::mModel->AddModel("Ground", "Resouse/ground.obj", "Resouse/ground.png");

	BaseScene::mModel->AddModel("TankPlayerA", "Resouse/houtou.obj", "Resouse/sensha_A.png");
	BaseScene::mModel->AddModel("TankPlayerB", "Resouse/sensha_body.obj", "Resouse/sensha_A.png");
	BaseScene::mModel->AddModel("ArmR", "Resouse/R_hands.obj", "Resouse/hands_one.png");
	BaseScene::mModel->SetAncPoint("ArmR", Vector3(0.0f, -2.1f, -0.1f));
	BaseScene::mModel->AddModel("OjyouSama", "Resouse/ojosama_body.obj", "Resouse/ojosama_one.png");
	BaseScene::mModel->SetAncPoint("OjyouSama", Vector3(0.0f, 0.0f, -0.1f));
	BaseScene::mModel->AddModel("ArmL", "Resouse/L_hands.obj", "Resouse/hands_one.png");
	BaseScene::mModel->SetAncPoint("ArmL", Vector3(0.0f, -2.1f, -0.1f));

	mSound = std::make_shared<Sound>("loop_157.mp3", false);
	//mSound->play();
	mSound->setVol(BaseScene::mMasterSoundVol * BaseScene::mBGMSoundVol);
}

void Title::UpdateScene()
{
	mSound->playLoop();
	if (Input::KeyDown(DIK_SPACE) || Input::pad_data.rgbButtons[2])
	{
		fadeF = true;
		//NextScene(std::make_shared<Select>());
	}
	if (fadeF)
	{
		fade += 0.01;
		if (fade >= 1)
		{
			NextScene(std::make_shared<Select>());
		}
	}
}

void Title::DrawScene()
{
	ojyouXR -= 10.0f;
	ojyouXL += 10.0f;
	ojyouY -= 10.0f;

	DirectXManager::GetInstance()->SetData3D();
	//BaseScene::mModel->Draw("Sora", Vector3(0, 0, -90.0f), Vector3(0, 0, 0), Vector3(5, 5, 5));
	BaseScene::mModel->Draw("TankPlayerA", ojoP, Vector3(0, 0, 0), Vector3(1.5f, 1.5f, 1.5f));
	//playerModel->Draw("TankPlayerHou", Vector3(position.x, position.y, position.z), Vector3(fireAngle, -atkAngle, 0), Vector3(1.5f, 1.5f, 1.5f));
	BaseScene::mModel->Draw("TankPlayerB", ojoP, Vector3(0, 0, 0), Vector3(1.5f, 1.5f, 1.5f));
	BaseScene::mModel->Draw("ArmR", Vector3(ojoP.x, ojoP.y+3.2f, ojoP.z), Vector3(ojyouXR, -ojyouY, 0), Vector3(1.5f, 1.5f, 1.5f));
	BaseScene::mModel->Draw("OjyouSama", ojoP, Vector3(0, -ojyouY, 0), Vector3(1.5f, 1.5f, 1.5f));
	BaseScene::mModel->Draw("ArmL", Vector3(ojoP.x, ojoP.y + 3.2f, ojoP.z), Vector3(ojyouXL, -ojyouY, 0), Vector3(1.5f, 1.5f, 1.5f));
	//BaseScene::mModel->Draw("Ground", Vector3(-20.0f, 4.0f, -90.0f), Vector3(0, 0, 0), Vector3(5, 5, 5));
	DirectXManager::GetInstance()->SetData2D();
	BaseScene::mSprite->Draw("Title", Vector3(100, 100, 0), 0.0f, Vector2(1, 1), Vector4(1, 1, 1, 1));
	BaseScene::mSprite->Draw("Push", Vector3(600, 300, 0), 0.0f, Vector2(1, 1), Vector4(1, 1, 1, 1));
	//BaseScene::mSprite->SetSize("Fade", Vector2(560-fade, 0-fade));
	BaseScene::mSprite->Draw("Fade", Vector3(0, 0, 0), 0.0f, Vector2(1, 1), Vector4(1, 1, 1, fade));
	//BaseScene::mSprite->Draw("Heart", Vector3(0, 0, 0), 0.0f, Vector2(100, 1), Vector4(1, 1, 1, 1));
}
