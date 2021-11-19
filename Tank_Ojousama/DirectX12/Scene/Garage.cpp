#include "Garage.h"
#include"Select.h"
#include"Title.h"
#include "../Sound/Sound.h"
#include "../Utility/Timer/Timer.h"

Garage::Garage()
	:mSound(nullptr) , mBGM(nullptr)
{
}

Garage::~Garage()
{
	delete objM;
}

void Garage::StartScene()
{
	fade = 0;
	fadeF = false;
	fadeFB = false;
	SAlfa1 = 0.5f;
	SAlfa2 = 0.5f;
	SAlfa3 = 0.5f;
	ChangeFlag = false;
	keyflag = 0;
	objM = new ObjectManager();
	objM->Claer();
	mChanger = new ModelChanger();
	mChanger->Init();

	BaseScene::mSprite->AddTexture("Sentaku1", "Resouse/Sankaku.png");
	BaseScene::mSprite->AddTexture("Sentaku2", "Resouse/Sankaku.png");
	BaseScene::mSprite->AddTexture("Sentaku3", "Resouse/Sankaku.png");
	BaseScene::mSprite->AddTexture("Sentaku4", "Resouse/Sankaku.png");
	BaseScene::mSprite->AddTexture("Sentaku5", "Resouse/Sankaku.png");
	BaseScene::mSprite->AddTexture("Sentaku6", "Resouse/Sankaku.png");
	BaseScene::mSprite->AddTexture("Garege", "Resouse/garege.jpg");
	BaseScene::mSprite->AddTexture("Syata", "Resouse/syata.jpg");
	BaseScene::mSprite->AddTexture("BackS", "Resouse/backselect.png");

	BaseScene::mSprite->AddTexture("sOjo", "Resouse/GaregeOjoSelect.png");
	BaseScene::mSprite->AddTexture("sHead", "Resouse/GaregeHeadSelect.png");
	BaseScene::mSprite->AddTexture("sBody", "Resouse/GaregeBodySelect.png");

	SentakuPos1 = Vector3((Window::Window_Width / 2) + 230, 250, 0);
	SentakuPos2 = Vector3((Window::Window_Width / 2) - 230, 250 + 64,0);
	SentakuPos3 = Vector3((Window::Window_Width / 2) + 230, 350,0);
	SentakuPos4 = Vector3((Window::Window_Width / 2) - 230, 350 + 64,0);
	SentakuPos5 = Vector3((Window::Window_Width / 2) + 230, 450, 0);
	SentakuPos6 = Vector3((Window::Window_Width / 2) - 230, 450 + 64, 0);
	//‚¨ì—l—pƒ‚ƒfƒ‹«
	BaseScene::mModel->AddModel("TankPlayerA", "Resouse/houtou.obj", "Resouse/sensha_A.png");
	BaseScene::mModel->AddModel("TankPlayerB", "Resouse/sensha_body.obj", "Resouse/sensha_A.png");

	BaseScene::mModel->AddModel("TankPlayerC", "Resouse/houtou.obj", "Resouse/sensha_A.png");
	BaseScene::mModel->AddModel("TankPlayerD", "Resouse/sensha_body.obj", "Resouse/sensha_A.png");

	//BaseScene::mModel->AddModel("TankPlayerC","Resouse/big_sensha_head.obj", "Resouse/big_sensha.png");
	//BaseScene::mModel->AddModel("TankPlayerD", "Resouse/big_sensha_body.obj", "Resouse/big_sensha.png");

	BaseScene::mModel->AddModel("ArmR1", "Resouse/R_hands.obj", "Resouse/hands_one.png");
	BaseScene::mModel->SetAncPoint("ArmR1", Vector3(0.0f, -2.1f, -0.1f));
	BaseScene::mModel->AddModel("OjyouSama1", "Resouse/ojosama_body.obj", "Resouse/ojosama_one.png");
	BaseScene::mModel->SetAncPoint("OjyouSama1", Vector3(0.0f, 0.0f, -0.1f));
	BaseScene::mModel->AddModel("ArmL1", "Resouse/L_hands.obj", "Resouse/hands_one.png");
	BaseScene::mModel->SetAncPoint("ArmL1", Vector3(0.0f, -2.1f, -0.1f));

	BaseScene::mModel->AddModel("ArmR2", "Resouse/R_hands.obj", "Resouse/hands_one.png");
	BaseScene::mModel->SetAncPoint("ArmR2", Vector3(0.0f, -2.1f, -0.1f));
	BaseScene::mModel->AddModel("OjyouSama2", "Resouse/ojosama_body.obj", "Resouse/ojosama_one.png");
	BaseScene::mModel->SetAncPoint("OjyouSama2", Vector3(0.0f, 0.0f, -0.1f));
	BaseScene::mModel->AddModel("ArmL2", "Resouse/L_hands.obj", "Resouse/hands_one.png");
	BaseScene::mModel->SetAncPoint("ArmL2", Vector3(0.0f, -2.1f, -0.1f));

	camera->SetEye(Vector3(0.0f, 0, -120));
	camera->SetTarget(Vector3(0.0f, 0, -100.0f));
	mTimer = std::make_shared<Timer>(0.01f);
	mSound = std::make_shared<Sound>("syata.mp3", false);
	mSound->play();
	mSound->setVol(BaseScene::mMasterSoundVol * BaseScene::mSESoundVol);

	mBGM = std::make_shared<Sound>("garegeBGM.mp3", false);
	mBGM->setVol(BaseScene::mMasterSoundVol * BaseScene::mBGMSoundVol);

	mSE = std::make_shared<Sound>("SelectSE.mp3", false);
	mSE->setVol(BaseScene::mMasterSoundVol * BaseScene::mSESoundVol);
}

