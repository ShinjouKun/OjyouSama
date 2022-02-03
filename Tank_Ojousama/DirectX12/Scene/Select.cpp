#include "Select.h"
#include"GamePlay.h"
#include"Title.h"
#include "Option.h"
#include"Operation.h"
#include"Garage.h"
#include"BossScene.h"
#include "Robbery.h"
#include"Opening.h"
#include "Defense.h"
#include "GameClear.h"
#include"GameOver.h"
#include"Result.h"
#include"EndRoll.h"
#include "../Collision/Collision.h"
#include "../Device/Input.h"
#include "../Sound/Sound.h"
#include "../Utility/Timer/Timer.h"
#include"../Utility/Sequence/Sequence.h"


Select::Select()
{
}

Select::~Select()
{

}

void Select::StartScene()
{
	fade = 0;
	fadeF1 = false;
	fadeF2 = false;
	fadeF3 = false;
	fadeF4 = false;
	SelectAlfa1 = 0.5f;
	SelectAlfa2 = 0.5f;
	SelectAlfa3 = 0.5f;
	SelectAlfa4 = 0.5f;
	SelectAlfa5 = 0.5f;
	camerapos = Vector3(100.0f, 0.0f, 0);
	setcamerapos = Vector3(110, 0, 0);
	//camera->SetEye(camerapos);
	camera->SetTarget(setcamerapos);
	selectFlag = false;
	position = Vector3(600, 300, 0);
	selectposition = Vector3(32, 64, 0);
	targetPos1 = Vector3(320, 180, 0.0f);
	targetPos2 = Vector3(400, 540 - 64, 0.0f);
	targetPos3 = Vector3(870, 200, 0.0f);
	targetPos4 = Vector3(850, 540 - 64, 0.0f);
	syutu = Vector3(32, 64 - 32, 0.0f);
	operation = Vector3(20, 124 - 32, 0.0f);
	garege = Vector3(32, 192 - 32, 0.0f);
	option = Vector3(32, 256 - 32, 0.0f);
	titleback = Vector3(32, 320 - 32, 0.0f);
	BaseScene::mModel->AddModel("Sora", "Resouse/skybox.obj", "Resouse/skybox_A.png");
	BaseScene::mSprite->AddTexture("Arm", "Resouse/cars.png");
	BaseScene::mSprite->AddTexture("AIM1", "Resouse/AIM64.png");
	BaseScene::mSprite->AddTexture("target", "Resouse/select.png");
	BaseScene::mSprite->AddTexture("target2", "Resouse/select.png");
	BaseScene::mSprite->AddTexture("target3", "Resouse/select.png");
	BaseScene::mSprite->AddTexture("target4", "Resouse/select.png");
	BaseScene::mSprite->AddTexture("flower1", "Resouse/pinkflo.png");
	BaseScene::mSprite->AddTexture("flower2", "Resouse/blueflo.png");
	BaseScene::mSprite->AddTexture("flower3", "Resouse/greenflo.png");
	BaseScene::mSprite->AddTexture("flower4", "Resouse/redflo.png");
	BaseScene::mSprite->AddTexture("Syutugeki", "Resouse/syutugeki.png");
	BaseScene::mSprite->AddTexture("Garage", "Resouse/garezi.png");
	BaseScene::mSprite->AddTexture("Option", "Resouse/opusyon.png");
	BaseScene::mSprite->AddTexture("Titleback", "Resouse/tatleback.png");
	BaseScene::mSprite->AddTexture("Operation", "Resouse/operation.png");
	BaseScene::mSprite->AddTexture("Setumei", "Resouse/setuemei.png");
	BaseScene::mSprite->AddTexture("SetumeiBoss", "Resouse/setumeibos.png");
	BaseScene::mSprite->AddTexture("SetumeiDef", "Resouse/setumeiDe.png");
	BaseScene::mSprite->AddTexture("SetumeiRob", "Resouse/setumeiRo.png");
	BaseScene::mSprite->AddTexture("Sentaku", "Resouse/map_base.png");
	BaseScene::mSprite->AddTexture("SentakuClear1", "Resouse/map_1_clear.png");
	BaseScene::mSprite->AddTexture("SentakuClear2", "Resouse/map_2_clear.png");
	BaseScene::mSprite->AddTexture("SentakuClear3", "Resouse/map_3_clear.png");
	BaseScene::mSprite->AddTexture("Fade", "Resouse/fade.png");
	BaseScene::mSprite->AddTexture("En", "Resouse/en.png");
	BaseScene::mSprite->AddTexture("Gunsikin", "Resouse/gunnsikinn.png");
	BaseScene::mSprite->AddTexture("test", "Resouse/testend.png");
	BaseScene::mSprite->AddTexture("GunsiOver", "Resouse/gunsikin_gameover.png");

	BaseScene::mSprite->SetSize("Sentaku", Vector2(1280, 720));
	BaseScene::mSprite->SetSize("SentakuClear1", Vector2(1280, 720));
	BaseScene::mSprite->SetSize("SentakuClear2", Vector2(1280, 720));
	BaseScene::mSprite->SetSize("SentakuClear3", Vector2(1280, 720));
	BaseScene::mSprite->SetSize("flower1", Vector2(64, 64));
	BaseScene::mSprite->SetSize("flower2", Vector2(64, 64));
	BaseScene::mSprite->SetSize("flower3", Vector2(64, 64));
	BaseScene::mSprite->SetSize("flower4", Vector2(64, 64));
	BaseScene::mSprite->SetSize("En", Vector2(36, 52));
	BaseScene::mSprite->SetSize("Gunsikin", Vector2(640, 52));

	mSound = std::make_shared<Sound>("BGM/loop_157.mp3", false);
	mSE = std::make_shared<Sound>("SE/SelectSE.mp3", false);
	mDecisionSE = std::make_shared<Sound>("SE/OkSE.mp3", false);

	mTimer = std::make_shared<Timer>(0.01f);
	mSound->setVol(BaseScene::mMasterSoundVol * BaseScene::mBGMSoundVol);
	mSE->setVol(BaseScene::mMasterSoundVol * BaseScene::mSESoundVol);
	mDecisionSE->setVol(BaseScene::mMasterSoundVol * BaseScene::mSESoundVol);
}

