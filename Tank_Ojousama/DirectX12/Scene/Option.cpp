#include "Option.h"
#include "Garage.h"
#include"Select.h"
#include"Title.h"
#include "../Device/Input.h"
#include "../Utility/Timer/Timer.h"
#include "../Sound/Sound.h"

Option::Option()
	:mSound(nullptr)
{
}

Option::~Option()
{
}

void Option::StartScene()
{
	posePos = Vector3(0, 0, 0);
	optionPos = Vector3(180, 180, 0);

	BaseScene::mSprite->AddTexture("VolOp", "Resouse/voloption.png");
	BaseScene::mSprite->AddTexture("VolOpAim1", "Resouse/volAim.png");
	BaseScene::mSprite->AddTexture("VolOpAim2", "Resouse/volAim.png");
	BaseScene::mSprite->AddTexture("VolOpAim3", "Resouse/volAim.png");
	BaseScene::mSprite->AddTexture("AimA1", "Resouse/volAimA.png");
	BaseScene::mSprite->AddTexture("AimA2", "Resouse/volAimA.png");
	BaseScene::mSprite->AddTexture("AimA3", "Resouse/volAimA.png");

	mSound = std::make_shared<Sound>("loop_157.mp3", false);
	mSound->setVol(BaseScene::mMasterSoundVol * BaseScene::mBGMSoundVol);

	mTimer = std::make_shared<Timer>(0.01f);
	mTimer->setTime(0.2f);
}

