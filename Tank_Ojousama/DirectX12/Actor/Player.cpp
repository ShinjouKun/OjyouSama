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
#include"../Items/ItemHolder.h"

#include "../ParticleSystem/ParticleType/Bom.h"
#include "../ParticleSystem/ParticleType/MachineGunAttackParticle.h"
#include "../ParticleSystem/ParticleType/NormalAttackParticle.h"
#include "../ParticleSystem/ParticleType/TankTrajectory.h"

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
		//強奪
		break;
	case 3:
		//タワーディフェンス
		position = Vector3(0, 0, 500.0f);
		sceneCamPlayerOk = true;
		sceneCamPos = Vector3(-70.0f, 100.0f, 450.0f);
		camera->SetEye(Vector3(sceneCamPos.x, sceneCamPos.y, position.z));
		camera->SetTarget(Vector3(0,0, position.z));
		break;
	case 4:
		//ボス
		//殲滅戦
		position = Vector3(0, 0, -130);
		sceneCamPos = Vector3(120, 80, -130);
		camera->SetEye(Vector3(sceneCamPos.x, sceneCamPos.y, position.z));
		camera->SetTarget(Vector3(0, 0, position.z));
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
	camera->SetEye(Vector3(sceneCamPos.x, sceneCamPos.y, sceneCamPos.z - 50.0f));
	camera->SetTarget(Vector3(position.x, position.y, position.z + 50.0f));
	if(sceneCamPos.y <= 4.0f)
	{
		
		sceneCamPos.x = position.x -= sceneCamPos.x * 0.03f;
		if (sceneCamPos.x == position.x)
		{
			sceneCamPos.z += 0.5f;
			if (sceneCamPos.z >= 550.0f)
			{
				sceneCamOk = true;
			}
		}
	}
	else
	{
		sceneCamPos.x += 0.5f;
		sceneCamPos.y -= 0.3f;
	}
}

void Player::SceneCamMove4()
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


void Player::UseWeapon1()
{
	objM->Add(new NormalBullet(Vector3(position.x, position.y + 1.5f, position.z), Vector3(fireAngle, -atkAngle, 0), objM, playerModel, playerParticle, objType, bulletStock,UpDamage));
	shotFlag1 = true;
	mNormalAtkParticle->setPos(position);//後で調整
	mNormalAtkParticle->Play();
}

void Player::UseWeapon2()
{
	
	if (modelChanger->GetWeaponState1() == WeaponsState::MachinGun)
	{
		masingunShot = true;
		objM->Add(new MashinGun(Vector3(position.x, position.y + 1.5f, position.z), Vector3(fireAngle, -atkAngle, 0), objM, playerModel, playerParticle, objType, bulletStock, UpDamage));
		mMGAParticle->setPos(position);//後で調整
		mMGAParticle->Play();
	}
	if (modelChanger->GetWeaponState1() == WeaponsState::Mine)
	{
		objM->Add(new LandMine(Vector3(position.x, position.y, position.z), Vector3(0, 0, 0), objM, playerModel, playerParticle, objType, bulletStock, UpDamage));
	}
	if (modelChanger->GetWeaponState1() == WeaponsState::ShotGun)
	{
		objM->Add(new ShotGunBullet(Vector3(position.x, position.y - 0.15f, position.z), Vector3(fireAngle, -atkAngle + 20.0f, 0), objM, playerModel, playerParticle, objType, bulletStock, UpDamage));
		objM->Add(new ShotGunBullet(Vector3(position.x, position.y - 0.15f, position.z), Vector3(fireAngle, -atkAngle + 10.0f, 0), objM, playerModel, playerParticle, objType, bulletStock + 1, UpDamage));
		objM->Add(new ShotGunBullet(Vector3(position.x, position.y - 0.15f, position.z), Vector3(fireAngle, -atkAngle, 0), objM, playerModel, playerParticle, objType, bulletStock + 2, UpDamage));
		objM->Add(new ShotGunBullet(Vector3(position.x, position.y - 0.15f, position.z), Vector3(fireAngle, -atkAngle - 10.0f, 0), objM, playerModel, playerParticle, objType, bulletStock + 3, UpDamage));
		objM->Add(new ShotGunBullet(Vector3(position.x, position.y - 0.15f, position.z), Vector3(fireAngle, -atkAngle - 20.0f, 0), objM, playerModel, playerParticle, objType, bulletStock + 4, UpDamage));
	}

	//objM->Add(new MissileBullet(Vector3(position.x, position.y, position.z), Vector3(0, 0, 0), objM, playerModel, playerParticle, objType, bulletStock));
	//

	shotFlag2 = true;
}

