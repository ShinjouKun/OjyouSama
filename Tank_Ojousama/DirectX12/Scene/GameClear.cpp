#include "GameClear.h"
#include "Title.h"

GameClear::GameClear()
	:mSound(nullptr)
{

}

GameClear::~GameClear()
{
	delete objM;//重要
}

void GameClear::StartScene()
{
	fade1 = 1;
	fade2 = 0;
	fadeF = false;
	objM = new ObjectManager();
	objM->Claer();
	posePos = Vector3(0, 0, 0);
	camerapos = Vector3(4, 5, -10);
	setcamerapos = Vector3(0, 5, 0);
	BaseScene::mSprite->AddTexture("End", "Resouse/endhing.png");
	BaseScene::mSprite->AddTexture("Fade1", "Resouse/fade.png");
	BaseScene::mSprite->AddTexture("Fade2", "Resouse/fade.png");
	BaseScene::mModel->AddModel("Sora2", "Resouse/skybox.obj", "Resouse/skybox_A.png");
	BaseScene::mModel->AddModel("Ground2", "Resouse/ground.obj", "Resouse/sougen.png");

#pragma region 花モデル用
	BaseScene::mModel->AddModel("Flower1", "Resouse/flower.obj", "Resouse/flower.png");
	BaseScene::mModel->AddModel("Flower2", "Resouse/flower2.obj", "Resouse/flower2.png");
	BaseScene::mModel->AddModel("Flower3", "Resouse/flower3.obj", "Resouse/flower3.png");	
	BaseScene::mModel->AddModel("Flower4", "Resouse/flower.obj", "Resouse/flower.png");
	BaseScene::mModel->AddModel("Flower5", "Resouse/flower2.obj", "Resouse/flower2.png");
	BaseScene::mModel->AddModel("Flower6", "Resouse/flower3.obj", "Resouse/flower3.png");
	BaseScene::mModel->AddModel("Flower7", "Resouse/flower1.obj", "Resouse/flower.png");
	BaseScene::mModel->AddModel("Flower8", "Resouse/flower2.obj", "Resouse/flower2.png");
	BaseScene::mModel->AddModel("Flower9", "Resouse/flower3.obj", "Resouse/flower3.png");	
	BaseScene::mModel->AddModel("Flower10", "Resouse/flower1.obj", "Resouse/flower.png");
	BaseScene::mModel->AddModel("Flower11", "Resouse/flower2.obj", "Resouse/flower2.png");
	BaseScene::mModel->AddModel("Flower12", "Resouse/flower3.obj", "Resouse/flower3.png");	
	BaseScene::mModel->AddModel("Flower13", "Resouse/flower1.obj", "Resouse/flower.png");
	BaseScene::mModel->AddModel("Flower14", "Resouse/flower2.obj", "Resouse/flower2.png");
	BaseScene::mModel->AddModel("Flower15", "Resouse/flower3.obj", "Resouse/flower3.png");
	BaseScene::mModel->AddModel("Flower16", "Resouse/flower1.obj", "Resouse/flower.png");
	BaseScene::mModel->AddModel("Flower17", "Resouse/flower2.obj", "Resouse/flower2.png");
	BaseScene::mModel->AddModel("Flower18", "Resouse/flower3.obj", "Resouse/flower3.png");
	BaseScene::mModel->AddModel("Flower19", "Resouse/flower1.obj", "Resouse/flower.png");
	BaseScene::mModel->AddModel("Flower20", "Resouse/flower2.obj", "Resouse/flower2.png");
	BaseScene::mModel->AddModel("Flower21", "Resouse/flower3.obj", "Resouse/flower3.png");	
	BaseScene::mModel->AddModel("Flower22", "Resouse/flower1.obj", "Resouse/flower.png");
	BaseScene::mModel->AddModel("Flower23", "Resouse/flower2.obj", "Resouse/flower2.png");
	BaseScene::mModel->AddModel("Flower24", "Resouse/flower3.obj", "Resouse/flower3.png");
	BaseScene::mModel->AddModel("Flower25", "Resouse/flower.obj", "Resouse/flower.png");
	BaseScene::mModel->AddModel("Flower26", "Resouse/flower2.obj", "Resouse/flower2.png");
	BaseScene::mModel->AddModel("Flower27", "Resouse/flower3.obj", "Resouse/flower3.png");	
	BaseScene::mModel->AddModel("Flower28", "Resouse/flower.obj", "Resouse/flower.png");
	BaseScene::mModel->AddModel("Flower29", "Resouse/flower2.obj", "Resouse/flower2.png");
	BaseScene::mModel->AddModel("Flower30", "Resouse/flower3.obj", "Resouse/flower3.png");
	BaseScene::mModel->AddModel("Flower31", "Resouse/flower1.obj", "Resouse/flower.png");
	BaseScene::mModel->AddModel("Flower32", "Resouse/flower2.obj", "Resouse/flower2.png");
	BaseScene::mModel->AddModel("Flower33", "Resouse/flower3.obj", "Resouse/flower3.png");	
	BaseScene::mModel->AddModel("Flower34", "Resouse/flower1.obj", "Resouse/flower.png");
	BaseScene::mModel->AddModel("Flower35", "Resouse/flower2.obj", "Resouse/flower2.png");
	BaseScene::mModel->AddModel("Flower36", "Resouse/flower3.obj", "Resouse/flower3.png");	
	BaseScene::mModel->AddModel("Flower37", "Resouse/flower1.obj", "Resouse/flower.png");
	BaseScene::mModel->AddModel("Flower38", "Resouse/flower2.obj", "Resouse/flower2.png");
	BaseScene::mModel->AddModel("Flower39", "Resouse/flower3.obj", "Resouse/flower3.png");
	BaseScene::mModel->AddModel("Flower40", "Resouse/flower1.obj", "Resouse/flower.png");
	BaseScene::mModel->AddModel("Flower41", "Resouse/flower2.obj", "Resouse/flower2.png");
	BaseScene::mModel->AddModel("Flower42", "Resouse/flower3.obj", "Resouse/flower3.png");
	BaseScene::mModel->AddModel("Flower43", "Resouse/flower1.obj", "Resouse/flower.png");
	BaseScene::mModel->AddModel("Flower44", "Resouse/flower2.obj", "Resouse/flower2.png");
	BaseScene::mModel->AddModel("Flower45", "Resouse/flower3.obj", "Resouse/flower3.png");	
	BaseScene::mModel->AddModel("Flower46", "Resouse/flower1.obj", "Resouse/flower.png");
	BaseScene::mModel->AddModel("Flower47", "Resouse/flower2.obj", "Resouse/flower2.png");
	BaseScene::mModel->AddModel("Flower48", "Resouse/flower3.obj", "Resouse/flower3.png");

#pragma endregion
}

