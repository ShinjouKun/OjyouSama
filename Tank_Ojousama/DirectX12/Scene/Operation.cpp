#include "Operation.h"
#include"Select.h"
#include "../Sound/Sound.h"
#include "../Utility/Timer/Timer.h"

Operation::Operation()
{
}

Operation::~Operation()
{
}

void Operation::StartScene()
{
	mTimer = std::make_shared<Timer>(0.01f);
	mTimer->setTime(0.2f);
	BaseScene::mSprite->AddTexture("OperationP", "Resouse/controller.png");
	BaseScene::mSprite->AddTexture("OperationB", "Resouse/operationback.png");
}

void Operation::UpdateScene()
{
	mTimer->update();
	if (!mTimer->isTime()) return;
	if (Input::getKeyDown(KeyCode::SPACE) || Input::getJoyDown(JoyCode::B))
	{
		NextScene(std::make_shared<Select>());
	}
}

void Operation::DrawScene()
{
	DirectXManager::GetInstance()->SetData2D();
	BaseScene::mSprite->Draw("OperationB", Vector3(0, 0, 0), 0.0f, Vector2(1, 1), Vector4(1, 1, 1, 1));
	BaseScene::mSprite->SetSize("OperationP", Vector2(1280, 720));
	BaseScene::mSprite->Draw("OperationP", Vector3(0, 0, 0), 0.0f, Vector2(1, 1), Vector4(1, 1, 1, 1));
}
