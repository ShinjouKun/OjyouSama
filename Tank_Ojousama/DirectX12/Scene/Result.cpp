#include "Result.h"
#include "GamePlay.h"
#include "Defense.h"
#include"Title.h"
#include"Select.h"
#include "../Sound/Sound.h"
#include "../Utility/Timer/Timer.h"
#include"../Utility/Sequence/Sequence.h"
#include"../Math/Math.h"

Result::Result()
	:mSound(nullptr)
{
}

Result::~Result()
{
}

void Result::StartScene()
{

	objM = new ObjectManager();
	objM->Claer();
	posePos = Vector3(0, 0, 0);
	BaseScene::mSprite->AddTexture("Pose", "Resouse/pose.png");
	BaseScene::mSprite->AddTexture("SelectAim", "Resouse/selectcursol.png");
	BaseScene::mSprite->AddTexture("SBack", "Resouse/selectback.png");
	BaseScene::mSprite->AddTexture("OptionP", "Resouse/option.png");
	BaseScene::mSprite->AddTexture("Ritorai", "Resouse/ritorai.png");
	//‹à
	BaseScene::mSprite->AddTexture("Plus", "Resouse/plus.png");
	BaseScene::mSprite->AddTexture("Minus", "Resouse/minus.png");
	BaseScene::mSprite->AddTexture("Plus2", "Resouse/plus.png");
	BaseScene::mSprite->AddTexture("Minus2", "Resouse/minus.png");
	BaseScene::mSprite->AddTexture("Gunsikin", "Resouse/gunnsikinn.png");
	BaseScene::mSprite->SetSize("Gunsikin", Vector2(640, 52));

	BaseScene::mSprite->AddTexture("Kakutoku", "Resouse/kakutokuhi.png");
	BaseScene::mSprite->AddTexture("Danyaku", "Resouse/dannyakuhi.png");
	BaseScene::mSprite->AddTexture("Goukei", "Resouse/goukei.png");
	BaseScene::mModel->AddModel("Sora2", "Resouse/skybox.obj", "Resouse/skybox_A.png");
	BaseScene::mModel->AddModel("Ground2", "Resouse/Plane.obj", "Resouse/Ground_Black.png");
	selectbackPos = Vector3(180, 180, 0);
	selectposition = Vector3(180, 180, 0);
	camerapos = Vector3(0,5,10);
	setcamerapos = Vector3(0,5,0);
	BaseScene::mModel->AddModel("TankPlayerA", "Resouse/houtou.obj", "Resouse/sensha_A.png");
	BaseScene::mModel->AddModel("TankPlayerB", "Resouse/sensha_body.obj", "Resouse/sensha_A.png");
	BaseScene::mModel->AddModel("ArmR", "Resouse/R_hands.obj", "Resouse/hands_one.png");
	BaseScene::mModel->SetAncPoint("ArmR", Vector3(0.0f, -2.1f, -0.1f));
	BaseScene::mModel->AddModel("OjyouSama", "Resouse/ojosama_body.obj", "Resouse/ojosama_one.png");
	BaseScene::mModel->SetAncPoint("OjyouSama", Vector3(0.0f, 0.0f, -0.1f));
	BaseScene::mModel->AddModel("ArmL", "Resouse/L_hands.obj", "Resouse/hands_one.png");
	BaseScene::mModel->SetAncPoint("ArmL", Vector3(0.0f, -2.1f, -0.1f));


	mSound = std::make_shared<Sound>("BGM/loop_157.mp3", false);
	mSound->setVol(BaseScene::mMasterSoundVol * BaseScene::mBGMSoundVol);
	mFanfare = std::make_shared<Sound>("SE/Fanfare.mp3", false);
	mFanfare->setVol(BaseScene::mMasterSoundVol * BaseScene::mSESoundVol);
	mFanfare->play();

	speed = 0;
	ojyouY = 0.0f;
	ojyouZ = 0.0f;
	ojyouXR = 7;
	ojyouXL = 7;
	angle = 0;
	zensin = 0;
	mMoney_PM = BaseScene::mPlusMoney - BaseScene::mMinusMoney;
	mTimer = std::make_shared<Timer>(0.01f);
	BaseScene::mMoney = (BaseScene::mMoney + BaseScene::mPlusMoney) - BaseScene::mMinusMoney;
}

