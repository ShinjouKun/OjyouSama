#include "Player.h"

#include<sstream>
#include "../Device/Input.h"
#include "../Collision/BaseCollider.h"
#include "../Sound/Listener.h"
#include"../Sound/Sound.h"
#include "../Actor/CameraEye.h"
#include"../Utility/Timer/Timer.h"
//武器たち
#include"../Weapons/NormalBullet.h"
#include"../Weapons/LandMine.h"
#include"../Weapons/ShotGunBullet.h"
#include"../Weapons/MissileBullet.h"
#include"../Weapons/MashinGun.h"
#include"../Scene/BaseScene.h"
#include"../Utility/Sequence/Sequence.h"
#include"../Utility/ModelChanger.h"
#define ToRad(deg)((deg)*(PI/180.0f))
Player::Player(Vector3 pos, Vector3 ang, ObjectManager * obj, shared_ptr<ModelRenderer> m, shared_ptr<ParticleManager>p, shared_ptr<TexRenderer>s,int sceneE)
	:playerModel(m), playerParticle(p), playerSprite(s),mSound(nullptr),
	listener(std::make_shared<Listener>())
{
	position = pos;
	angle = ang;
	objM = obj;
	sceneEffectNum = sceneE;
}

Player::~Player()
{
}

void Player::StartCamScene()
{
	sceneCount++;
	switch (sceneEffectNum)
	{
	case 1:
		//殲滅戦
		position = Vector3(0, 0, -130);
		sceneCamPos = Vector3(120, 80, -130);
		camera->SetEye(Vector3(sceneCamPos.x,sceneCamPos.y,position.z));
		camera->SetTarget(Vector3(0, 0, position.z));
		break;
	case 2:
		break;
	case 3:
		break;
	case 4:
		break;
	default:
		break;
	}
	
}

void Player::SceneCamMove1()
{
	
	if (position.z >= 520)
	{
		sceneCamPlayerOk = true;
		if (sceneCamPos.x <= 0)
		{
			sceneCamOk = true;
		}
		else
		{
			sceneCamPos.x -= 3.0f;
			sceneCamPos.y -= 2.0f;
			camera->SetEye(Vector3(sceneCamPos.x, sceneCamPos.y, position.z));
			camera->SetTarget(Vector3(0, 0, position.z));
		}
	}
	else
	{
		position.z += 4.0f;
		camera->SetEye(Vector3(sceneCamPos.x, sceneCamPos.y, position.z));
		camera->SetTarget(Vector3(0, 0, position.z));
	}
}

void Player::SceneCamMove2()
{
}

void Player::SceneCamMove3()
{
}

void Player::SceneCamMove4()
{
}


void Player::UseWeapon1()
{
	
	objM->Add(new NormalBullet(Vector3(position.x, position.y + 1.5f, position.z), Vector3(fireAngle, -atkAngle, 0), objM, playerModel, playerParticle, objType, bulletStock));
	shotFlag1 = true;
}

void Player::UseWeapon2()
{
	//objM->Add(new MashinGun(Vector3(position.x, position.y + 1.5f, position.z), Vector3(fireAngle, -atkAngle, 0), objM, playerModel, playerParticle, objType, bulletStock));
	//objM->Add(new MissileBullet(Vector3(position.x, position.y, position.z), Vector3(0, 0, 0), objM, playerModel, playerParticle, objType, bulletStock));
	objM->Add(new LandMine(Vector3(position.x, position.y, position.z), Vector3(0,0,0), objM, playerModel, playerParticle, objType, bulletStock));
	/*objM->Add(new ShotGunBullet(Vector3(position.x, position.y - 0.15f, position.z), Vector3(fireAngle, -atkAngle+20.0f, 0), objM, playerModel, playerParticle, objType, bulletStock));
	objM->Add(new ShotGunBullet(Vector3(position.x, position.y - 0.15f, position.z), Vector3(fireAngle, -atkAngle+10.0f, 0), objM, playerModel, playerParticle, objType, bulletStock+1));
	objM->Add(new ShotGunBullet(Vector3(position.x, position.y - 0.15f, position.z), Vector3(fireAngle, -atkAngle, 0), objM, playerModel, playerParticle, objType, bulletStock+2));
	objM->Add(new ShotGunBullet(Vector3(position.x, position.y - 0.15f, position.z), Vector3(fireAngle, -atkAngle-10.0f, 0), objM, playerModel, playerParticle, objType, bulletStock+3));
	objM->Add(new ShotGunBullet(Vector3(position.x, position.y - 0.15f, position.z), Vector3(fireAngle, -atkAngle-20.0f, 0), objM, playerModel, playerParticle, objType, bulletStock+4));*/
	shotFlag2 = true;
}

