#include "Title.h"
#include"Select.h"
//#include"Collision.h"
#include "../Collision/Collision.h"

Title::Title()
{

}

Title::~Title()
{
	
}

void Title::StartScene()
{
	camera->SetEye(Vector3(0.0f,6.0f,-90.0f));
	camera->SetTarget(Vector3(0.0f, 6.0f,-100.0f));
	BaseScene::mSprite->AddTexture("Title", "Resouse/titleAho.png");
	BaseScene::mSprite->AddTexture("Push", "Resouse/Space.png");
	BaseScene::mModel->AddModel("Sora", "Resouse/skydome.obj", "Resouse/skydome.jpg");
	BaseScene::mModel->AddModel("Ground", "Resouse/ground.obj", "Resouse/ground.png");
}

void Title::UpdateScene()
{
	if (Input::KeyDown(DIK_SPACE) || Input::pad_data.rgbButtons[2])
	{
		NextScene(std::make_shared<Select>());
	}
}

void Title::DrawScene()
{
	DirectXManager::GetInstance()->SetData3D();
	BaseScene::mModel->Draw("Sora", Vector3(0, 0, -90.0f), Vector3(0, 0, 0), Vector3(5, 5, 5));
	BaseScene::mModel->Draw("Ground", Vector3(-20.0f, 4.0f, -90.0f), Vector3(0, 0, 0), Vector3(5, 5, 5));
	DirectXManager::GetInstance()->SetData2D();
	BaseScene::mSprite->Draw("Title", Vector3(100, 100, 0), 0.0f, Vector2(1, 1), Vector4(1, 1, 1, 1));
	BaseScene::mSprite->Draw("Push", Vector3(600, 300, 0), 0.0f, Vector2(1, 1), Vector4(1, 1, 1, 1));
}
