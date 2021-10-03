#include "Player.h"

#include<sstream>
#include "../Device/Input.h"
#include "../Collision/BaseCollider.h"
#include "../Sound/Listener.h"

//武器たち
#include"../Weapons/NormalBullet.h"
#include"../Weapons/LandMine.h"
#include"../Weapons/ShotGunBullet.h"
#include"../Weapons/MissileBullet.h"
#include"../Weapons/MashinGun.h"
Player::Player(Vector3 pos, Vector3 ang, ObjectManager * obj,shared_ptr<ModelRenderer> m, shared_ptr<ParticleManager>p, shared_ptr<TexRenderer>s)
	:playerModel(m),playerParticle(p),playerSprite(s),
	listener(std::make_shared<Listener>())
{
	position = pos;
	angle = ang;
	objM = obj;
}

Player::~Player()
{
}


void Player::UseWeapon1()
{
	objM->Add(new NormalBullet(Vector3(position.x, position.y - 0.15f, position.z), Vector3(fireAngle, -atkAngle, 0), objM, playerModel, playerParticle, objType, bulletStock));
	shotFlag1 = true;
}

void Player::UseWeapon2()
{
	objM->Add(new MashinGun(Vector3(position.x, position.y - 0.15f, position.z), Vector3(fireAngle, -atkAngle, 0), objM, playerModel, playerParticle, objType, bulletStock));
	//objM->Add(new MissileBullet(Vector3(position.x, position.y, position.z), Vector3(0, 0, 0), objM, playerModel, playerParticle, objType, bulletStock));
	//objM->Add(new LandMine(Vector3(position.x, position.y, position.z), Vector3(0,0,0), objM, playerModel, playerParticle, objType, bulletStock));
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
	//model
	playerModel->AddModel("TankPlayerA", "Resouse/BoxTankATKA.obj","Resouse/BoxTankATKA.png");
	playerModel->SetAncPoint("TankPlayerA", Vector3(-2.0f, -2.0f, -2.0f));

	playerModel->AddModel("TankPlayerHou", "Resouse/BoxTankATKB.obj", "Resouse/BoxTankATKB.png");
	playerModel->SetAncPoint("TankPlayerHou", Vector3(-2.0f,-2.0f, -2.0f));

	playerModel->AddModel("TankPlayerB", "Resouse/BoxTankBTM.obj", "Resouse/BoxTankBTM.png");
	playerModel->SetAncPoint("TankPlayerB", Vector3(-2.0f, -2.0f, -2.0f));

	//playerModel->AddModel("TankPlayerA", "Resouse/head.obj", "Resouse/head2.png");
	//playerModel->AddModel("TankPlayerB", "Resouse/body.obj", "Resouse/body2.png");
	//playerParticleBox = make_shared<ParticleEmitterBox>(playerParticle);
	//playerParticleBox->LoadAndSet("KemuriL","Resouse/tuti.jpg");
	//playerParticleBox->LoadAndSet("KemuriR", "Resouse/tuti.jpg");
	//HP
	HP = 3;
	playerSprite->AddTexture("DETH", "Resouse/Deth.png");
	playerSprite->AddTexture("UI", "Resouse/TankUI.png");
	playerSprite->AddTexture("Life1","Resouse/TankAicn.png");
	playerSprite->AddTexture("Life2","Resouse/TankAicn.png");
	playerSprite->AddTexture("Life3","Resouse/TankAicn.png");
    playerSprite->AddTexture("HIT","Resouse/hit.png");
	death = false;
	objType = ObjectType::PLAYER;
	position = Vector3(100.0f, 0.0f, -50.0f);
	
	angle = Vector3(0.0f, 0.0f, 0.0f);//車体
	atkAngle = 0.0f;//砲塔
	fireAngle = 0.0f;
	speed = 0.3f;	
	cameraSpeed = 1.0f;
	bulletStock = 0;
	TargetPos = Vector3(position.x, position.y + 4.0f, position.z);
	CameraPos = Vector3(position.x, position.y, position.z + 15.0f);
	//コライダーの情報をセット
	SetCollidder(Vector3(position.x, position.y, position.z), 1.0f);
	//SetCollidder(Vector3(position.x, position.y, position.z), Vector3(position.x + 2.0f, position.y + 2.0f, position.z + 2.0f));
}

