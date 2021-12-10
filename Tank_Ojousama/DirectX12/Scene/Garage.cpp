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
	bm = 0;
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
	headNum = 0;
	bodyNum = 0;
	bottomNum = 0;
	weaponNum = 0;

	BaseScene::mSprite->AddTexture("Sentaku1", "Resouse/Sankaku.png");
	BaseScene::mSprite->AddTexture("Sentaku2", "Resouse/Sankaku.png");
	BaseScene::mSprite->AddTexture("Sentaku3", "Resouse/Sankaku.png");
	BaseScene::mSprite->AddTexture("Sentaku4", "Resouse/Sankaku.png");
	BaseScene::mSprite->AddTexture("Sentaku5", "Resouse/Sankaku.png");
	BaseScene::mSprite->AddTexture("Sentaku6", "Resouse/Sankaku.png");
	BaseScene::mSprite->AddTexture("Sentaku7", "Resouse/Sankaku.png");
	BaseScene::mSprite->AddTexture("Sentaku8", "Resouse/Sankaku.png");
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
	SentakuPos7 = Vector3((Window::Window_Width / 2) + 230, 550, 0);
	SentakuPos8 = Vector3((Window::Window_Width / 2) - 230, 550 + 64, 0);
	//‚¨ì—l—pƒ‚ƒfƒ‹«
	BaseScene::mModel->AddModel("TankPlayerA", "Resouse/houtou.obj", "Resouse/sensha_A.png");
	BaseScene::mModel->AddModel("TankPlayerB", "Resouse/sensha_body.obj", "Resouse/sensha_A.png");

	BaseScene::mModel->AddModel("TankPlayerC", "Resouse/big_sensha_head.obj", "Resouse/big_sensha.png");
	BaseScene::mModel->AddModel("TankPlayerD", "Resouse/big_sensha_body.obj", "Resouse/big_sensha.png");

	BaseScene::mModel->AddModel("TankPlayerE", "Resouse/sensya_Type2_head.obj", "Resouse/sensya_type2_B.png");
	BaseScene::mModel->AddModel("TankPlayerF", "Resouse/sensya_Typ2_body.obj", "Resouse/sensya_type2_B.png");

	//BaseScene::mModel->AddModel("TankPlayerC","Resouse/big_sensha_head.obj", "Resouse/big_sensha.png");
	//BaseScene::mModel->AddModel("TankPlayerD", "Resouse/big_sensha_body.obj", "Resouse/big_sensha.png");

	BaseScene::mModel->AddModel("OjyouSama1", "Resouse/ojosama_body.obj", "Resouse/ojosama_one.png");
	BaseScene::mModel->SetAncPoint("OjyouSama1", Vector3(0.0f, 0.0f, -0.1f));

	BaseScene::mModel->AddModel("OjyouSama2", "Resouse/ojousama_body_black.obj", "Resouse/ojosama_black.png");
	BaseScene::mModel->SetAncPoint("OjyouSama2", Vector3(0.0f, 0.0f, -0.1f));

	BaseScene::mModel->AddModel("OjyouSama_r", "Resouse/ojousama_body_red.obj", "Resouse/ojosama_red.png");
	BaseScene::mModel->SetAncPoint("OjyouSama_r", Vector3(0.0f, 0.0f, -0.1f));



	camera->SetEye(Vector3(0.0f, 0, -120));
	camera->SetTarget(Vector3(0.0f, 0, -100.0f));
	mTimer = std::make_shared<Timer>(0.01f);
	mSound = std::make_shared<Sound>("SE/syata.mp3", false);
	mSound->play();
	mSound->setVol(BaseScene::mMasterSoundVol * BaseScene::mSESoundVol);

	mBGM = std::make_shared<Sound>("BGM/garegeBGM.mp3", false);
	mBGM->setVol(BaseScene::mMasterSoundVol * BaseScene::mBGMSoundVol);

	mSE = std::make_shared<Sound>("SE/SelectSE.mp3", false);
	mSE->setVol(BaseScene::mMasterSoundVol * BaseScene::mSESoundVol);
}