void Player::UseULT()
{
}

void Player::Item()
{
	ItemHolder::GetInstance()->UseItem();
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
	
	mSound = std::make_shared<Sound>("SE/bomb3.mp3", false);
	mTimer = std::make_shared<Timer>();

	//パーティクル初期化
	mNormalAtkParticle = std::make_shared<NormalAttackParticle>(Vector3::zero, true);
	mMGAParticle       = std::make_shared<MachineGunAttackParticle>(Vector3::zero, true);
	mTankTra           = std::make_shared<TankTrajectory>(Vector3::zero, true);
	mBom               = std::make_shared<Bom>(Vector3::zero, true);

	mNormalAtkParticle->Stop();
	mMGAParticle->Stop();
	mTankTra->Stop();
	mBom->Stop();

	playerSprite->AddTexture("HpUi", "Resouse/hpUI.png");
	playerSprite->AddTexture("HpGage", "Resouse/hpgage.png");
	playerSprite->AddTexture("WeponUi", "Resouse/wepon.png");
	playerSprite->AddTexture("Blood", "Resouse/blood.png");
	playerSprite->AddTexture("Blood2", "Resouse/blood2.png");
	playerSprite->AddTexture("Blood3", "Resouse/blood3.png");
	//model
	modelChanger = new ModelChanger();
	modelChanger->Load(playerModel);
	//HP
	maxHP = modelChanger->GetHP();
	HP = maxHP;
	maxSpeed = modelChanger->GetSpeed();
	playerSprite->AddTexture("DETH", "Resouse/Deth.png");
	playerSprite->AddTexture("UI", "Resouse/TankUI.png");
	playerSprite->AddTexture("AIM", "Resouse/AIM64.png");
	playerSprite->AddTexture("AIM_S", "Resouse/croshear.png");
	
	death = false;
	SetTresureGet(false);//宝未入手
	objType = ObjectType::PLAYER;
	CamPos_Y = 2.5f;
	TargetPos.y = 2.5f;
	angle = Vector3(0.0f, 0.0f, 0.0f);//車体
	atkAngle = 0.0f;//砲塔
	aimPos_Y = 360.0f;
	fireAngle = 0.0f;
	speed = 0.0f;
	
	speedTime = 0.0f;
	speedLimitTime = 10.0f;
	cameraSpeed = 1.0f;
	bulletStock = 0;
	HitFlag = false;
	sniperShotFlag = false;
	masingunShot = false;
	HitCount = 0;
	sceneCamFinish = false;
	sceneCamOk = false;
	sceneCamPlayerOk = false;
	damageFade = false;
	damageFadeYpos = 0;
	CameraPos = Vector3(position.x, position.y, position.z + 15.0f);

	//コライダーの情報をセット
	SetCollidder(Vector3(0,0,0), 1.0f);
	ojyouY = 0.0f;
	ojyouZ = 0.0f;
	ojyouXR = 0.0f;
	ojyouXL = 0.0f;
	ojyouZR = 0.0f;
	ojyouZL = 0.0f;

	item = new ItemHolder();
	StartCamScene();//演出決定
	//SetCollidder(Vector3(position.x, position.y, position.z), Vector3(2.0f,2.0f,2.0f));
}