void Result::UpdateScene()
{
	mTimer->update();
	speed += 0.1f;
	mSound->playLoop();
	ojyouY = ojyouY + speed;
	camera->SetEye(camerapos);
	camera->SetTarget(setcamerapos);
	camerapos.z -=0.15;
	camerapos.x -= 0.05;
	camerapos.y -= 0.01;
	time += 1;

	if (time >= 120)
	{
		camerapos.z += 0.15;
		camerapos.x += 0.05;
		camerapos.y += 0.01;
		if (selectposition.x <= 0)
		{
			selectposition.x = 820;
		}
		if (selectposition.x > 820)
		{
			selectposition.x = 180;
		}

		if (!mTimer->isTime()) return;
		if (Input::getKeyDown(KeyCode::A) || Input::joyHorizontal() < 0)
		{
			if (selectposition.y == 360)
			{
				return;
			}
			selectposition.x -= 640;
			mTimer->setTime(0.5f);
		}

		if (Input::getKeyDown(KeyCode::D) || Input::joyHorizontal() > 0)
		{
			if (selectposition.y == 360)
			{
				return;
			}
			selectposition.x += 640;
			mTimer->setTime(0.5f);
		}

		if (selectposition.x == 180)
		{
			if (Input::getKeyDown(KeyCode::SPACE) || Input::getJoyDown(JoyCode::B))
			{
				NextScene(std::make_shared<Select>());
			}
		}
		if (selectposition.x == 820)
		{
			if (Input::getKeyDown(KeyCode::SPACE) || Input::getJoyDown(JoyCode::B))
			{
				NextScene(std::make_shared<GamePlay>());
			}
		}
	}
}

void Result::DrawScene()
{


	DirectXManager::GetInstance()->SetData3D();
	BaseScene::mModel->Draw("TankPlayerA", Vector3(0, 0, zensin), Vector3(0, angle, 0), Vector3(1.5f, 1.5f, 1.5f));
	BaseScene::mModel->Draw("TankPlayerB", Vector3(0, 0, zensin), Vector3(0, angle, 0), Vector3(1.5f, 1.5f, 1.5f));
	BaseScene::mModel->Draw("ArmR", Vector3(0,3.2f , zensin), Vector3(150, ojyouY, 0), Vector3(1.5f, 1.5f, 1.5f));
	BaseScene::mModel->Draw("OjyouSama", Vector3(0,0 , zensin), Vector3(0, ojyouY, 0), Vector3(1.5f, 1.5f, 1.5f));
	BaseScene::mModel->Draw("ArmL", Vector3(0,3.2f , zensin), Vector3(150, ojyouY, 0), Vector3(1.5f, 1.5f, 1.5f));
	BaseScene::mModel->Draw("Sora2", Vector3(0, 2.0f, 0), Vector3(0, 0, 0), Vector3(7, 7, 7));
	BaseScene::mModel->Draw("Ground2", Vector3(-20.0f, 0.0f, 400.0f), Vector3(0, 0, 0), Vector3(500, 500, 500));
	objM->Draw();
	DirectXManager::GetInstance()->SetData2D();
	if (time >= 120)
	{
	
		zensin += 1.0f;
		ojyouZ += 0.1f;
		BaseScene::mSprite->Draw("SBack", selectbackPos, 0.0f, Vector2(0, 0), Vector4(1, 1, 1, 1));
		BaseScene::mSprite->Draw("Ritorai", Vector3(820, 180, 0), 0.0f, Vector2(0, 0), Vector4(1, 1, 1, 1));
		BaseScene::mSprite->Draw("SelectAim", selectposition, 0.0f, Vector2(0, 0), Vector4(1, 1, 1, 1));
	}
	else
	{
		Sequence::instance().set(BaseScene::mPlusMoney, Vector2(680, 115), Vector2(64, 64));
		BaseScene::mSprite->Draw("Plus", Vector3(600, 100, 0), 0.0f, Vector2(0, 0), Vector4(1, 1, 1, 1));
		BaseScene::mSprite->Draw("Kakutoku", Vector3(200,100,0), 0.0f, Vector2(0, 0), Vector4(1, 1, 1, 1));
		Sequence::instance().set(BaseScene::mMinusMoney, Vector2(680, 315), Vector2(64, 64));
		BaseScene::mSprite->Draw("Minus", Vector3(600, 300, 0), 0.0f, Vector2(0, 0), Vector4(1, 1, 1, 1));
		BaseScene::mSprite->Draw("Danyaku", Vector3(200, 300, 0), 0.0f, Vector2(0, 0), Vector4(1, 1, 1, 1));
		if (mMoney_PM < 0)
		{
			BaseScene::mSprite->Draw("Minus2", Vector3(500, 500, 0), 0.0f, Vector2(0, 0), Vector4(1, 1, 1, 1));
		}
		else
		{
			BaseScene::mSprite->Draw("Plus2", Vector3(500, 500, 0), 0.0f, Vector2(0, 0), Vector4(1, 1, 1, 1));
		}
		Sequence::instance().set(Math::abs(mMoney_PM), Vector2(600, 515), Vector2(64, 64));
		BaseScene::mSprite->Draw("Goukei", Vector3(200, 500, 0), 0.0f, Vector2(0, 0), Vector4(1, 1, 1, 1));
		angle += 1.5f;
	}
}

void Result::FinalizeScene()
{
	delete objM;
}
