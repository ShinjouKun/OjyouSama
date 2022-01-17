#include "Title.h"
#include"Select.h"
#include"Opening.h"
#include "../Collision/Collision.h"
#include "../Sound/Sound.h"

Title::Title() :
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
	camera->SetEye(Vector3(0.0f, 0, -122));
	camera->SetTarget(Vector3(0.0f, 0, -100.0f));



	//ojoP = Vector3(0, -25.0f, -113.0f);
	ojoP = Vector3(0, -25.0f, 200.0f);
	mAnimStep = AnimationStep::MOVE_FRONT;



	BaseScene::mSprite->AddTexture("Title", "Resouse/Title.png");
	BaseScene::mSprite->AddTexture("Push", "Resouse/start.png");
	BaseScene::mSprite->AddTexture("Fade1", "Resouse/fade.png");
	BaseScene::mSprite->AddTexture("Fade2", "Resouse/fade.png");
	/*BaseScene::mModel->AddModel("Sora", "Resouse/skybox.obj", "Resouse/skybox_A.png");
	BaseScene::mModel->AddModel("Ground", "Resouse/ground.obj", "Resouse/ground.png");*/

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

	//移動SE
	mMoveSE = std::make_shared<Sound>("SE/lab.wav", false);
	mMoveSE->setVol(BaseScene::mMasterSoundVol * BaseScene::mSESoundVol);
	//衝突SE
	mBreakSE = std::make_shared<Sound>("SE/Big_Explosion.mp3", false);
	mBreakSE->setVol(BaseScene::mMasterSoundVol * BaseScene::mSESoundVol);
	//インターバル
	mIntervalTime = std::make_shared<Timer>();
	mIntervalTime->setTime(1.0f);
}

void Title::UpdateScene()
{
	switch (mAnimStep)
	{
	case Title::MOVE_FRONT:

		ojoP.z -= 1.5f;

		mMoveSE->playLoop();

		if (ojoP.z <= -113.0f)
		{
			ojoP.z = -113.0f;
			mBreakSE->play();
			mAnimStep = AnimationStep::INTERVAL;
		}

		break;
	case Title::INTERVAL:

		mIntervalTime->update();

		if (mIntervalTime->isTime())
		{
			mAnimStep = AnimationStep::PLAY;
		}

		break;
	case Title::PLAY:

		mBGM->playLoop();

		break;
	default:
		break;
	}




	kakudai += 70;
	kakudai2 += 103;
	kakudai3 += 130;
	kakudai4 += 197.8f;
	fade1 -= 0.01f;
	if (fade1 < 0)
	{
		fade1 = 0;
	}
	//mBGM->playLoop();
	if (fade1 <= 0)
	{
		if (Input::getKeyDown(KeyCode::SPACE) || Input::getJoyDown(JoyCode::A))
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
	
	BaseScene::mModel->Draw("TankPlayerA", Vector3(ojoP.x, ojoP.y, ojoP.z), Vector3(0, 0, 0), Vector3(10.f, 10.f, 1.0f));
	BaseScene::mModel->Draw("TankPlayerB", Vector3(ojoP.x, ojoP.y, ojoP.z), Vector3(0, 0, 0), Vector3(10.f, 10.f, 1.0f));
	BaseScene::mModel->Draw("ArmR", Vector3(ojoP.x, ojoP.y + 25.0f, ojoP.z), Vector3(ojyouXR, -ojyouY, 0), Vector3(10.0f, 10.0f, 10.0f));
	BaseScene::mModel->Draw("OjyouSama", ojoP, Vector3(0, -ojyouY, 0), Vector3(10.0f, 10.0f, 10.0f));
	BaseScene::mModel->Draw("ArmL", Vector3(ojoP.x, ojoP.y + 25.0f, ojoP.z), Vector3(ojyouXL, -ojyouY, 0), Vector3(10.0f, 10.0f, 10.0f));

	DirectXManager::GetInstance()->SetData2D();
	if (mAnimStep == AnimationStep::PLAY)
	{
		
		BaseScene::mSprite->Draw("Title", Vector3(385, -10, 0), 0.0f, Vector2(1.2f, 1.2f), Vector4(1, 1, 1, 1));
		BaseScene::mSprite->Draw("Push", Vector3(290, 580, 0), 0.0f, Vector2(1, 1), Vector4(1, 1, 1, 1));
	}


	//BaseScene::mModel->Draw("Ground", Vector3(-20.0f, 4.0f, -90.0f), Vector3(0, 0, 0), Vector3(5, 5, 5));

	//	BaseScene::mSprite->Draw("Title", Vector3(385, -10, 0), 0.0f, Vector2(1.2f, 1.2f), Vector4(1, 1, 1, 1));
		//BaseScene::mSprite->Draw("Push", Vector3(290, 580, 0), 0.0f, Vector2(1, 1), Vector4(1, 1, 1, 1));
		//BaseScene::mSprite->SetSize("Heart", Vector2(1280+kakudai3, 720+kakudai));
		//BaseScene::mSprite->SetAncPoint("Heart", Vector2(640.0f+kakudai3, 360.0f+kakudai));
		//BaseScene::mSprite->Draw("Heart",Vector3(-640.0f-kakudai4, -360.0f-kakudai2, 0), 0.0f, Vector2(1, 1), Vector4(1, 1, 1, 1));
	BaseScene::mSprite->Draw("Fade1", Vector3(0, 0, 0), 0.0f, Vector2(1, 1), Vector4(1, 1, 1, fade1));
	BaseScene::mSprite->Draw("Fade2", Vector3(0, 0, 0), 0.0f, Vector2(1, 1), Vector4(1, 1, 1, fade2));
}
