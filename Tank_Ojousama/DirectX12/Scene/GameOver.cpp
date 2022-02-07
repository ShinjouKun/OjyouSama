#include "GameOver.h"
#include "GamePlay.h"
#include "BossScene.h"
#include "Robbery.h"
#include "Defense.h"
#include"Title.h"
#include"Select.h"
#include "../Sound/Sound.h"
#include "../Utility/Timer/Timer.h"
#include"../Utility/Sequence/Sequence.h"
#include "../ParticleSystem/ParticleType/BlackSmoke.h"

GameOver::GameOver()
	:mSound(nullptr)
{
}

GameOver::~GameOver()
{
}

void GameOver::StartScene()
{
	objM = new ObjectManager();
	objM->Claer();
	posePos = Vector3(0, 0, 0);
	BaseScene::mSprite->AddTexture("Shuri", "Resouse/syuurihi.png");
	BaseScene::mSprite->SetSize("Shuri", Vector2(320, 32));
	BaseScene::mSprite->AddTexture("GOver", "Resouse/gameover.png");
	BaseScene::mSprite->AddTexture("GEOver", "Resouse/sinend.png");
	BaseScene::mSprite->AddTexture("\1", "Resouse/en.png");
	BaseScene::mSprite->SetSize("\1", Vector2(32, 32));

	BaseScene::mModel->AddModel("Sora", "Resouse/skybox.obj", "Resouse/skybox_A.png");
	BaseScene::mModel->AddModel("Ground2", "Resouse/Plane.obj", "Resouse/Ground_Black.png");
	selectbackPos = Vector3(180, 180, 0);
	selectposition = Vector3(180, 180, 0);
	camerapos = Vector3(4, 5, -10);
	setcamerapos = Vector3(0, 5, 0);
	BaseScene::mModel->AddModel("TankPlayerA", "Resouse/houtou.obj", "Resouse/sensha_A.png");
	BaseScene::mModel->AddModel("TankPlayerB", "Resouse/sensha_body.obj", "Resouse/sensha_A.png");
	BaseScene::mModel->AddModel("ArmR", "Resouse/R_hands.obj", "Resouse/hands_one.png");
	BaseScene::mModel->SetAncPoint("ArmR", Vector3(0.0f, -2.1f, -0.1f));
	BaseScene::mModel->AddModel("OjyouSama", "Resouse/ojosama_body.obj", "Resouse/ojosama_one.png");
	BaseScene::mModel->SetAncPoint("OjyouSama", Vector3(0.0f, 0.0f, -0.1f));
	BaseScene::mModel->AddModel("ArmL", "Resouse/L_hands.obj", "Resouse/hands_one.png");
	BaseScene::mModel->SetAncPoint("ArmL", Vector3(0.0f, -2.1f, -0.1f));
	mSound = std::make_shared<Sound>("BGM/gameover.mp3", false);

	ParticleBox = make_shared<ParticleEmitterBox>(BaseScene::mParticle);
	ParticleBox->LoadAndSet("Bom", "Resouse/Bom.jpg");

	mTimer = std::make_shared<Timer>(0.01f);
	mParticleTimer = std::make_shared<Timer>(0.01f);

	mSound->setVol(BaseScene::mMasterSoundVol * BaseScene::mBGMSoundVol);

	mHit = std::make_shared<BlackSmoke>(Vector3(-pos - 6, 0, 1), true);
}

void GameOver::UpdateScene()
{
	mTimer->update();
	mParticleTimer->update();
	speed += 1;
	mSound->playLoop();
	camera->SetEye(camerapos);
	camera->SetTarget(setcamerapos);
	time += 1;

	if (!mTimer->isTime()) return;
	if (BaseScene::mMoney <= 0)
	{
		if (Input::getKeyDown(KeyCode::SPACE) || Input::getJoyDown(JoyCode::B))
		{
			BaseScene::mMoney = 20000000;
			NextScene(std::make_shared<Title>());
		}
	}
	else
	{
		if (Input::getKeyDown(KeyCode::SPACE) || Input::getJoyDown(JoyCode::B))
		{
			NextScene(std::make_shared<Select>());
		}
	}
	
	
}

void GameOver::DrawScene()
{
	DirectXManager::GetInstance()->SetData3D();
	BaseScene::mModel->Draw("TankPlayerA", Vector3(-pos-4, -2.5f, 4), Vector3(15, 40, 0), Vector3(2.5f, 2.5f, 2.5f));
	BaseScene::mModel->Draw("TankPlayerB", Vector3(-5-pos, 0, 0), Vector3(10, 15, 20), Vector3(1.5f, 1.5f, 1.5f));
	BaseScene::mModel->Draw("OjyouSama", Vector3(1, 1 + ojyouZ, -1), Vector3(90, -90, 0), Vector3(1.5f, 1.5f, 1.5f));
	BaseScene::mModel->Draw("Sora", Vector3(0, 2.0f, 0), Vector3(0, 0, 0), Vector3(7, 7, 7));
	BaseScene::mModel->Draw("Ground2", Vector3(-20.0f, 0.0f, -90.0f), Vector3(0, 0, 0), Vector3(300, 300, 300));
	objM->Draw();
	DirectXManager::GetInstance()->SetData2D();
	if (BaseScene::mMoney < 0)
	{
		BaseScene::mSprite->Draw("GEOver", posePos, 0.0f, Vector2(0.25f, 0.5f), Vector4(1, 1, 1, 1));
	}
	else
	{
		BaseScene::mSprite->Draw("GOver", posePos, 0.0f, Vector2(0.25f, 0.5f), Vector4(1, 1, 1, 1));
		Sequence::instance().set(500000, Vector2(1000, 350), Vector2(32, 32));
		BaseScene::mSprite->Draw("Shuri", Vector3(850, 350, 0), 0.0f, Vector2(0, 0), Vector4(1, 1, 1, 1));
		BaseScene::mSprite->Draw("\1", Vector3(950, 350, 0), 0.0f, Vector2(0, 0), Vector4(1, 1, 1, 1));
	}
	if (!mParticleTimer->isTime()) return;
	ParticleBox->EmitterUpdate("Bom", Vector3(-pos - 6, 0, 1), Vector3(0, 0, 0));
	mParticleTimer->setTime(0.1f);
}

void GameOver::FinalizeScene()
{
	delete objM;
}