void Garage::UpdateScene()
{
	mBGM->playLoop();
	mTimer->update();
	if (!mTimer->isTime()) return;
	if (keyflag < 0)
	{
		keyflag = 3;
	}
	if (keyflag > 3)
	{
		keyflag = 0;
	}
	if (headNum < 0)
	{
		headNum = 2;
	}
	if (headNum > 2)
	{
		headNum = 0;
	}
	if (bodyNum < 0)
	{	
		bodyNum = 2;
	}	
	if (bodyNum > 2)
	{	
		bodyNum = 0;
	}
	if (bottomNum < 0)
	{
		bottomNum = 2;
	}
	if (bottomNum > 2)
	{
		bottomNum = 0;
	}
	if (fadeF)
	{
		if (Input::getKeyDown(KeyCode::SPACE) || Input::getJoyDown(JoyCode::B))
		{
			if (!M_Head_buy&&bodyNum == 1)
			{
				
				bm = BaseScene::mMoney -= 2000000;
				if (bm <= 0)
				{
					return;
				}
				M_Head_buy = true;
			}
			else if (!H_Head_buy&&bodyNum == 2)
			{
				bm = BaseScene::mMoney - 3500000;
				if (bm <= 0)
				{
					return;
				}
				H_Head_buy = true;
			}
			else if(!M_Bottom_buy&&bottomNum == 1)
			{
				bm = BaseScene::mMoney - 2000000;
				if (bm <= 0)
				{
					return;
				}
				M_Bottom_buy = true;
			}
			else if(!H_Bottom_buy&&bottomNum == 2)
			{
				bm = BaseScene::mMoney - 3000000;
				if (bm <= 0)
				{
					return;
				}
				H_Bottom_buy = true;
			}
			else if(!M_Ojyou_buy&&headNum == 1)
			{
				bm = BaseScene::mMoney - 2000000;
				if (bm <= 0)
				{
					return;
				}
				M_Ojyou_buy = true;
			}
			else if (!H_Ojyou_buy&&headNum == 2)
			{
				bm = BaseScene::mMoney - 4000000;
				if (bm <= 0)
				{
					return;
				}
				H_Ojyou_buy = true;
			}


			fadeFB = true;
			mSound->play();
			mChanger->Save();
		}
		if (Input::getKey(KeyCode::W) || Input::joyVertical() > 0)
		{
			keyflag -= 1;
			mSE->play();
			mTimer->setTime(0.2f);
		}		
		if (Input::getKey(KeyCode::S) || Input::joyVertical() < 0)
		{
			keyflag += 1;
			mSE->play();
			mTimer->setTime(0.2f);
		}
		if (Input::getKey(KeyCode::D) || Input::joyHorizontal() > 0)
		{
			switch (keyflag)
			{
			case 0:
				headNum += 1;
				break;
			case 1:
				bodyNum += 1;
				break;
			case 2:
				bottomNum += 1;
				break;
			case 3:
				weaponNum += 1;
				break;
			default:
				headNum = 0;
				bodyNum = 0;
				bottomNum = 0;
				weaponNum = 0;
				break;
			}
			mSE->play();
			mTimer->setTime(0.2f);
		}		
		if (Input::getKey(KeyCode::A) || Input::joyHorizontal() < 0)
		{
			switch (keyflag)
			{
			case 0:
				headNum -= 1;
				break;
			case 1:
				bodyNum -= 1;
				break;
			case 2:
				bottomNum -= 1;
				break;
			case 3:
				weaponNum -= 1;
				break;
			default:
				headNum = 0;
				bodyNum = 0;
				bottomNum = 0;
				weaponNum = 0;
				break;
			}
			mSE->play();
			mTimer->setTime(0.2f);
		}
		if (keyflag == 0)
		{
			SAlfa1 = 1.0f;
			SAlfa2 = 0.5f;
			SAlfa3 = 0.5f;
			SAlfa4 = 0.5f;
		}
		if (keyflag == 1)
		{
			SAlfa1 = 0.5f;
			SAlfa2 = 1.0f;
			SAlfa3 = 0.5f;
			SAlfa4 = 0.5f;
		}
		if (keyflag == 2)
		{
			SAlfa1 = 0.5f;
			SAlfa2 = 0.5f;
			SAlfa3 = 1.0f;
			SAlfa4 = 0.5f;
		}
		if (keyflag == 4)
		{
			SAlfa1 = 0.5f;
			SAlfa2 = 0.5f;
			SAlfa3 = 0.5f;
			SAlfa4 = 1.0f;
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
	BaseScene::mSprite->Draw("Sentaku7", SentakuPos7, 0.0f, Vector2(0.25f, 0.5f), Vector4(1, 1, 1, SAlfa4));
	BaseScene::mSprite->Draw("Sentaku8", SentakuPos8, 180.0f, Vector2(0.25f, 0.5f), Vector4(1, 1, 1, SAlfa4));
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
		switch (headNum)
		{
		case 0:
			BaseScene::mModel->Draw("OjyouSama1", Vector3(0, -2, -113), Vector3(0, 30, 0), Vector3(1.5f, 1.5f, 1.5f));
			mChanger->ChangeHead(Normal);
			break;
		case 1:
			BaseScene::mModel->Draw("OjyouSama2", Vector3(0, -2, -113), Vector3(0, 30, 0), Vector3(1.5f, 1.5f, 1.5f));
			mChanger->ChangeHead(Other01);
			break;
		case 2:
			BaseScene::mModel->Draw("OjyouSama_r", Vector3(0, -2, -113), Vector3(0, 30, 0), Vector3(1.5f, 1.5f, 1.5f));
			mChanger->ChangeHead(Other02);
			break;
		default:
			BaseScene::mModel->Draw("OjyouSama1", Vector3(0, -2, -113), Vector3(0, 30, 0), Vector3(1.5f, 1.5f, 1.5f));
			mChanger->ChangeHead(Normal);
			break;
		}
		switch (bodyNum)
		{
		case 0:
			BaseScene::mModel->Draw("TankPlayerA", Vector3(0, -2, -113), Vector3(0, 30, 0), Vector3(1.5f, 1.5f, 1.5f));
			mChanger->ChangeBody(Light);
			break;
		case 1:
			BaseScene::mModel->Draw("TankPlayerC", Vector3(0, -2, -113), Vector3(0, 30, 0), Vector3(1.5f, 1.5f, 1.5f));
			mChanger->ChangeBody(Midium);
			break;
		case 2:
			BaseScene::mModel->Draw("TankPlayerE", Vector3(0, -2, -113), Vector3(0, 30, 0), Vector3(1.5f, 1.5f, 1.5f));
			mChanger->ChangeBody(Heavy);
			break;
		default:
			BaseScene::mModel->Draw("TankPlayerA", Vector3(0, -2, -113), Vector3(0, 30, 0), Vector3(1.5f, 1.5f, 1.5f));
			mChanger->ChangeBody(Light);
			break;
		}
		switch (bottomNum)
		{
		case 0:
			BaseScene::mModel->Draw("TankPlayerB", Vector3(0, -2, -113), Vector3(0, 30, 0), Vector3(1.5f, 1.5f, 1.5f));
			mChanger->ChangeBottom(Light_b);
			break;
		case 1:
			BaseScene::mModel->Draw("TankPlayerD", Vector3(0, -2, -113), Vector3(0, 30, 0), Vector3(1.5f, 1.5f, 1.5f));
			mChanger->ChangeBottom(Midium_b);
			break;
		case 2:
			BaseScene::mModel->Draw("TankPlayerF", Vector3(0, -2, -113), Vector3(0, 30, 0), Vector3(1.5f, 1.5f, 1.5f));
			mChanger->ChangeBottom(Heavy_b);
			break;
		default:
			BaseScene::mModel->Draw("TankPlayerB", Vector3(0, -2, -113), Vector3(0, 30, 0), Vector3(1.5f, 1.5f, 1.5f));
			mChanger->ChangeBottom(Light_b);
			break;
		}
		switch (weaponNum)
		{
		case 0:
			mChanger->ChangeWeapons1(MachinGun);
			break;
		case 1:
			mChanger->ChangeWeapons1(ShotGun);
			break;
		case 2:
			mChanger->ChangeWeapons1(Mine);
			break;
		default:
			mChanger->ChangeWeapons1(MachinGun);
			break;
		}
	}
	objM->Draw();
}