void GameClear::UpdateScene()
{
	camera->SetEye(camerapos);
	camera->SetTarget(setcamerapos);
	fade1 -= 0.01f;
	if (fade1 < 0)
	{
		fade1 = 0;
	}
	if (fade1 <= 0)
	{
		if (Input::getKeyDown(KeyCode::SPACE) || Input::getJoyDown(JoyCode::B))
		{
			fadeF = true;
		}
		if (fadeF)
		{
			fade2 += 0.01;
			if (fade2 >= 1)
			{
				BaseScene::mStageFlag1 = false;
				BaseScene::mStageFlag2 = false;
				BaseScene::mStageFlag3 = false;
				NextScene(std::make_shared<Title>());
			}
		}
	}
}

void GameClear::DrawScene()
{
	DirectXManager::GetInstance()->SetData3D();
	//BaseScene::mModel->Draw("TankPlayerA", Vector3(2, 0, 10), Vector3(0, -90, 0), Vector3(2.5f, 2.5f, 2.5f));
	//BaseScene::mModel->Draw("TankPlayerB", Vector3(2, 0, 10), Vector3(0, -90, 0), Vector3(2.5f, 2.5f, 2.5f));
	//BaseScene::mModel->Draw("OjyouSama", Vector3(-3, -2.5f , 4), Vector3(0, 0, 0), Vector3(1.5f, 1.5f, 1.5f));
	BaseScene::mModel->Draw("Ground2", Vector3(-20.0f, 0.0f, -90.0f), Vector3(0, 0, 0), Vector3(15, 15, 15));
	BaseScene::mModel->Draw("Sora2", Vector3(0, 2.0f, 0), Vector3(0, 0, 0), Vector3(2, 7, 2));
#pragma region 花用
	BaseScene::mModel->Draw("Flower1", Vector3(0, -0 , 0), Vector3(0, 0, 0), Vector3(1, 1, 1));
	BaseScene::mModel->Draw("Flower2", Vector3(5, -0 , 0), Vector3(0, 0, 0), Vector3(1, 1, 1));
	BaseScene::mModel->Draw("Flower3", Vector3(-5, -0 , 0), Vector3(0, 0, 0), Vector3(1, 1, 1));
	BaseScene::mModel->Draw("Flower4", Vector3(0, -0 , 8), Vector3(0, 0, 0), Vector3(1, 1, 1));	
	BaseScene::mModel->Draw("Flower5", Vector3(5, -0 , 8), Vector3(0, 0, 0), Vector3(1, 1, 1));
	BaseScene::mModel->Draw("Flower6", Vector3(-5, -0 , 8), Vector3(0, 0, 0), Vector3(1, 1, 1));
	BaseScene::mModel->Draw("Flower7", Vector3(0, -0 , 16), Vector3(0, 0, 0), Vector3(1, 1, 1));	
	BaseScene::mModel->Draw("Flower8", Vector3(5, -0 , 16), Vector3(0, 0, 0), Vector3(1, 1, 1));
	BaseScene::mModel->Draw("Flower9", Vector3(-5, -0 , 16), Vector3(0, 0, 0), Vector3(1, 1, 1));	
	BaseScene::mModel->Draw("Flower10", Vector3(0, -0 , 24), Vector3(0, 0, 0), Vector3(1, 1, 1));	
	BaseScene::mModel->Draw("Flower11", Vector3(5, -0 , 24), Vector3(0, 0, 0), Vector3(1, 1, 1));
	BaseScene::mModel->Draw("Flower12", Vector3(-5, -0 , 24), Vector3(0, 0, 0), Vector3(1, 1, 1));	
	BaseScene::mModel->Draw("Flower13", Vector3(0, -0 , 32), Vector3(0, 0, 0), Vector3(1, 1, 1));	
	BaseScene::mModel->Draw("Flower14", Vector3(5, -0 , 32), Vector3(0, 0, 0), Vector3(1, 1, 1));
	BaseScene::mModel->Draw("Flower15", Vector3(-5, -0 , 32), Vector3(0, 0, 0), Vector3(1, 1, 1));	
	BaseScene::mModel->Draw("Flower16", Vector3(0, -0 , 40), Vector3(0, 0, 0), Vector3(1, 1, 1));	
	BaseScene::mModel->Draw("Flower17", Vector3(5, -0 , 40), Vector3(0, 0, 0), Vector3(1, 1, 1));
	BaseScene::mModel->Draw("Flower18", Vector3(-5, -0 , 40), Vector3(0, 0, 0), Vector3(1, 1, 1));	
	BaseScene::mModel->Draw("Flower19", Vector3(0, -0 , 48), Vector3(0, 0, 0), Vector3(1, 1, 1));	
	BaseScene::mModel->Draw("Flower20", Vector3(5, -0 , 48), Vector3(0, 0, 0), Vector3(1, 1, 1));
	BaseScene::mModel->Draw("Flower21", Vector3(-5, -0 , 48), Vector3(0, 0, 0), Vector3(1, 1, 1));	
	BaseScene::mModel->Draw("Flower22", Vector3(0, -0 , 54), Vector3(0, 0, 0), Vector3(1, 1, 1));	
	BaseScene::mModel->Draw("Flower23", Vector3(5, -0 , 54), Vector3(0, 0, 0), Vector3(1, 1, 1));
	BaseScene::mModel->Draw("Flower24", Vector3(-5, -0 , 54), Vector3(0, 0, 0), Vector3(1, 1, 1));
	BaseScene::mModel->Draw("Flower25", Vector3(-15, -0 , 0), Vector3(0, 0, 0), Vector3(1, 1, 1));
	BaseScene::mModel->Draw("Flower26", Vector3(-10, -0 , 0), Vector3(0, 0, 0), Vector3(1, 1, 1));
	BaseScene::mModel->Draw("Flower27", Vector3(-20, -0 , 0), Vector3(0, 0, 0), Vector3(1, 1, 1));
	BaseScene::mModel->Draw("Flower28", Vector3(-15, -0 , 8), Vector3(0, 0, 0), Vector3(1, 1, 1));
	BaseScene::mModel->Draw("Flower29", Vector3(-10, -0 , 8), Vector3(0, 0, 0), Vector3(1, 1, 1));
	BaseScene::mModel->Draw("Flower30", Vector3(-20, -0 , 8), Vector3(0, 0, 0), Vector3(1, 1, 1));
	BaseScene::mModel->Draw("Flower31", Vector3(-15, -0 , 16), Vector3(0, 0, 0), Vector3(1, 1, 1));
	BaseScene::mModel->Draw("Flower32", Vector3(-10, -0 , 16), Vector3(0, 0, 0), Vector3(1, 1, 1));
	BaseScene::mModel->Draw("Flower33", Vector3(-20, -0 , 16), Vector3(0, 0, 0), Vector3(1, 1, 1));
	BaseScene::mModel->Draw("Flower34", Vector3(-15, -0 , 24), Vector3(0, 0, 0), Vector3(1, 1, 1));
	BaseScene::mModel->Draw("Flower35", Vector3(-10, -0 , 24), Vector3(0, 0, 0), Vector3(1, 1, 1));
	BaseScene::mModel->Draw("Flower36", Vector3(-20, -0 , 24), Vector3(0, 0, 0), Vector3(1, 1, 1));
	BaseScene::mModel->Draw("Flower37", Vector3(-15, -0 , 32), Vector3(0, 0, 0), Vector3(1, 1, 1));
	BaseScene::mModel->Draw("Flower38", Vector3(-10, -0 , 32), Vector3(0, 0, 0), Vector3(1, 1, 1));
	BaseScene::mModel->Draw("Flower39", Vector3(-20, -0 , 32), Vector3(0, 0, 0), Vector3(1, 1, 1));
	BaseScene::mModel->Draw("Flower40", Vector3(-15, -0 , 40), Vector3(0, 0, 0), Vector3(1, 1, 1));
	BaseScene::mModel->Draw("Flower41", Vector3(-10, -0 , 40), Vector3(0, 0, 0), Vector3(1, 1, 1));
	BaseScene::mModel->Draw("Flower42", Vector3(-20, -0 , 40), Vector3(0, 0, 0), Vector3(1, 1, 1));
	BaseScene::mModel->Draw("Flower43", Vector3(-15, -0 , 48), Vector3(0, 0, 0), Vector3(1, 1, 1));
	BaseScene::mModel->Draw("Flower44", Vector3(-10, -0 , 48), Vector3(0, 0, 0), Vector3(1, 1, 1));
	BaseScene::mModel->Draw("Flower45", Vector3(-20, -0 , 48), Vector3(0, 0, 0), Vector3(1, 1, 1));
	BaseScene::mModel->Draw("Flower46", Vector3(-15, -0 , 54), Vector3(0, 0, 0), Vector3(1, 1, 1));
	BaseScene::mModel->Draw("Flower47", Vector3(-10, -0 , 54), Vector3(0, 0, 0), Vector3(1, 1, 1));
	BaseScene::mModel->Draw("Flower48", Vector3(-20, -0 , 54), Vector3(0, 0, 0), Vector3(1, 1, 1));
#pragma endregion
	
	DirectXManager::GetInstance()->SetData2D();
	BaseScene::mSprite->Draw("End", Vector3(0, 0, 0), 0.0f, Vector2(1, 1), Vector4(1, 1, 1, 1));
	BaseScene::mSprite->Draw("Fade1", Vector3(0, 0, 0), 0.0f, Vector2(1, 1), Vector4(1, 1, 1, fade1));
	BaseScene::mSprite->Draw("Fade2", Vector3(0, 0, 0), 0.0f, Vector2(1, 1), Vector4(1, 1, 1, fade2));
}