void Player::UseULT()
{
}

void Player::Item()
{
}

void Player::AngleReset()
{
	//上限
	if (angle.x >= 360.0f)
	{
		angle.x = 0.0f;
	}
	if (angle.y >= 360.0f)
	{
		angle.y = 0.0f;
	}
	if (angle.z >= 360.0f)
	{
		angle.z = 0.0f;
	}

	if (atkAngle >= 360.0f)
	{
		atkAngle = 0.0f;
	}
	//下限
	if (angle.x <= -360.0f)
	{
		angle.x = 0.0f;
	}
	if (angle.y <= -360.0f)
	{
		angle.y = 0.0f;
	}
	if (angle.z <= -360.0f)
	{
		angle.z = 0.0f;
	}

	if (atkAngle <= -360.0f)
	{
		atkAngle = 0.0f;
	}
}


void Player::Init()
{
	
	mSound = std::make_shared<Sound>("bomb3.mp3", false);
	mTimer = std::make_shared<Timer>();


	playerSprite->AddTexture("HpUi", "Resouse/hpUI.png");
	playerSprite->AddTexture("WeponUi", "Resouse/wepon.png");
	//model
	//戦車
	//playerModel->AddModel("TankA", "Resouse/houtou.obj", "Resouse/sensha_A.png");
	//playerModel->AddModel("TankB", "Resouse/sensha_body.obj", "Resouse/sensha_A.png");
	///*playerModel->AddModel("TankA", "Resouse/big_sensha_head.obj", "Resouse/big_sensha.png");
	//playerModel->AddModel("TankB", "Resouse/big_sensha_body.obj", "Resouse/big_sensha.png");*/
	////お嬢様
	//playerModel->AddModel("ArmR", "Resouse/R_hands.obj", "Resouse/hands_one.png");
	//playerModel->SetAncPoint("ArmR", Vector3(0.0f, -2.1f, -0.1f));
	//playerModel->AddModel("OjyouSama", "Resouse/ojosama_body.obj", "Resouse/ojosama_one.png");
	//playerModel->SetAncPoint("OjyouSama", Vector3(0.0f, 0.0f, -0.1f));
	//playerModel->AddModel("ArmL", "Resouse/L_hands.obj", "Resouse/hands_one.png");
	//playerModel->SetAncPoint("ArmL", Vector3(0.0f, -2.1f, -0.1f));

	modelChanger = new ModelChanger();
	modelChanger->Load(playerModel);

	//playerParticleBox = make_shared<ParticleEmitterBox>(playerParticle);
	//playerParticleBox->LoadAndSet("KemuriL","Resouse/tuti.jpg");
	//playerParticleBox->LoadAndSet("KemuriR", "Resouse/tuti.jpg");
	//HP
	maxHP = 100;
	HP = maxHP;
	playerSprite->AddTexture("DETH", "Resouse/Deth.png");
	playerSprite->AddTexture("UI", "Resouse/TankUI.png");
	playerSprite->AddTexture("AIM", "Resouse/AIM64.png");
	playerSprite->AddTexture("AIM_S", "Resouse/croshear.png");
	/*playerSprite->AddTexture("Life1", "Resouse/TankAicn.png");
	playerSprite->AddTexture("Life2", "Resouse/TankAicn.png");
	playerSprite->AddTexture("Life3", "Resouse/TankAicn.png");
	playerSprite->AddTexture("HIT", "Resouse/hit.png");*/
	death = false;
	objType = ObjectType::PLAYER;
	CamPos_Y = 2.5f;
	TargetPos.y = 2.5f;
	angle = Vector3(0.0f, 0.0f, 0.0f);//車体
	atkAngle = 0.0f;//砲塔
	aimPos_Y = 360.0f;
	fireAngle = 0.0f;
	speed = 0.0f;
	maxSpeed = 0.5f;
	speedTime = 0.0f;
	speedLimitTime = 10.0f;
	cameraSpeed = 1.0f;
	bulletStock = 0;
	HitFlag = false;
	sniperShotFlag = false;
	HitCount = 0;
	sceneCount = 0;
	sceneCamOk = false;
	sceneCamPlayerOk = false;
	CameraPos = Vector3(position.x, position.y, position.z + 15.0f);

	//コライダーの情報をセット
	SetCollidder(Vector3(0,0,0), 1.0f);
	ojyouY = 0.0f;
	ojyouXR = 0.0f;
	ojyouXL = 0.0f;

	StartCamScene();//演出決定
	//SetCollidder(Vector3(position.x, position.y, position.z), Vector3(2.0f,2.0f,2.0f));
}

