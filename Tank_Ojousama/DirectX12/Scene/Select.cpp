#include "Select.h"
#include"GamePlay.h"
#include"Title.h"
#include "Option.h"
#include"Garage.h"
#include "../Collision/Collision.h"
#include "../Device/Input.h"
#include "../Sound/Sound.h"
#include "../Utility/Timer/Timer.h"


Select::Select()
{
}

Select::~Select()
{

}

void Select::StartScene()
{
	//g = new GamePlay();
	fade = 0;
	fadeF = false;
	camerapos = Vector3(100.0f, 0.0f, 0);
	setcamerapos = Vector3(110, 0, 0);
	//camera->SetEye(camerapos);
	camera->SetTarget(setcamerapos);
	selectFlag = false;
	position = Vector3(600, 300, 0);
	selectposition = Vector3(32, 64, 0);
	targetPos1 = Vector3(320, 180, 0.0f);
	targetPos2 = Vector3(400, 540 - 64, 0.0f);
	targetPos3 = Vector3(960, 120, 0.0f);
	targetPos4 = Vector3(850, 540 - 64, 0.0f);
	syutu = Vector3(32, 64 - 32, 0.0f);
	garege = Vector3(32, 128 - 32, 0.0f);
	option = Vector3(32, 192 - 32, 0.0f);
	titleback = Vector3(32, 256 - 32, 0.0f);
	BaseScene::mModel->AddModel("Sora", "Resouse/skydome.obj", "Resouse/skydome.jpg");
	BaseScene::mSprite->AddTexture("Arm", "Resouse/cars.png");
	BaseScene::mSprite->AddTexture("AIM1", "Resouse/AIM64.png");
	BaseScene::mSprite->AddTexture("target", "Resouse/select.png");
	BaseScene::mSprite->AddTexture("target2", "Resouse/select.png");
	BaseScene::mSprite->AddTexture("target3", "Resouse/select.png");
	BaseScene::mSprite->AddTexture("target4", "Resouse/select.png");
	BaseScene::mSprite->AddTexture("Syutugeki", "Resouse/syutugeki.png");
	BaseScene::mSprite->AddTexture("Garage", "Resouse/garezi.png");
	BaseScene::mSprite->AddTexture("Option", "Resouse/opusyon.png");
	BaseScene::mSprite->AddTexture("Titleback", "Resouse/tatleback.png");
	BaseScene::mSprite->AddTexture("Setumei", "Resouse/setuemei.png");
	BaseScene::mSprite->AddTexture("Sentaku", "Resouse/sentaku.png");
	BaseScene::mSprite->AddTexture("Fade", "Resouse/fade.png");
	mSound = std::make_shared<Sound>("loop_157.mp3", false);

	mTimer = std::make_shared<Timer>(0.01f);
}