void Select::UpdateScene()
{
	
	if (BaseScene::mMoney <= 0)
	{
		NextScene(std::make_shared<GameOver>());
	}
	if (fadeF1)
	{
		fade += 0.01f;
		if (fade >= 1)
		{
			NextScene(std::make_shared<GamePlay>());
		}
	}
	if (fadeF2)
	{
		fade += 0.01f;
		if (fade >= 1)
		{
			NextScene(std::make_shared<BossScene>());
		}
	}	
	if (fadeF3)
	{
		fade += 0.01f;
		if (fade >= 1)
		{
			NextScene(std::make_shared<Robbery>());
		}
	}	
	if (fadeF4)
	{
		fade += 0.01f;
		if (fade >= 1)
		{
			NextScene(std::make_shared<Defense>());
		}
	}
	mSound->playLoop();
	camera->SetEye(camerapos);
	camera->SetTarget(setcamerapos);
	setumeiFlag = false;
	setumeiBossFlag = false;
	setumeiDefFlag = false;
	setumeiRobFlag = false;
	setumei = Vector3(position.x + 32, position.y - 128, position.z);
#pragma region セレクト画面の選択


	mTimer->update();



	if (selectFlag == false)
	{
		if (selectposition.y < 64)
		{
			selectposition.y = 320;
		}
		else if (selectposition.y > 320)
		{
			selectposition.y = 64;
		}
		if (!mTimer->isTime()) return;
		//AIM用キー処理

		if (Input::getKey(KeyCode::W) || Input::joyVertical() > 0)
		{
			selectposition.y -= 64;
			mSE->play();
			mTimer->setTime(0.2f);
		}

		if (Input::getKey(KeyCode::S) || Input::joyVertical() < 0)
		{
			selectposition.y += 64;
			mSE->play();
			mTimer->setTime(0.2f);
		}
		if (selectposition.y == 64)
		{
			SelectAlfa1 = 1.0f;
			SelectAlfa2 = 0.5f;
			SelectAlfa3 = 0.5f;
			SelectAlfa4 = 0.5f;
			SelectAlfa5 = 0.5f;
			if (Input::getKeyDown(KeyCode::SPACE) || Input::getJoyDown(JoyCode::B))
			{
				selectFlag = true;
				mDecisionSE->play();
			}
		}
		else if (selectposition.y == 128)
		{
			SelectAlfa1 = 0.5f;
			SelectAlfa2 = 1.0f;
			SelectAlfa3 = 0.5f;
			SelectAlfa4 = 0.5f;
			SelectAlfa5 = 0.5f;
			if (Input::getKeyDown(KeyCode::SPACE) || Input::getJoyDown(JoyCode::B))
			{
				NextScene(std::make_shared<Operation>());
				mDecisionSE->play();
				mTimer->setTime(0.2f);
			}
		}
		else if (selectposition.y == 192)
		{
			SelectAlfa1 = 0.5f;
			SelectAlfa2 = 0.5f;
			SelectAlfa3 = 1.0f;
			SelectAlfa4 = 0.5f;
			SelectAlfa5 = 0.5f;
			if (Input::getKeyDown(KeyCode::SPACE) || Input::getJoyDown(JoyCode::B))
			{
				//NextScene(std::make_shared<EndRoll>());
				NextScene(std::make_shared<Garage>());
				mDecisionSE->play();
				mTimer->setTime(0.2f);
			}
		}
		else if (selectposition.y == 256)
		{
			SelectAlfa1 = 0.5f;
			SelectAlfa2 = 0.5f;
			SelectAlfa3 = 0.5f;
			SelectAlfa4 = 1.0f;
			SelectAlfa5 = 0.5f;
			if (Input::getKeyDown(KeyCode::SPACE) || Input::getJoyDown(JoyCode::B))
			{
				mDecisionSE->play();
				NextScene(std::make_shared<Option>());
			}
		}
		else if (selectposition.y == 320)
		{
			SelectAlfa1 = 0.5f;
			SelectAlfa2 = 0.5f;
			SelectAlfa3 = 0.5f;
			SelectAlfa4 = 0.5f;
			SelectAlfa5 = 1.0f;
			if (Input::getKeyDown(KeyCode::SPACE) || Input::getJoyDown(JoyCode::B))
			{
				mDecisionSE->play();
				NextScene(std::make_shared<Title>());
			}
		}
	}
#pragma endregion

#pragma region 選択場所のポジション
	if (selectFlag == true)
	{
		if (position.x >= targetPos1.x && position.x <= targetPos1.x + 64 && position.y >= targetPos1.y && position.y <= targetPos1.y + 64)
		{
			setumeiFlag = true;
			if (Input::getKeyDown(KeyCode::SPACE) || Input::getJoyDown(JoyCode::B))
			{
				fadeF1 = true;
				mDecisionSE->play();
			}
		}
		if (position.x >= targetPos2.x && position.x <= targetPos2.x + 64 && position.y >= targetPos2.y  && position.y <= targetPos2.y + 64)
		{
			if (BaseScene::mStageFlag1)
			{
				setumeiRobFlag = true;
				if (Input::getKeyDown(KeyCode::SPACE) || Input::getJoyDown(JoyCode::B))
				{
					fadeF3 = true;
					mDecisionSE->play();
				}
			}
		}
		if (position.x >= targetPos3.x && position.x <= targetPos3.x + 64 && position.y >= targetPos3.y && position.y <= targetPos3.y + 64)
		{
			if (BaseScene::mStageFlag3)
			{
				setumeiBossFlag = true;
				if (Input::getKeyDown(KeyCode::SPACE) || Input::getJoyDown(JoyCode::B))
				{
					fadeF2 = true;
					mDecisionSE->play();
				}
			}
		}
		if (position.x >= targetPos4.x && position.x <= targetPos4.x + 64 && position.y >= targetPos4.y && position.y <= targetPos4.y + 64)
		{
			if (BaseScene::mStageFlag2)
			{
				setumeiDefFlag = true;
				if (Input::getKeyDown(KeyCode::SPACE) || Input::getJoyDown(JoyCode::B))
				{
					fadeF4 = true;
					mDecisionSE->play();
				}
			}
		}

		if (Input::getKeyDown(KeyCode::ESCAPE) || Input::getJoyDown(JoyCode::A))
		{
			selectFlag = false;
		}
#pragma endregion

#pragma region カーソルキー処理

		//AIM用キー処理
		if (Input::getKey(KeyCode::W) || Input::joyVertical() > 0)
		{
			position.y -= 10;
		}

		if (Input::getKey(KeyCode::S) || Input::joyVertical() < 0)
		{
			position.y += 10;
		}

		if (Input::getKey(KeyCode::D) || Input::joyHorizontal() > 0)
		{
			position.x += 10;
		}
		if (Input::getKey(KeyCode::A) || Input::joyHorizontal() < 0)
		{
			position.x -= 10;
		}
		if (position.x <= 0)
		{
			position.x += 10;
		}		
		if (position.x >= 1250)
		{
			position.x -= 10;
		}	
		if (position.y <= 0)
		{
			position.y += 10;
		}		
		if (position.y >= 620)
		{
			position.y -= 10;
		}
	}
#pragma endregion

	if (cameramoveFlag)
	{
		camerapos.y -= 2;
		camerapos.x -= 2;
		setcamerapos.x -= 2;
		setcamerapos.y -= 2;
		if (setcamerapos.z < camerapos.z)
		{
			NextScene(std::make_shared<GamePlay>());
		}
	}
}

