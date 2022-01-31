#include "EndRoll.h"
#include "GameClear.h"
#include "../Sound/Sound.h"

EndRoll::EndRoll()
	:mSound(nullptr)
{
}

EndRoll::~EndRoll()
{
}

void EndRoll::StartScene()
{
	speed = 0;
	BaseScene::mSprite->AddTexture("Roll", "Resouse/theend.png");
	BaseScene::mSprite->AddTexture("E1", "Resouse/end1.png");
	BaseScene::mSprite->AddTexture("E2", "Resouse/end2.png");
	BaseScene::mSprite->AddTexture("E3", "Resouse/end3.png");
	mSound = std::make_shared<Sound>("BGM/endhing.wav", false);
	mSound->setVol(BaseScene::mMasterSoundVol * BaseScene::mBGMSoundVol);
}

void EndRoll::UpdateScene()
{
	mSound->playLoop();
	speed -= 1;
	if (speed <= -6700)
	{
		NextScene(std::make_shared<GameClear>());
	}
	if (Input::getKeyDown(KeyCode::Enter) || Input::getJoyDown(JoyCode::MenuButton))
	{
		NextScene(std::make_shared<GameClear>());
	}
}

void EndRoll::DrawScene()
{
	DirectXManager::GetInstance()->SetData2D();

	BaseScene::mSprite->SetSize("E1", Vector2(300, 200));
	BaseScene::mSprite->Draw("E1", Vector3(800, 2600 + speed, 0), 0.0f, Vector2(1, 1), Vector4(1, 1, 1, 1));
	BaseScene::mSprite->SetSize("E2", Vector2(300, 200));
	BaseScene::mSprite->Draw("E2", Vector3(70, 2000 + speed, 0), 0.0f, Vector2(1, 1), Vector4(1, 1, 1, 1));
	BaseScene::mSprite->SetSize("E3", Vector2(300, 200));
	BaseScene::mSprite->Draw("E3", Vector3(800, 940 + speed, 0), 0.0f, Vector2(1, 1), Vector4(1, 1, 1, 1));
	BaseScene::mSprite->SetSize("Roll", Vector2(1280-128, 7200-128));
	BaseScene::mSprite->Draw("Roll", Vector3(64, 720+speed, 0), 0.0f, Vector2(1, 1), Vector4(1, 1, 1, 1));

}
