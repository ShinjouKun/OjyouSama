#include "GamePlay.h"
#include"Title.h"
GamePlay::GamePlay()
{
	
}

GamePlay::~GamePlay()
{
	delete objM;//重要
}

void GamePlay::StartScene()
{
	objM = new ObjectManager();
	objM->Claer();
	//障害物
	/*objM->Add(new Block(Vector3(0.0f, -4.0f, -130.0f), Vector3(0, 0, 0), objM, BaseScene::mModel, 0));
	objM->Add(new Block(Vector3(45.0f, -4.0f, -130.0f), Vector3(0, 0, 0), objM, BaseScene::mModel, 1));
	objM->Add(new Block(Vector3(10.0f, -4.0f, -180.0f), Vector3(0, 0, 0), objM, BaseScene::mModel, 2));
    objM->Add(new Block(Vector3(80.0f, -4.0f, -110.0f), Vector3(0, 0, 0), objM, BaseScene::mModel, 3));
	objM->Add(new Block(Vector3(120.0f, -4.0f, -200.0f), Vector3(0, 0, 0), objM, BaseScene::mModel, 4));
	objM->Add(new Block(Vector3(120.0f, -4.0f, -130.0f), Vector3(0, 0, 0), objM, BaseScene::mModel,5));
	objM->Add(new Block(Vector3(160.0f, -4.0f, -160.0f), Vector3(0, 0, 0), objM, BaseScene::mModel, 6));
	objM->Add(new Block(Vector3(180.0f, -4.0f, -170.0f), Vector3(0, 0, 0), objM, BaseScene::mModel, 7));
	objM->Add(new Block(Vector3(140.0f, -4.0f, -210.0f), Vector3(0, 0, 0), objM, BaseScene::mModel, 8));
	objM->Add(new Block(Vector3(200.0f, -4.0f, -190.0f), Vector3(0, 0, 0), objM, BaseScene::mModel, 9));
	objM->Add(new Block(Vector3(150.0f, -4.0f, -120.0f), Vector3(0, 0, 0), objM, BaseScene::mModel, 10));
	objM->Add(new Block(Vector3(130.0f, -4.0f, -160.0f), Vector3(0, 0, 0), objM, BaseScene::mModel, 11));
*/
	//objM->Add(new T_REX(Vector3(60.0f, -4.0f, -250.0f), Vector3(0, 0, 0), objM, BaseScene::mModel, BaseScene::mParticle));
	/*int s = 0;
	float x_dis = 10.0f;
	float z_dis = 0.0f;
	for (int i = 0; i <= 49; i++)
	{
		if (s >= 5)
		{
			z_dis += 50.0f;
			x_dis = 0.0f;
			s = 0;
		}
	  objM->Add(new EnemyTank(Vector3(90.0f+(s*x_dis), 0.0f, -800.0f + z_dis), Vector3(0, 0, 0), objM, BaseScene::mModel, BaseScene::mParticle, BaseScene::mSprite, i));
	 
	  x_dis = 10.0f;
	  s++;
	}*/
		
	objM->Add(new GolemEnemy(Vector3(100.0f , 4.0f, -120.0f), Vector3(0, 0, 0), objM, BaseScene::mModel, BaseScene::mSprite, BaseScene::mParticle, 0));
	//objM->Add(new BlowEnemy(Vector3(95.0f, 0.0f, -125.0f), Vector3(0, 0, 0), objM, BaseScene::mModel, BaseScene::mSprite, BaseScene::mParticle, 0));
	//objM->Add(new TestEnemy(Vector3(90.0f, 0.0f, -100.0f), Vector3(0, 0, 0), objM, BaseScene::mModel, BaseScene::mSprite, 1));
	//objM->Add(new SniperEnemy(Vector3(100.0f, 0.0f, -120.0f), Vector3(0, 0, 0), objM, BaseScene::mModel, BaseScene::mSprite, BaseScene::mParticle, 2));
	BaseScene::mSprite->AddTexture("AIM", "Resouse/AIM64.png");
	BaseScene::mModel->AddModel("Sora2", "Resouse/skydome.obj", "Resouse/skydome.jpg");
	BaseScene::mModel->AddModel("Ground2", "Resouse/ground.obj", "Resouse/sougen.jpg");
	//プレイヤーは最後に、又はUIクラスを作る
	
	objM->Add(new Player(Vector3(0.0f, 0.0f, -50.0f), Vector3(0, 0, 0), objM, BaseScene::mModel, BaseScene::mParticle, BaseScene::mSprite));
	
}

void GamePlay::UpdateScene()
{
	objM->Update();
	if (Input::KeyDown(DIK_1))
	{
		NextScene(std::make_shared<Title>());
	}
}

void GamePlay::DrawScene()
{

	DirectXManager::GetInstance()->SetData3D();
	BaseScene::mModel->Draw("Sora2", Vector3(0, 2.0f, -90.0f), Vector3(0, 0, 0), Vector3(7, 7, 7));
	BaseScene::mModel->Draw("Ground2", Vector3(-20.0f, 0.0f, -90.0f), Vector3(0, 0, 0), Vector3(15, 15, 15));
	objM->Draw();
	DirectXManager::GetInstance()->SetData2D();
	BaseScene::mSprite->Draw("AIM", Vector3((Window::Window_Width / 2) - 32.0f, Window::Window_Height / 2, 0.0f), 0.0f, Vector2(1, 1), Vector4(1, 1, 1, 1));
}