void Select::DrawScene()
{
	
	DirectXManager::GetInstance()->SetData2D();
	
	BaseScene::mSprite->Draw("Sentaku", Vector3(0, 0, 0), 0.0f, Vector2(1, 1), Vector4(1, 1, 1, 1));
	BaseScene::mSprite->Draw("target", targetPos1, 0.0f, Vector2(0.25f, 0.5f), Vector4(1, 1, 1, 1));
	
	if (!BaseScene::mStageFlag1)
	{
		BaseScene::mSprite->Draw("target2", targetPos2, 0.0f, Vector2(0.25f, 0.5f), Vector4(1, 1, 1, 0.5f));
	}
	else if(BaseScene::mStageFlag1)
	{
		BaseScene::mSprite->Draw("SentakuClear1", Vector3(0, 0, 0), 0.0f, Vector2(1, 1), Vector4(1, 1, 1, 1));
		BaseScene::mSprite->Draw("target2", targetPos2, 0.0f, Vector2(0.25f, 0.5f), Vector4(1, 1, 1, 1));
		BaseScene::mSprite->Draw("flower1", targetPos1, 0.0f, Vector2(0.25f, 0.5f), Vector4(1, 1, 1, 1));
	}
	if (!BaseScene::mStageFlag2)
	{
		BaseScene::mSprite->Draw("target3", targetPos4, 0.0f, Vector2(0.25f, 0.5f), Vector4(1, 1, 1, 0.5f));
	}
	else if (BaseScene::mStageFlag2)
	{
		BaseScene::mSprite->Draw("SentakuClear2", Vector3(0, 0, 0), 0.0f, Vector2(1, 1), Vector4(1, 1, 1, 1));
		BaseScene::mSprite->Draw("target3", targetPos4, 0.0f, Vector2(0.25f, 0.5f), Vector4(1, 1, 1, 1));
		BaseScene::mSprite->Draw("flower2", targetPos2, 0.0f, Vector2(0.25f, 0.5f), Vector4(1, 1, 1, 1));
		BaseScene::mSprite->Draw("flower1", targetPos1, 0.0f, Vector2(0.25f, 0.5f), Vector4(1, 1, 1, 1));
	}
	if (!BaseScene::mStageFlag3)
	{
		BaseScene::mSprite->Draw("target4", targetPos3, 0.0f, Vector2(1, 1), Vector4(1, 1, 1, 0.5f));
	}
	else if (BaseScene::mStageFlag3)
	{
		BaseScene::mSprite->Draw("SentakuClear3", Vector3(0, 0, 0), 0.0f, Vector2(1, 1), Vector4(1, 1, 1, 1));
		BaseScene::mSprite->Draw("target4", targetPos3, 0.0f, Vector2(1, 1), Vector4(1, 1, 1, 1));
		BaseScene::mSprite->Draw("flower3", targetPos4, 0.0f, Vector2(1, 1), Vector4(1, 1, 1, 1));
		BaseScene::mSprite->Draw("flower2", targetPos2, 0.0f, Vector2(0.25f, 0.5f), Vector4(1, 1, 1, 1));
		BaseScene::mSprite->Draw("flower1", targetPos1, 0.0f, Vector2(0.25f, 0.5f), Vector4(1, 1, 1, 1));
	}

	if (selectFlag == false)
	{
		BaseScene::mSprite->Draw("Titleback", titleback, 0.0f, Vector2(0.5f, 0.5f), Vector4(1, 1, 1, SelectAlfa5));
		BaseScene::mSprite->Draw("Option", option, 0.0f, Vector2(0.5f, 0.5f), Vector4(1, 1, 1, SelectAlfa4));
		BaseScene::mSprite->Draw("Syutugeki", syutu, 0.0f, Vector2(0.5f, 0.5f), Vector4(1, 1, 1, SelectAlfa1));
		BaseScene::mSprite->Draw("Operation", operation, 0.0f, Vector2(0.5f, 0.5f), Vector4(1, 1, 1, SelectAlfa2));
		BaseScene::mSprite->Draw("Garage", garege, 0.0f, Vector2(0.5f, 0.5f), Vector4(1, 1, 1, SelectAlfa3));
	}
	if (selectFlag)
	{
		BaseScene::mSprite->Draw("Arm", Vector3(position.x - 80, position.y - 32, 0), 0.0f, Vector2(5, 1), Vector4(1, 1, 1, 1));
	}
	if (setumeiFlag)
	{
		BaseScene::mSprite->Draw("Setumei", setumei, 0.0f, Vector2(0.25f, 0.5f), Vector4(1, 1, 1, 1));
	}
	if (setumeiBossFlag)
	{
		BaseScene::mSprite->Draw("SetumeiBoss", setumei, 0.0f, Vector2(0.25f, 0.5f), Vector4(1, 1, 1, 1));
	}	
	if (setumeiDefFlag)
	{
		BaseScene::mSprite->Draw("SetumeiDef", setumei, 0.0f, Vector2(0.25f, 0.5f), Vector4(1, 1, 1, 1));
	}	
	if (setumeiRobFlag)
	{
		BaseScene::mSprite->Draw("SetumeiRob", setumei, 0.0f, Vector2(0.25f, 0.5f), Vector4(1, 1, 1, 1));
	}
	Sequence::instance().set(BaseScene::mMoney, Vector2(564, 20), Vector2(32, 32));
	Sequence::instance().drawNumber(DirectXManager::GetInstance()->CmdList());
	DirectXManager::GetInstance()->SetDrawComnd();
	DirectXManager::GetInstance()->SetData2D();
	BaseScene::mSprite->Draw("En", Vector3(525, 8, 0), 0.0f, Vector2(1, 1), Vector4(1, 1, 1, 1));
	BaseScene::mSprite->Draw("Gunsikin", Vector3(320, 12, 0), 0.0f, Vector2(1, 1), Vector4(1, 1, 1, 1));
	BaseScene::mSprite->SetSize("GunsiOver", Vector2(256, 256));
	BaseScene::mSprite->Draw("GunsiOver", Vector3(0, 720-256, 0), 0.0f, Vector2(1, 1), Vector4(1, 1, 1, 1));
	BaseScene::mSprite->Draw("Fade", Vector3(0, 0, 0), 0.0f, Vector2(1, 1), Vector4(1, 1, 1, fade));
}

void Select::FinalizeScene()
{
}