void Player::Update()
{
	mTimer->update();

	mSound->setVol(BaseScene::mMasterSoundVol*BaseScene::mSESoundVol);
	//シーン演出 
#pragma region シーン
	switch (sceneEffectNum&&!sceneCamOk)
	{
	case 1:
		SceneCamMove1();
		break;
	case 2:
		SceneCamMove2();
		break;
	case 3:
		SceneCamMove3();
		break;
	case 4:
		SceneCamMove4();
		break;
	default:
		break;
	}
#pragma endregion
	
	
	if (!GameOver&&sceneCamOk)
	{
		if (HP <= 0)
		{
			GameOver = true;
		}
		if (HitFlag)
		{
			HitCount++;
			if (HitCount >= 90)
			{
				HitCount = 0;
				HitFlag = false;
			}
		}
		
		velocity = Vector3(0, 0, 0);
		speed = Easing::ease_inout_cubic(speedTime, 0, maxSpeed, 20.0f);
		velocity = RotateY(angle.y + 90.0f)*speed;

		CamVelocity = Vector3(0, 0, 0);
		moveFlag = false;
	
		ImGuiDebug();//デバッグ用
		AngleReset();
		if (FrontMove)
		{
			position += velocity;
		}
		else if (BackMove)
		{
			position -= velocity;
		}
		//キー押し処理
		if (Input::getKey(KeyCode::W) || Input::joyVertical() > 0)
		{
			speedTime++;
			if (speedTime >= speedLimitTime)
			{
				speedTime = speedLimitTime;
			}
			FrontMove = true;
			BackMove = false;
			moveFlag = true;
		}
		else if (Input::getKey(KeyCode::S) || Input::joyVertical() < 0)
		{
			speedTime++;
			if (speedTime >= speedLimitTime)
			{
				speedTime = speedLimitTime;
			}
			BackMove = true;
			FrontMove = false;
		}
		else if (speedTime == speedLimitTime || speedTime < speedLimitTime)
		{
			speedTime--;
			if (speedTime <= 0.0f)
			{
				speedTime = 0;
			}

		}
		if (Input::getKey(KeyCode::D)|| Input::joyHorizontal() > 0)
		{
			angle.y += 1.0f;
		}
		if (Input::getKey(KeyCode::A) || Input::joyHorizontal()< 0)
		{
			angle.y -= 1.0f;
		}

		//砲塔
		if (Input::getKey(KeyCode::UPARROW) || Input::joyRightVertical() > 0)
		{
			aimPos_Y -= 4.5f;
			CamPos_Y -= 0.04f;
			TargetPos.y += 0.02f;
			fireAngle -= 1.0f;
		}
		if (fireAngle <= -25.0f)
		{
			fireAngle = -25.0f;//上限
		}
		if (aimPos_Y <= 120.0f)
		{
			aimPos_Y = 120.0f;
		}
		if (TargetPos.y >= 3.0f)
		{
			TargetPos.y = 3.0f;
			CamPos_Y = 1.6f;
		}
		if (Input::getKey(KeyCode::DOWNARROW) || Input::joyRightVertical()<0)
		{
			aimPos_Y += 4.5f;
			CamPos_Y += 0.04f;
			TargetPos.y -= 0.02f;
			fireAngle += 1.0f;
		}
		if (fireAngle >= 0.0f)
		{
			fireAngle = 0.0f;//下限
		}
		if (aimPos_Y >= 360.0f)
		{
			aimPos_Y = 360.0f;
		}
		if (TargetPos.y <= 2.5f)
		{
			CamPos_Y = 2.5f;
			TargetPos.y = 2.5f;
		}
		if (Input::getKey(KeyCode::RIGHTARROW) || Input::joyRightHorizontal()>0)
		{
			atkAngle += cameraSpeed;
		}
		if (Input::getKey(KeyCode::LEFTARROW)|| Input::joyRightHorizontal()<0)
		{
			atkAngle -= cameraSpeed;
		}
		//行動制限
		//右
		if (position.x <= -150.0f)
		{
			position.x = -150.0f;
		}
		//左
		if (position.x >= 150.0f)
		{
			position.x = 150.0f;
		}
		//後ろ
		if (position.z >= 510)
		{
			position.z = 510;
		}
		//前
		if (position.z <= -250)
		{
			position.z = -250;
		}
		//カメラ更新
		
		
		if(Input::getKeyDown(KeyCode::Q)|| Input::getJoyDown(JoyCode::LeftButton))
		{
			if (mTimer->isTime()) {
				sniperShotFlag = !sniperShotFlag;
				mTimer->setTime(0.2f);
			}

		}
		if (!sniperShotFlag)
		{
			CamPos_Y = 2.5f;
			CamVelocity = RotateY(atkAngle - 90.0f)*8.0f;
			CameraPos = position + CamVelocity;
			camera->SetEye(Vector3(CameraPos.x, CameraPos.y + 6.0f, CameraPos.z));
			camera->SetTarget(Vector3(position.x, position.y + 6.0f, position.z));
		}
		else
		{
			CamVelocity = RotateY(atkAngle - 90.0f)*2.1f;
			CameraPos = position + CamVelocity;
			camera->SetEye(Vector3(CameraPos.x, CameraPos.y + CamPos_Y, CameraPos.z));
			camera->SetTarget(Vector3(position.x, position.y+TargetPos.y, position.z));
		}
		


		if (shotFlag1)
		{
			int t = objM->GetReloadTime();
			shotcnt1++;
			if (shotcnt1 >= t)
			{
				shotFlag1 = false;
			}
		}
		else
		{
			if (Input::getKeyDown(KeyCode::SPACE) || Input::getJoyDown(JoyCode::RightButton))//Rトリガー
			{
				UseWeapon1();
				bulletStock++;
				shotcnt1 = 0;
				mSound->play();
			}
		}
		if (shotFlag2)
		{
			int t = objM->GetReloadTime();
			shotcnt2++;
			if (shotcnt2 >= t)
			{
				shotFlag2 = false;
			}
		}
		else
		{
			if (Input::getKeyDown(KeyCode::LEFTSHIFT) || Input::getJoyDown(JoyCode::Y))//Yボタン
			{
				UseWeapon2();
				bulletStock++;
				shotcnt2 = 0;
			}
		}

		//球数上限を設け
		if (bulletStock >= 70)
		{
			bulletStock = 0;
		}

	}

	//リスナーに自身の位置を更新
	listener->setPos(position);
}