void Option::UpdateScene()
{
	mSound->playLoop();
	mTimer->update();
	if (!mTimer->isTime()) return;
	if (optionPos.y >= 541)
	{
		optionPos.y = 180;
	}
	if (optionPos.y <= 179)
	{
		optionPos.y = 540;
	}
	if (Input::KeyDown(DIK_S) || Input::pad_data.lY > 0)
	{
		optionPos += Vector3(0, 180.0f, 0);
		mTimer->setTime(0.2f);
	}
	if (Input::KeyDown(DIK_W) || Input::pad_data.lY < 0)
	{
		optionPos -= Vector3(0, 180.0f, 0);
		mTimer->setTime(0.2f);
	}
	//マスターボリューム
	if (optionPos.y == 180)
	{
		OpFlag1 = true;
		OpFlag2 = false;
		OpFlag3 = false;
		if (Input::KeyDown(DIK_D) || Input::pad_data.lX > 0)
		{
			BaseScene::mMasterSoundVol += 0.1f;
			OpAim1.x += 48.0f;
			OpAimA1.x += 48.0f;
			if (OpAim1.x > 880 && OpAimA1.x > 880)
			{
				OpAim1.x = 880;
				OpAimA1.x = 880;
			}
			if (BaseScene::mMasterSoundVol >= 1.0f)
			{
				BaseScene::mMasterSoundVol = 1.0f;
			}
			mTimer->setTime(0.2f);
		}
		if (Input::KeyDown(DIK_A) || Input::pad_data.lX < 0)
		{
			BaseScene::mMasterSoundVol -= 0.1f;
			OpAim1.x -= 48.0f;
			OpAimA1.x -= 48.0f;
			if (OpAim1.x < 352 && OpAimA1.x < 352)
			{
				OpAim1.x = 352.0f;
				OpAimA1.x = 352.0f;
			}
			if (BaseScene::mMasterSoundVol <= 0.0f)
			{
				BaseScene::mMasterSoundVol = 0.0f;
			}
			mTimer->setTime(0.2f);
		}
	}
	//BGMボリューム
	if (optionPos.y == 360)
	{
		OpFlag1 = false;
		OpFlag2 = true;
		OpFlag3 = false;
		if (Input::KeyDown(DIK_D) || Input::pad_data.lX > 0)
		{
			BaseScene::mBGMSoundVol += 0.1f;
			OpAim2.x += 48.0f;
			OpAimA2.x += 48.0f;
			if (OpAim2.x > 880 && OpAimA2.x > 880)
			{
				OpAim2.x = 880.0f;
				OpAimA2.x = 880.0f;
			}
			if (BaseScene::mBGMSoundVol >= 1.0f)
			{
				BaseScene::mBGMSoundVol = 1.0f;
			}
			mTimer->setTime(0.2f);
		}
		if (Input::KeyDown(DIK_A) || Input::pad_data.lX < 0)
		{
			BaseScene::mBGMSoundVol -= 0.1f;
			OpAim2.x -= 48.0f;
			OpAimA2.x -= 48.0f;
			if (OpAim2.x < 352 && OpAim2.x < 352)
			{
				OpAim2.x = 352.0f;
				OpAimA2.x = 352.0f;
			}
			if (BaseScene::mBGMSoundVol <= 0.0f)
			{
				BaseScene::mBGMSoundVol = 0.0f;
			}
			mTimer->setTime(0.2f);
		}
	}
	//SEボリューム
	if (optionPos.y == 540)
	{
		OpFlag1 = false;
		OpFlag2 = false;
		OpFlag3 = true;
		if (Input::KeyDown(DIK_D) || Input::pad_data.lX > 0)
		{
			BaseScene::mSESoundVol += 0.1f;
			OpAim3.x += 48.0f;
			OpAimA3.x += 48.0f;
			if (OpAim3.x > 880 && OpAimA3.x > 880)
			{
				OpAim3.x = 880.0f;
				OpAimA3.x = 880.0f;
			}
			if (BaseScene::mSESoundVol >= 1.0f)
			{

				BaseScene::mSESoundVol = 1.0f;
			}
			mTimer->setTime(0.2f);
		}
		if (Input::KeyDown(DIK_A) || Input::pad_data.lX < 0)
		{
			BaseScene::mSESoundVol -= 0.1f;
			OpAim3.x -= 48.0f;
			OpAimA3.x -= 48.0f;
			if (OpAim3.x < 352 && OpAimA3.x < 352)
			{
				OpAim3.x = 352.0f;
				OpAimA3.x = 352.0f;
			}
			if (BaseScene::mSESoundVol <= 0.0f)
			{

				BaseScene::mSESoundVol = 0.0f;
			}
			mTimer->setTime(0.2f);
		}
	}
	if (Input::KeyDown(DIK_SPACE) || Input::pad_data.rgbButtons[2])
	{
		NextScene(std::make_shared<Select>());
	}

	mSound->setVol(BaseScene::mMasterSoundVol * BaseScene::mBGMSoundVol);
}


void Option::DrawScene()
{
	DirectXManager::GetInstance()->SetData2D();
	BaseScene::mSprite->Draw("VolOp", posePos, 0.0f, Vector2(1, 1), Vector4(1, 1, 1, 1));
	BaseScene::mSprite->Draw("VolOpAim1", OpAim1, 0.0f, Vector2(1, 1), Vector4(1, 1, 1, 1));
	BaseScene::mSprite->Draw("VolOpAim2", OpAim2, 0.0f, Vector2(1, 1), Vector4(1, 1, 1, 1));
	BaseScene::mSprite->Draw("VolOpAim3", OpAim3, 0.0f, Vector2(1, 1), Vector4(1, 1, 1, 1));
	if (OpFlag1)
	{
		BaseScene::mSprite->Draw("AimA1", OpAimA1, 0.0f, Vector2(1, 1), Vector4(1, 1, 1, 0.5f));
	}
	else if (OpFlag2)
	{
		BaseScene::mSprite->Draw("AimA2", OpAimA2, 0.0f, Vector2(1, 1), Vector4(1, 1, 1, 0.5f));
	}
	else if (OpFlag3)
	{
		BaseScene::mSprite->Draw("AimA3", OpAimA3, 0.0f, Vector2(1, 1), Vector4(1, 1, 1, 0.5f));
	}
}