void Garage::UpdateScene()
{
	mBGM->playLoop();
	mTimer->update();
	if (!mTimer->isTime()) return;
	if (keyflag < 0)
	{
		keyflag = 2;
	}
	if (keyflag > 2)
	{
		keyflag = 0;
	}
	if (fadeF)
	{
		if (Input::KeyDown(DIK_SPACE) || Input::pad_data.rgbButtons[2])
		{
			fadeFB = true;
			mSound->play();
			mChanger->Save();
		}
		if (Input::KeyDown(DIK_W) || Input::pad_data.lY < 0)
		{
			keyflag -= 1;
			mSE->play();
			mTimer->setTime(0.2f);
		}		
		if (Input::KeyDown(DIK_S) || Input::pad_data.lY > 0)
		{
			keyflag += 1;
			mSE->play();
			mTimer->setTime(0.2f);
		}
		if (Input::KeyDown(DIK_D) || Input::pad_data.lX > 0)
		{
			if (ChangeFlag == false)
			{
				ChangeFlag = true;
			}
			else
			{
				ChangeFlag = false;
			}
			mSE->play();
			mTimer->setTime(0.2f);
		}		
		if (Input::KeyDown(DIK_A) || Input::pad_data.lX < 0)
		{
			if (ChangeFlag == false)
			{
				ChangeFlag = true;
			}
			else
			{
				ChangeFlag = false;
			}
			mSE->play();
			mTimer->setTime(0.2f);
		}
		if (keyflag == 0)
		{
			SAlfa1 = 1.0f;
			SAlfa2 = 0.5f;
			SAlfa3 = 0.5f;
		}
		if (keyflag == 1)
		{
			SAlfa1 = 0.5f;
			SAlfa2 = 1.0f;
			SAlfa3 = 0.5f;
		}
		if (keyflag == 2)
		{
			SAlfa1 = 0.5f;
			SAlfa2 = 0.5f;
			SAlfa3 = 1.0f;
		}
	}
	if (fadeF == false)
	{
		fade -= 15;
	}
	if (fade <= -700)
	{
		fadeF = true;
		fade == -700;
	}
	if (fadeF&&fadeFB)
	{
		fade += 15;
		if (fade >= 0)
		{
			NextScene(std::make_shared<Select>());
		}
	}
}