void Player::Rend()
{
	Sequence::instance().set(HP, Vector2(64, 0), Vector2(64, 64));
	ojyouXR -= 10.0f;
	ojyouXL += 10.0f;
	ojyouY -= 10.0f;
	DirectXManager::GetInstance()->SetData3D();//モデル用をセット
	if (!sniperShotFlag&&sceneCamPlayerOk)
	{
		playerModel->Draw(modelChanger->GetModelName(3), Vector3(position.x, position.y, position.z), Vector3(0, -atkAngle, 0), Vector3(1.5f, 1.5f, 1.5f));
		playerModel->Draw(modelChanger->GetModelName(4), Vector3(position.x, position.y, position.z), Vector3(0, -angle.y, 0), Vector3(1.5f, 1.5f, 1.5f));

		playerModel->Draw(modelChanger->GetModelName(0), Vector3(position.x, position.y + 3.2f, position.z), Vector3(ojyouXR, -ojyouY, 0), Vector3(1.5f, 1.5f, 1.5f));
		playerModel->Draw(modelChanger->GetModelName(1), Vector3(position.x, position.y, position.z), Vector3(0, -ojyouY, 0), Vector3(1.5f, 1.5f, 1.5f));
		playerModel->Draw(modelChanger->GetModelName(2), Vector3(position.x, position.y + 3.2f, position.z), Vector3(ojyouXL, -ojyouY, 0), Vector3(1.5f, 1.5f, 1.5f));
	}

	//if (moveFlag)
	//{
	//	playerParticleBox->EmitterUpdateUpGas("KemuriL", Vector3(position.x - 0.8f, position.y+0.5f, position.z + 1.8f), Vector3(angle.x, angle.y, angle.z));
	//	playerParticleBox->EmitterUpdateUpGas("KemuriR", Vector3(position.x + 0.8f, position.y+0.5f, position.z + 1.8f), Vector3(angle.x, angle.y, angle.z));
	//}

	DirectXManager::GetInstance()->SetData2D();
	playerSprite->Draw("HpUi", Vector3(0, 0, 0), 0.0f, Vector2(1, 1), Vector4(1, 1, 1, 1));
	playerSprite->Draw("WeponUi", Vector3(1280 -180, 720-180, 0), 0.0f, Vector2(1, 1), Vector4(1, 1, 1, 1));
	if (!sniperShotFlag)
	{
		playerSprite->Draw("AIM", Vector3((Window::Window_Width / 2) - 32, aimPos_Y, 0), 0.0f, Vector2(1, 1), Vector4(1, 1, 1, 1));
	}
	else
	{
		playerSprite->Draw("AIM_S", Vector3(0,0,0), 0.0f, Vector2(1, 1), Vector4(1, 1, 1, 1));
	}
	
	if (GameOver)
	{
		
		DirectXManager::GetInstance()->SetData2D();
		playerSprite->Draw("DETH", Vector3(500, 200, 0), 0.0f, Vector2(0, 0), Vector4(1, 1, 1, 1));
	}

}

