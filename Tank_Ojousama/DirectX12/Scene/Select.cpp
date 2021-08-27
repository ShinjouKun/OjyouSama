#include "Select.h"
#include"GamePlay.h"
#include"Title.h"
#include "../Collision/Collision.h"
#include "../Device/Input.h"


Select::Select()
{
}

Select::~Select()
{

}

void Select::StartScene()
{
	//g = new GamePlay();
	
	selectFlag = false;
	position = Vector3(600, 300, 0);
	selectposition = Vector3(32, 64, 0);
	targetPos1 = Vector3(320, 180, 0.0f);
	targetPos2 = Vector3(320, 540 - 64, 0.0f);
	targetPos3 = Vector3(960, 180, 0.0f);
	targetPos4 = Vector3(960, 540 - 64, 0.0f);
	BaseScene::mModel->AddModel("Sora", "Resouse/skydome.obj", "Resouse/skydome.jpg");
	BaseScene::mSprite->AddTexture("AIM", "Resouse/AIM64.png");
	BaseScene::mSprite->AddTexture("AIM1", "Resouse/AIM64.png");
	BaseScene::mSprite->AddTexture("target", "Resouse/AIM64.png");
	BaseScene::mSprite->AddTexture("target2", "Resouse/AIM64.png");
	BaseScene::mSprite->AddTexture("target3", "Resouse/AIM64.png");
	BaseScene::mSprite->AddTexture("target4", "Resouse/AIM64.png");
}

void Select::UpdateScene()
{
	//b = g->GetA();
	ImGui::Begin("conf");
	//ImGui::SliderFloat("slider",&b , 0.0f, 360.0f);
	ImGui::Checkbox("selectflag",&selectFlag);
	ImGui::End();
#pragma region セレクト画面の選択

	if(selectFlag == false)
	{
		if (selectposition.y <= 64)
		{
			selectposition.y = 64;
		}
		else if (selectposition.y > 256)
		{
			selectposition.y = 256;
		}
		//AIM用キー処理
		if (Input::KeyDown(DIK_W) || Input::pad_data.lY < 0)
		{
			selectposition.y -= 64;
		}

		if (Input::KeyDown(DIK_S) || Input::pad_data.lY > 0)
		{
			selectposition.y += 64;
		}
		if (selectposition.y == 64)
		{
			if (Input::KeyDown(DIK_SPACE) || Input::pad_data.rgbButtons[2])
			{
				selectFlag = true;
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
			if (Input::KeyDown(DIK_SPACE) || Input::pad_data.rgbButtons[2])
			{
				camera->SetEye(position);
				NextScene(std::make_shared<GamePlay>());
			}
		}
		if (position.x >= targetPos2.x && position.x <= targetPos2.x + 64 && position.y >= targetPos2.y  && position.y <= targetPos2.y + 64)
		{
			if (Input::KeyDown(DIK_SPACE) || Input::pad_data.rgbButtons[2])
			{
				NextScene(std::make_shared<GamePlay>());
			}
		}
		if (position.x >= targetPos3.x && position.x <= targetPos3.x + 64 && position.y >= targetPos3.y && position.y <= targetPos3.y + 64)
		{
			if (Input::KeyDown(DIK_SPACE) || Input::pad_data.rgbButtons[2])
			{
				NextScene(std::make_shared<GamePlay>());
			}
		}
		if (position.x >= targetPos4.x && position.x <= targetPos4.x + 64 && position.y >= targetPos4.y && position.y <= targetPos4.y + 64)
		{
			if (Input::KeyDown(DIK_SPACE) || Input::pad_data.rgbButtons[2])
			{
				NextScene(std::make_shared<GamePlay>());
			}
		}

		if (Input::KeyDown(DIK_B) || Input::pad_data.rgbButtons[2])
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
}

void Select::DrawScene()
{
	DirectXManager::GetInstance()->SetData3D();
	BaseScene::mModel->Draw("Sora", Vector3(0, 0, -90.0f), Vector3(0, 0, 0), Vector3(5, 5, 5));
	DirectXManager::GetInstance()->SetData2D();
	BaseScene::mSprite->Draw("AIM", Vector3(position.x - 32, position.y - 32, 0), 0.0f, Vector2(5, 1), Vector4(1, 1, 1, 1));
	if (selectFlag == false)
	{
		BaseScene::mSprite->Draw("AIM1", Vector3(selectposition.x - 32, selectposition.y - 32, 0), 0.0f, Vector2(3, 1), Vector4(1, 1, 1, 1));
	}
	BaseScene::mSprite->Draw("target", targetPos1, 0.0f, Vector2(0.5f, 0.5f), Vector4(1, 1, 1, 1));
	BaseScene::mSprite->Draw("target2", targetPos2, 0.0f, Vector2(0.5f, 0.5f), Vector4(1, 1, 1, 1));
	BaseScene::mSprite->Draw("target3", targetPos3, 0.0f, Vector2(0.5f, 0.5f), Vector4(1, 1, 1, 1));
	BaseScene::mSprite->Draw("target4", targetPos4, 0.0f, Vector2(0.5f, 0.5f), Vector4(1, 1, 1, 1));
}
