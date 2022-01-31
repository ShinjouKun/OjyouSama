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
	mTimer = std::make_shared<Timer>();
	mTimer->setTime(0.2f);
	BaseScene::mSprite->AddTexture("OperationP", "Resouse/controller.png");
	BaseScene::mSprite->AddTexture("OperationB", "Resouse/operationback.png");

	/*BGM’Ç‰Á*/
	mBGM = std::make_shared<Sound>("BGM/loop_157.mp3", false);
	mBGM->setVol(BaseScene::mMasterSoundVol * BaseScene::mBGMSoundVol);
}

void Operation::UpdateScene()
{
	//BGMÄ¶
	mBGM->playLoop();

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
	BaseScene::mSprite->Draw("OperationB", Vector3(0.0f, 0.0f, 0.0f), 0.0f, Vector2(1.0f, 1.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f));
	BaseScene::mSprite->SetSize("OperationP", Vector2(1280.0f, 720.0f));
	BaseScene::mSprite->Draw("OperationP", Vector3(0.0f, 0.0f, 0.0f), 0.0f, Vector2(1.0f, 1.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f));
}

void Operation::FinalizeScene()
{
}