void Select::UpdateScene()
{
	if (fadeF)
	{
		fade += 0.01f;
		if (fade >= 1)
		{
			NextScene(std::make_shared<GamePlay>());
		}
	}
	mSound->playLoop();
	mSound->setVol(BaseScene::mBGMSoundVol);
	camera->SetEye(camerapos);
	camera->SetTarget(setcamerapos);
	setumeiFlag = false;
	setumei = Vector3(position.x + 32, position.y - 128, position.z);
	//b = g->GetA();
	//ImGui::Begin("conf");
	//ImGui::SliderFloat("slider",&camerapos.x , 0.0f, 360.0f);
	//ImGui::SliderFloat("slider",&camerapos.y , 0.0f, 360.0f);
	//ImGui::SliderFloat("slider",&camerapos.z , 0.0f, 360.0f);
	//ImGui::SliderInt("Money",&BaseScene::mMoney ,0,1000000000);
	//ImGui::Checkbox("selectflag",&selectFlag);
	//ImGui::End();
#pragma region セレクト画面の選択


	mTimer->update();

	if (!mTimer->isTime()) return;

	if(selectFlag == false)
	{
		if (selectposition.y < 64)
		{
			selectposition.y = 256;
		}
		else if (selectposition.y > 256)
		{
			selectposition.y = 64;
		}

		//AIM用キー処理

		if (Input::KeyDown(DIK_W) || Input::pad_data.lY < 0)
		{
			selectposition.y -= 64;
			mTimer->setTime(0.5f);
		}

		if (Input::KeyDown(DIK_S) || Input::pad_data.lY > 0)
		{
			selectposition.y += 64;
			mTimer->setTime(0.5f);
		}
		if (selectposition.y == 64)
		{
			if (Input::KeyDown(DIK_SPACE) || Input::pad_data.rgbButtons[2])
			{
				selectFlag = true;
				//mTimer->setTime(0.5f);
			}
		}
		else if (selectposition.y == 128)
		{
			if (Input::KeyDown(DIK_SPACE) || Input::pad_data.rgbButtons[2])
			{
				//NextScene(std::make_shared<Option>());
				mTimer->setTime(0.5f);
			}
		}		
		else if (selectposition.y == 192)
		{
			if (Input::KeyDown(DIK_SPACE) || Input::pad_data.rgbButtons[2])
			{
				//NextScene(std::make_shared<Garage>());
				mTimer->setTime(0.5f);
			}
		}
		else if (selectposition.y == 256)
		{
			if (Input::KeyDown(DIK_SPACE) || Input::pad_data.rgbButtons[2])
			{
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
			if (Input::KeyDown(DIK_SPACE) || Input::pad_data.rgbButtons[2])
			{
				fadeF = true;
				//cameramoveFlag = true;
			}
		}
		if (position.x >= targetPos2.x && position.x <= targetPos2.x + 64 && position.y >= targetPos2.y  && position.y <= targetPos2.y + 64)
		{
			setumeiFlag = true;
			if (Input::KeyDown(DIK_SPACE) || Input::pad_data.rgbButtons[2])
			{
				fadeF = true;
			}
		}
		if (position.x >= targetPos3.x && position.x <= targetPos3.x + 64 && position.y >= targetPos3.y && position.y <= targetPos3.y + 64)
		{
			setumeiFlag = true;
			if (Input::KeyDown(DIK_SPACE) || Input::pad_data.rgbButtons[2])
			{
				fadeF = true;
			}
		}
		if (position.x >= targetPos4.x && position.x <= targetPos4.x + 64 && position.y >= targetPos4.y && position.y <= targetPos4.y + 64)
		{
			setumeiFlag = true;
			if (Input::KeyDown(DIK_SPACE) || Input::pad_data.rgbButtons[2])
			{
				fadeF = true;
			}
		}

		if (Input::KeyDown(DIK_B) || Input::pad_data.rgbButtons[3])
		{
			selectFlag = false;
		}
#pragma endregion

#pragma region カーソルキー処理



		//AIM用キー処理
		if (Input::KeyState(DIK_W) || Input::pad_data.lY < 0)
		{
			position.y -= 10;
		}

		if (Input::KeyState(DIK_S) || Input::pad_data.lY > 0)
		{
			position.y += 10;
		}

		if (Input::KeyState(DIK_D) || Input::pad_data.lX > 0)
		{
			position.x += 10;
		}
		if (Input::KeyState(DIK_A) || Input::pad_data.lX < 0)
		{
			position.x -= 10;
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
			//camerapos = Vector3(150, 150, 150);
			NextScene(std::make_shared<GamePlay>());
		}
	}
}

void Select::DrawScene()
{
	DirectXManager::GetInstance()->SetData3D();
	BaseScene::mModel->Draw("Sora", Vector3(0, 2.0f, -90.0f), Vector3(0, 0, 0), Vector3(5, 5, 5));
	DirectXManager::GetInstance()->SetData2D();

	BaseScene::mSprite->Draw("Sentaku", Vector3(0,0,0),0.0f, Vector2(1, 1), Vector4(1, 1, 1, 1));
	BaseScene::mSprite->Draw("target", targetPos1, 0.0f, Vector2(0.25f, 0.5f), Vector4(1, 1, 1, 1));
	BaseScene::mSprite->Draw("target2", targetPos2, 0.0f, Vector2(0.25f, 0.5f), Vector4(1, 1, 1, 1));
	BaseScene::mSprite->Draw("target3", targetPos3, 0.0f, Vector2(0.25f, 0.5f), Vector4(1, 1, 1, 1));
	BaseScene::mSprite->Draw("target4", targetPos4, 0.0f, Vector2(1, 1), Vector4(1, 1, 1, 1));
		BaseScene::mSprite->Draw("Arm", Vector3(position.x -80 , position.y - 32, 0), 0.0f, Vector2(5, 1), Vector4(1, 1, 1, 1));
	if (selectFlag == false)
	{
		BaseScene::mSprite->Draw("Titleback", titleback, 0.0f, Vector2(0.5f, 0.5f), Vector4(1, 1, 1, 1));
		BaseScene::mSprite->Draw("Option", option, 0.0f, Vector2(0.5f, 0.5f), Vector4(1, 1, 1, 1));
		BaseScene::mSprite->Draw("Syutugeki", syutu, 0.0f, Vector2(0.5f, 0.5f), Vector4(1, 1, 1, 1));
		BaseScene::mSprite->Draw("Garage", garege, 0.0f, Vector2(0.5f, 0.5f), Vector4(1, 1, 1, 1));
	}
	if (setumeiFlag)
	{
		BaseScene::mSprite->Draw("Setumei", setumei, 0.0f, Vector2(0.25f, 0.5f), Vector4(1, 1, 1, 1));
	}
	if (selectFlag == false)
	{
		BaseScene::mSprite->Draw("AIM1", Vector3(selectposition.x - 32, selectposition.y - 32, 0), 0.0f, Vector2(3, 1), Vector4(1, 1, 1, 1));
	}
	BaseScene::mSprite->Draw("Fade", Vector3(0, 0, 0), 0.0f, Vector2(1, 1), Vector4(1, 1, 1, fade));
}