void Player::SetHP(int value)
{
	HP = value;
}


void Player::OnCollison(BaseCollider* col)
{
	if (!HitFlag)
	{
		if (col->GetColObject()->GetType() == ObjectType::ENEMYBULLET
			|| col->GetColObject()->GetType() == ObjectType::ENEMY
			|| col->GetColObject()->GetType() == ObjectType::BOSS)
		{
			HP -= col->GetColObject()->GetDamage();
			HitFlag = true;
		}
	}

	if (col->GetColObject()->GetType() == ObjectType::BLOCK)
	{
		if (FrontMove)//いずれ修正
		{
			position -= velocity;//前方移動のみ
		}
		else if (BackMove)
		{
			position += velocity;//後方移動のみ
		}
		DirectXManager::GetInstance()->SetData2D();

	}
}

void Player::ImGuiDebug()
{
#ifdef _DEBUG



	float pos[3] = { position.x,position.y,position.z };
	ImGui::SliderFloat3("PlayerPosition", pos, 0, 10000.0f);

	float posC[3] = { CameraPos.x,CameraPos.y,CameraPos.z };
	ImGui::SliderFloat3("CameraPosition", posC, 0, 10000.0f);

	float ang[3] = { angle.x,angle.y,angle.z };
	ImGui::SliderFloat3("BtmAngle", ang, 0, 360);
	ImGui::SliderFloat("AtkAngle", &atkAngle, 0, 360);
	ImGui::SliderFloat("FireAngle", &fireAngle, 0, 360);
	ImGui::SliderFloat("CamPosY", &CamPos_Y, -10, 10);
	ImGui::Checkbox("ShotFlag", &shotFlag1);
	ImGui::Checkbox("ShotFlag", &shotFlag2);
	ImGui::SliderInt("HP", &HP, 0, HP);
	ImGui::SliderInt("mainWeapon", &shotcnt1, 0, objM->GetReloadTime());
	ImGui::SliderInt("subWeapon", &shotcnt2, 0, objM->GetReloadTime());
	ImGui::SliderFloat("SPEED", &speed, 0, 100);
	ImGui::SliderInt("SPEEDTime", &speedTime, 0, 100);

#endif  _DEBUG
}