void Player::Update()
{
	if (!GameOver)
	{
		velocity = Vector3(0, 0, 0);
		CamVelocity = Vector3(0, 0, 0);
		moveFlag = false;
		FrontMove = false;
		BackMove = false;
		CameraPos = Vector3(position.x, position.y+4.0f, position.z +15.0f);
		ImGuiDebug();//デバッグ用
		AngleReset();
		
		if (position.x <= 0)
		{
			position.x= 0.1f;//バミューダ対策
		}
		//if (position.y > -3.9f)
		//{
		//	position.y -= 0.2f;//重力
		//}
		//テスト用
		if (Input::KeyState(DIK_2))
		{
			position.y += 0.4f;
		}
		//キー押し処理
		if (Input::KeyState(DIK_W)||Input::pad_data.lY<0)
		{
			velocity = RotateY(angle.y + 90.0f)*speed;
			position += velocity;
			FrontMove = true;
			moveFlag = true;
		}

		if (Input::KeyState(DIK_S) || Input::pad_data.lY > 0)
		{
			velocity = RotateY(angle.y + 90.0f)*speed;
			position -= velocity;
			BackMove = true;
		}

		if (Input::KeyState(DIK_D) || Input::pad_data.lX > 0)
		{
			angle.y += 1.0f;
		}
		if (Input::KeyState(DIK_A) || Input::pad_data.lX < 0)
		{
			angle.y -= 1.0f;
		}
		//砲塔
		if (Input::KeyState(DIK_UP)|| Input::pad_data.lRz < 0)
		{
			fireAngle -= 1.0f;
		}
		if (fireAngle <= -25.0f)
		{
			fireAngle = -25.0f;//上限
		}
		if (Input::KeyState(DIK_DOWN) || Input::pad_data.lRz > 0)
		{
			fireAngle += 1.0f;
		}
		if (fireAngle >= 0.0f)
		{
			fireAngle = 0.0f;//下限
		}
		if (Input::KeyState(DIK_RIGHT) || Input::pad_data.lZ > 0)
		{
			atkAngle += cameraSpeed;
		}
		if (Input::KeyState(DIK_LEFT) || Input::pad_data.lZ < 0)
		{
			atkAngle -= cameraSpeed;
		}
		//カメラ更新
		CamVelocity = RotateY(atkAngle - 90.0f)*8.0f;
		CameraPos = position + CamVelocity;
		camera->SetEye(Vector3(CameraPos.x, CameraPos.y+4.0f, CameraPos.z));
		camera->SetTarget(Vector3(position.x, position.y+4.0f, position.z));
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
			if (Input::KeyState(DIK_SPACE) || Input::pad_data.rgbButtons[7])//Rトリガー
			{
				UseWeapon1();
				bulletStock++;
				shotcnt1 = 0;
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
			if (Input::KeyState(DIK_LSHIFT) || Input::pad_data.rgbButtons[1])//Yボタン
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
	
	DirectXManager::GetInstance()->SetData3D();//モデル用をセット
	playerModel->Draw("TankPlayerA", Vector3(position.x, position.y, position.z), Vector3(0, -atkAngle, 0), Vector3(1.5f, 1.5f, 1.5f));
	playerModel->Draw("TankPlayerHou", Vector3(position.x, position.y, position.z), Vector3(fireAngle, -atkAngle, 0), Vector3(1.5f, 1.5f, 1.5f));
	playerModel->Draw("TankPlayerB", Vector3(position.x, position.y, position.z), Vector3(0, -angle.y, 0), Vector3(1.5f, 1.5f, 1.5f));
	
	//if (moveFlag)
	//{
	//	playerParticleBox->EmitterUpdateUpGas("KemuriL", Vector3(position.x - 0.8f, position.y+0.5f, position.z + 1.8f), Vector3(angle.x, angle.y, angle.z));
	//	playerParticleBox->EmitterUpdateUpGas("KemuriR", Vector3(position.x + 0.8f, position.y+0.5f, position.z + 1.8f), Vector3(angle.x, angle.y, angle.z));
	//}

	DirectXManager::GetInstance()->SetData2D();
	playerSprite->Draw("UI", Vector3(0, 0, 0), 0.0f, Vector2(0, 0), Vector4(1, 1, 1, 1));
	switch (HP)
	{
	case 3:
		playerSprite->Draw("Life1", Vector3(0, 20, 0), 0.0f, Vector2(0, 0), Vector4(1, 1, 1, 1));
		playerSprite->Draw("Life2", Vector3(45, 20, 0), 0.0f, Vector2(0, 0), Vector4(1, 1, 1, 1));
		playerSprite->Draw("Life3", Vector3(90, 20, 0), 0.0f, Vector2(0, 0), Vector4(1, 1, 1, 1));
		break;
	case 2:
		playerSprite->Draw("Life1", Vector3(0, 20, 0), 0.0f, Vector2(0, 0), Vector4(1, 1, 1, 1));
		playerSprite->Draw("Life2", Vector3(45, 20, 0), 0.0f, Vector2(0, 0), Vector4(1, 1, 1, 1));
		break;
	case 1:
		playerSprite->Draw("Life1", Vector3(0, 20, 0), 0.0f, Vector2(0, 0), Vector4(1, 1, 1, 1));
		break;
	case 0:
		GameOver = true;
		break;
	default:
		break;
	}
	if (GameOver)
	{
		DirectXManager::GetInstance()->SetData2D();
		playerSprite->Draw("DETH", Vector3(500, 200, 0), 0.0f, Vector2(0, 0), Vector4(1, 1, 1, 1));
	}
	
}


void Player::OnCollison(BaseCollider* col)
{
	if (col->GetColObject()->GetType() == ObjectType::ENEMYBULLET)
	{
		HP -= col->GetColObject()->GetDamage();
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
	
	float pos[3] = { position.x,position.y,position.z };
	ImGui::SliderFloat3("PlayerPosition", pos, 0, 10000.0f);
	float ang[3] = { angle.x,angle.y,angle.z };
	ImGui::SliderFloat3("BtmAngle", ang, 0, 360);
	ImGui::SliderFloat("AtkAngle", &atkAngle, 0, 360);
	ImGui::SliderFloat("FireAngle", &fireAngle, 0, 360);
	ImGui::Checkbox("ShotFlag", &shotFlag1);
	ImGui::Checkbox("ShotFlag", &shotFlag2);
	ImGui::SliderInt("mainWeapon", &shotcnt1, 0, objM->GetReloadTime());
	ImGui::SliderInt("subWeapon", &shotcnt2, 0, objM->GetReloadTime());
}
