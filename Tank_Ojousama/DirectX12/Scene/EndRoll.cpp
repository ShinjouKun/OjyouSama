#include "EndRoll.h"
#include "GameClear.h"

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
}

void EndRoll::UpdateScene()
{
	speed -= 1;
	if (speed <= -6700)
	{
		NextScene(std::make_shared<GameClear>());
	}
}

void EndRoll::DrawScene()
{
	DirectXManager::GetInstance()->SetData2D();
	BaseScene::mSprite->SetSize("Roll", Vector2(640, 7200));
	BaseScene::mSprite->Draw("Roll", Vector3(320, 720+speed, 0), 0.0f, Vector2(1, 1), Vector4(1, 1, 1, 1));
}