void Garage::DrawScene()
{
	DirectXManager::GetInstance()->SetData2D();
	BaseScene::mSprite->SetSize("Garege", Vector2(1280, 720));
	BaseScene::mSprite->Draw("Garege", Vector3(0,0,0), 0.0f, Vector2(0.25f, 0.5f), Vector4(1, 1, 1, 1.0f));
	BaseScene::mSprite->Draw("Sentaku1", SentakuPos1, 0.0f, Vector2(0.25f, 0.5f), Vector4(1, 1, 1, SAlfa1));
	BaseScene::mSprite->Draw("Sentaku2", SentakuPos2, 180.0f, Vector2(0.25f, 0.5f), Vector4(1, 1, 1, SAlfa1));
	BaseScene::mSprite->Draw("Sentaku3", SentakuPos3, 0.0f, Vector2(0.25f, 0.5f), Vector4(1, 1, 1, SAlfa2));
	BaseScene::mSprite->Draw("Sentaku4", SentakuPos4, 180.0f, Vector2(0.25f, 0.5f), Vector4(1, 1, 1, SAlfa2));
	BaseScene::mSprite->Draw("Sentaku5", SentakuPos5, 0.0f, Vector2(0.25f, 0.5f), Vector4(1, 1, 1, SAlfa3));
	BaseScene::mSprite->Draw("Sentaku6", SentakuPos6, 180.0f, Vector2(0.25f, 0.5f), Vector4(1, 1, 1, SAlfa3));
	BaseScene::mSprite->Draw("BackS", Vector3(0,0,0), 0, Vector2(0.25f, 0.5f), Vector4(1, 1, 1, 1));
	if (keyflag == 0)
	{
		BaseScene::mSprite->SetSize("sOjo", Vector2(300, 300));
		BaseScene::mSprite->Draw("sOjo", Vector3(970, 210, 0), 0.0f, Vector2(1, 1), Vector4(1, 1, 1, 1));
	}	
	if (keyflag == 1)
	{
		BaseScene::mSprite->SetSize("sHead", Vector2(300, 300));
		BaseScene::mSprite->Draw("sHead", Vector3(970, 210, 0), 0.0f, Vector2(1, 1), Vector4(1, 1, 1, 1));
	}
	if (keyflag == 2)
	{
		BaseScene::mSprite->SetSize("sBody", Vector2(300, 300));
		BaseScene::mSprite->Draw("sBody", Vector3(970, 210, 0), 0.0f, Vector2(1, 1), Vector4(1, 1, 1, 1));
	}
		BaseScene::mSprite->SetSize("Syata", Vector2(1280, 720));
		BaseScene::mSprite->Draw("Syata", Vector3(0, fade, 0), 0, Vector2(0.25f, 0.5f), Vector4(1, 1, 1, 1));
	
	DirectXManager::GetInstance()->SetData3D();
	if (fadeF)
	{
		if (fadeFB) return;
		if (ChangeFlag == false)
		{
			BaseScene::mModel->Draw("ArmR1", Vector3(0, -2 + 3.2f, -113), Vector3(150, 30, 0), Vector3(1.5f, 1.5f, 1.5f));
			BaseScene::mModel->Draw("OjyouSama1", Vector3(0, -2, -113), Vector3(0, 30, 0), Vector3(1.5f, 1.5f, 1.5f));
			BaseScene::mModel->Draw("ArmL1", Vector3(0, -2 + 3.2f, -113), Vector3(150, 30, 0), Vector3(1.5f, 1.5f, 1.5f));
			BaseScene::mModel->Draw("TankPlayerA", Vector3(0, -2, -113), Vector3(0, 30, 0), Vector3(1.5f, 1.5f, 1.5f));
			BaseScene::mModel->Draw("TankPlayerB", Vector3(0, -2, -113), Vector3(0, 30, 0), Vector3(1.5f, 1.5f, 1.5f));
			
		}

		else if (ChangeFlag)
		{
			BaseScene::mModel->Draw("ArmR2", Vector3(0, -2 + 3.2f, -113), Vector3(150, -30, 0), Vector3(1.5f, 1.5f, 1.5f));
			BaseScene::mModel->Draw("OjyouSama2", Vector3(0, -2, -113), Vector3(0, -30, 0), Vector3(1.5f, 1.5f, 1.5f));
			BaseScene::mModel->Draw("ArmL2", Vector3(0, -2 + 3.2f, -113), Vector3(150, -30, 0), Vector3(1.5f, 1.5f, 1.5f));
			BaseScene::mModel->Draw("TankPlayerC", Vector3(0, -2, -113), Vector3(0, -30, 0), Vector3(1.5f, 1.5f, 1.5f));
			BaseScene::mModel->Draw("TankPlayerD", Vector3(0, -2, -113), Vector3(0, -30, 0), Vector3(1.5f, 1.5f, 1.5f));
		}
	}
	objM->Draw();
}