void Player::Update()
{
	if (HP <= 0)
	{
		ojyouZ += 1.0f;
	}
	mTimer->update();

	mSound->setVol(BaseScene::mMasterSoundVol*BaseScene::mSESoundVol);

	//シーン演出 
#pragma region シーン
	if (!sceneCamOk)
	{
		switch (sceneEffectNum)
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
	}
	else
	{
		sceneCamFinish = true;
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
			damageFade -= 0.01f;
			damageFadeYpos -= 0.2f;
			if (HitCount >= 90)
			{
				HitCount = 0;
				damageFade = 1.0f;
				damageFadeYpos = 0.0f;
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
			mTankTra->setPos(position);//後で調整
			mTankTra->Play();
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
			mTankTra->setPos(position);//後で調整
			mTankTra->Play();
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
	
		if (Input::getKeyDown(KeyCode::E) || Input::getJoyDown(JoyCode::X))
		{
			Item();
		}
		
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
			if (Input::getKeyDown(KeyCode::SPACE) || Input::getRightTriggerDown())//Rトリガー
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
			if (Input::getKey(KeyCode::LEFTSHIFT) || Input::getJoy(JoyCode::RightButton))//Yボタン
			{
				UseWeapon2();
				bulletStock++;
				shotcnt2 = 0;
			}
			else
			{
				masingunShot = false;
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
	if (!masingunShot)
	{
		ojyouXR = -90.0f;
		ojyouXL = -90.0f;
		ojyouZR = 45.0f;
		ojyouZL = -45.0f;
	}
	else
	{
		ojyouXR = 0.0f;
		ojyouXL = 0.0f;
		ojyouZR = 0.0f;
		ojyouZL = 0.0f;
	}
	DirectXManager::GetInstance()->SetData3D();//モデル用をセット
	if (!sniperShotFlag&&sceneCamPlayerOk)
	{
		playerModel->Draw(modelChanger->GetModelName(3), Vector3(position.x, position.y, position.z), Vector3(0, -atkAngle, 0), Vector3(1.5f, 1.5f, 1.5f));
		playerModel->Draw(modelChanger->GetModelName(4), Vector3(position.x, position.y, position.z), Vector3(0, -angle.y, 0), Vector3(1.5f, 1.5f, 1.5f));

		playerModel->Draw(modelChanger->GetModelName(0), Vector3(position.x-0.2f, position.y + 3.2f + ojyouZ, position.z), Vector3(ojyouXR, -ojyouY, ojyouZR), Vector3(1.5f, 1.5f, 1.5f));
		playerModel->Draw(modelChanger->GetModelName(1), Vector3(position.x, position.y + ojyouZ, position.z), Vector3(0, -ojyouY, 0), Vector3(1.5f, 1.5f, 1.5f));
		playerModel->Draw(modelChanger->GetModelName(2), Vector3(position.x+0.2f, position.y + 3.2f + ojyouZ, position.z), Vector3(ojyouXL, -ojyouY, ojyouZL), Vector3(1.5f, 1.5f, 1.5f));
	}


	DirectXManager::GetInstance()->SetData2D();
	if (sceneCamOk&&HitFlag)
	{
		playerSprite->Draw("Blood", Vector3(500, 0 - damageFadeYpos, 0), 0.0f, Vector2(1, 1), Vector4(1, 1, 1, damageFade));
		playerSprite->Draw("Blood2", Vector3(0, 500 - damageFadeYpos, 0), 0.0f, Vector2(1, 1), Vector4(1, 1, 1, damageFade));
		playerSprite->Draw("Blood3", Vector3(900, 200 - damageFadeYpos, 0), 0.0f, Vector2(1, 1), Vector4(1, 1, 1, damageFade));
	}
	playerSprite->Draw("HpUi", Vector3(0, 0, 0), 0.0f, Vector2(1, 1), Vector4(1, 1, 1, 1));
	playerSprite->Draw("HpGage", Vector3(64, 0, 0), 0.0f, Vector2(1, 1), Vector4(1, 1, 1, 1));
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
			mBom->setPos(position);//後で調整
			mBom->Play();
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
	}

	if (!getTreasure&&col->GetColObject()->GetType() == ObjectType::TREASURE)
	{
		SetTresureGet(true);//宝ゲット
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
