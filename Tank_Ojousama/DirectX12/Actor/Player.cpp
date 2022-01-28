#include "Player.h"

#include<sstream>
#include "../Device/Input.h"
#include "../Collision/BaseCollider.h"
#include "../Sound/Listener.h"
#include"../Sound/Sound.h"
#include "../Actor/CameraEye.h"
#include"../Utility/Timer/Timer.h"
//���킽��
#include"../Weapons/NormalBullet.h"
#include"../Weapons/LandMine.h"
#include"../Weapons/ShotGunBullet.h"
#include"../Weapons/MashinGun.h"
#include"../Scene/BaseScene.h"
#include"../Utility/Sequence/Sequence.h"
#include"../Utility/ModelChanger.h"
#include "../Utility/Random.h"
#include"../Items/ItemHolder.h"

#include "../ParticleSystem/ParticleType/Hit.h"
#include "../ParticleSystem/ParticleType/MachineGunAttackParticle.h"
#include "../ParticleSystem/ParticleType/NormalAttackParticle.h"
#include "../ParticleSystem/ParticleType/TankTrajectory.h"

#define ToRad(deg)((deg)*(PI/180.0f))
Player::Player(Vector3 pos, Vector3 ang, ObjectManager * obj, shared_ptr<ModelRenderer> m, shared_ptr<ParticleManager>p, shared_ptr<TexRenderer>s, int sceneE)
	:playerModel(m), playerParticle(p), playerSprite(s), mNormalAttackSE(nullptr),
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
		//�r�Ő�
		position = Vector3(0, 0, -130);
		sceneCamPos = Vector3(120, 80, -130);
		camera->SetEye(Vector3(sceneCamPos.x, sceneCamPos.y, position.z));
		camera->SetTarget(Vector3(0, 0, position.z));
		break;
	case 2:
		//���D
		position = Vector3(0, 0, 100);
		sceneCamPos = Vector3(120, 80, 100);
		camera->SetEye(Vector3(sceneCamPos.x, sceneCamPos.y, position.z));
		camera->SetTarget(Vector3(0, 0, position.z));
		break;
	case 3:
		//�^���[�f�B�t�F���X
		position = Vector3(0, 0, 500.0f);
		sceneCamPlayerOk = true;
		sceneCamPos = Vector3(-70.0f, 100.0f, 450.0f);
		camera->SetEye(Vector3(sceneCamPos.x, sceneCamPos.y, position.z));
		camera->SetTarget(Vector3(0, 0, position.z));
		break;
	case 4:
		//�{�X
		//�r�Ő�
		position = Vector3(0, 0, 500);
		sceneCamPos = Vector3(0, 15, 410);
		camera->SetEye(Vector3(sceneCamPos.x, sceneCamPos.y, sceneCamPos.z));
		camera->SetTarget(Vector3(0, sceneCamPos.y, 400));
		break;
	default:
		break;
	}

}

void Player::SceneCamMove1()
{
	GPS = true;
	if (position.z >= 520)
	{
		sceneCamPlayerOk = true;
		if (sceneCamPos.x <= 0)
		{
			GPS = false;
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
		position.z += 3.0f;
		camera->SetEye(Vector3(sceneCamPos.x, sceneCamPos.y, position.z));
		camera->SetTarget(Vector3(0, 0, position.z));
	}
}

void Player::SceneCamMove2()
{
	RSS = true;
	if (position.z >= 510)
	{
		sceneCamPlayerOk = true;
		if (sceneCamPos.x <= 0)
		{
			RSS = false;
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

void Player::SceneCamMove3()
{
	DSS = true;
	camera->SetEye(Vector3(sceneCamPos.x, sceneCamPos.y, sceneCamPos.z - 50.0f));
	camera->SetTarget(Vector3(position.x, position.y, position.z + 50.0f));
	if (sceneCamPos.y <= 4.0f)
	{

		sceneCamPos.x = position.x -= sceneCamPos.x * 0.03f;
		if (sceneCamPos.x == position.x)
		{
			sceneCamPos.z += 0.5f;
			if (sceneCamPos.z >= 550.0f)
			{
				DSS = false;
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
	BSS = true;
	if (sceneCamPos.z >= 500)
	{
		sceneCamPlayerOk = true;
		if (sceneCamPos.y <= 0)
		{
			BSS = false;
			sceneCamOk = true;
		}
		else
		{
			sceneCamPos.y -= 1.0f;
			camera->SetEye(Vector3(sceneCamPos.x, sceneCamPos.y, sceneCamPos.z));
			camera->SetTarget(Vector3(0, sceneCamPos.y, 400.0f));
		}
	}
	else
	{
		sceneCamPos.z += 0.7f;
		camera->SetEye(Vector3(sceneCamPos.x, sceneCamPos.y, sceneCamPos.z));
		camera->SetTarget(Vector3(0, sceneCamPos.y, 400.0f));
	}
}

void Player::SceneMoveBlock1()
{
	//�E
	if (position.x <= -80.0f)
	{
		position.x = -80.0f;
	}
	//��
	if (position.x >= 80.0f)
	{
		position.x = 80.0f;
	}
	//���
	if (position.z >= 510)
	{
		position.z = 510;
	}
	//�O
	if (position.z <= -250)
	{
		position.z = -250;
	}
}

void Player::SceneMoveBlock2()
{
	//�E
	if (position.x <= -60.0f)
	{
		position.x = -60.0f;
	}
	//��
	if (position.x >= 60.0f)
	{
		position.x = 60.0f;
	}
	//���
	if (position.z >= 510)
	{
		position.z = 510.0f;
	}
	//�O
	if (position.z <= 30)
	{
		position.z = 30.0f;
	}
}

void Player::SceneMoveBlock3()
{
	//�E
	if (position.x <= -150.0f)
	{
		position.x = -150.0f;
	}
	//��
	if (position.x >= 150.0f)
	{
		position.x = 150.0f;
	}
	//���
	if (position.z >= 510)
	{
		position.z = 510;
	}
	//�O
	if (position.z <= -250)
	{
		position.z = -250;
	}
}

void Player::SceneMoveBlock4()
{
	//�E
	if (position.x <= -150.0f)
	{
		position.x = -150.0f;
	}
	//��
	if (position.x >= 150.0f)
	{
		position.x = 150.0f;
	}
	//���
	if (position.z >= 510)
	{
		position.z = 510;
	}
	//�O
	if (position.z <= -250)
	{
		position.z = -250;
	}
}


void Player::UseWeapon1()
{
	moneyMax += 10000;
	objM->Add(new NormalBullet(Vector3(position.x, position.y + 1.5f, position.z), Vector3(fireAngle, -atkAngle, 0), objM, playerModel, playerParticle, objType, bulletStock, UpDamage));
	shotFlag1 = true;

	auto axis = CamVelocity;
	axis.normalize();
	mNormalAtkParticle->setPos(Vector3(position.x - (axis.x * 2.5f), position.y + 1.5f, position.z - (axis.z * 2.5f)));//��Œ���
	mNormalAtkParticle->Play();
}

void Player::UseWeapon2()
{

	if (modelChanger->GetWeaponState1() == WeaponsState::MachinGun)
	{
		mMashingunSE->setVol(BaseScene::mMasterSoundVol*BaseScene::mSESoundVol);
		mMashingunSE->play();
		moneyMax += 1000;
		masingunShot = true;
		objM->Add(new MashinGun(Vector3(position.x, position.y + 1.5f, position.z), Vector3(fireAngle, -atkAngle, 0), objM, playerModel, playerParticle, objType, bulletStock, UpDamage));
		
		auto axis = CamVelocity;
		axis.normalize();
		mMGAParticle->setPos(Vector3(position.x - (axis.x * 2.5f), position.y + 1.5f, position.z - (axis.z * 2.5f)));//��Œ���
		mMGAParticle->Play();
	}
	if (modelChanger->GetWeaponState1() == WeaponsState::Mine)
	{
		moneyMax += 20000;
		objM->Add(new LandMine(Vector3(position.x, position.y, position.z), Vector3(0, 0, 0), objM, playerModel, playerParticle, objType, bulletStock, UpDamage));
	}
	if (modelChanger->GetWeaponState1() == WeaponsState::ShotGun)
	{
		moneyMax += 15000;
		objM->Add(new ShotGunBullet(Vector3(position.x, position.y - 0.15f, position.z), Vector3(fireAngle, -atkAngle + 20.0f, 0), objM, playerModel, playerParticle, objType, bulletStock, UpDamage));
		objM->Add(new ShotGunBullet(Vector3(position.x, position.y - 0.15f, position.z), Vector3(fireAngle, -atkAngle + 10.0f, 0), objM, playerModel, playerParticle, objType, bulletStock + 1, UpDamage));
		objM->Add(new ShotGunBullet(Vector3(position.x, position.y - 0.15f, position.z), Vector3(fireAngle, -atkAngle, 0), objM, playerModel, playerParticle, objType, bulletStock + 2, UpDamage));
		objM->Add(new ShotGunBullet(Vector3(position.x, position.y - 0.15f, position.z), Vector3(fireAngle, -atkAngle - 10.0f, 0), objM, playerModel, playerParticle, objType, bulletStock + 3, UpDamage));
		objM->Add(new ShotGunBullet(Vector3(position.x, position.y - 0.15f, position.z), Vector3(fireAngle, -atkAngle - 20.0f, 0), objM, playerModel, playerParticle, objType, bulletStock + 4, UpDamage));
		
		auto axis = CamVelocity;
		axis.normalize();
		mNormalAtkParticle->setPos(Vector3(position.x - (axis.x * 2.5f), position.y + 1.5f, position.z - (axis.z * 2.5f)));//��ŕς��邩���H
		mNormalAtkParticle->Play();
	}
	shotFlag2 = true;
}

void Player::UseULT()
{
}

void Player::Item()
{
	if (ItemHolder::GetInstance()->GetItemNum(ItemNames::heal) > 0 && ItemNum == 0)
	{
		ItemHolder::GetInstance()->UseItem(ItemNames::heal);
	}
	if (ItemHolder::GetInstance()->GetItemNum(ItemNames::dome) > 0 && ItemNum == 1)
	{
		ItemHolder::GetInstance()->UseItem(ItemNames::dome);
	}
	if (ItemHolder::GetInstance()->GetItemNum(ItemNames::smock) > 0 && ItemNum == 2)
	{
		ItemHolder::GetInstance()->UseItem(ItemNames::smock);
	}
}

void Player::AngleReset()
{
	//���
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
	//����
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

	mNormalAttackSE = std::make_shared<Sound>("SE/bomb3.mp3", false);
	mDamageSE01 = std::make_shared<Sound>("SE/Player_Damage01.mp3", false);
	mDamageSE02 = std::make_shared<Sound>("SE/Player_Damage02.mp3", false);
	mDeathSE = std::make_shared<Sound>("SE/Player_Death.mp3", false);
	mMoveSE = std::make_shared<Sound>("SE/lab.wav", false);
	mMashingunSE = std::make_shared<Sound>("SE/MachinGun02.wav", false);
	mTimer = std::make_shared<Timer>();

	//�p�[�e�B�N��������
	mNormalAtkParticle = std::make_shared<NormalAttackParticle>(Vector3::zero, true);
	mMGAParticle = std::make_shared<MachineGunAttackParticle>(Vector3::zero, true);
	mTankTraL = std::make_shared<TankTrajectory>(Vector3::zero, true);
	mTankTraR = std::make_shared<TankTrajectory>(Vector3::zero, true);
	mHit = std::make_shared<Hit>(Vector3::zero, true);

	mNormalAtkParticle->Stop();
	mMGAParticle->Stop();
	mTankTraL->Stop();
	mTankTraR->Stop();
	mHit->Stop();

	playerSprite->AddTexture("HpUi", "Resouse/hpUI.png");
	playerSprite->AddTexture("HpGage", "Resouse/hpgage.png");
	playerSprite->AddTexture("HpGage2", "Resouse/hpgage2.png");
	playerSprite->AddTexture("WeponUi", "Resouse/wepon.png");
	playerSprite->AddTexture("Blood", "Resouse/blood.png");
	playerSprite->AddTexture("Blood2", "Resouse/blood2.png");
	playerSprite->AddTexture("Blood3", "Resouse/blood3.png");
	playerSprite->AddTexture("Film", "Resouse/Film.png");
	playerSprite->AddTexture("GamaplaySetumei", "Resouse/gameplaysetumei.png");
	playerSprite->AddTexture("RobberySetumei", "Resouse/robberysetumei.png");
	playerSprite->AddTexture("DefenceSetumei", "Resouse/difencesetumei.png");
	playerSprite->AddTexture("BossSceneSetumei", "Resouse/bossscenesetumei.png");
	playerSprite->AddTexture("repairIcon", "Resouse/RepairIcon.png");
	playerSprite->AddTexture("smokeIcon", "Resouse/SmokeIcon.png");
	playerSprite->AddTexture("shieldIcon", "Resouse/ShieldIcon.png");
	playerSprite->AddTexture("X", "Resouse/X.png");

	playerSprite->AddTexture("Danyaku2", "Resouse/dannyakuhi.png");

	//�R���p�X
	/*playerSprite->AddTexture("Compas", "Resouse/compas.png");
	playerSprite->SetSize("Compas", Vector2(228, 190));
	playerSprite->SetAncPoint("Compas", Vector2(-104, -104));
	playerSprite->AddTexture("Hari", "Resouse/hari.png");
	playerSprite->SetSize("Hari", Vector2(188, 188));
	playerSprite->SetAncPoint("Hari", Vector2(-95,-120));*/

	//model
	modelChanger = new ModelChanger();
	modelChanger->Load(playerModel);
	//HP
	maxHP = modelChanger->GetHP();
	HP = maxHP;
	maxSpeed = modelChanger->GetSpeed();
	UpDamage = modelChanger->GetUpDamage();
	playerSprite->AddTexture("AIM", "Resouse/AIM64.png");
	playerSprite->AddTexture("AIM_S", "Resouse/croshear.png");
	death = false;
	SetTresureGet(false);//�󖢓���
	objType = ObjectType::PLAYER;
	CamPos_Y = 2.5f;
	TargetPos.y = 2.5f;
	angle = Vector3(0.0f, 0.0f, 0.0f);//�ԑ�
	atkAngle = 0.0f;//�C��
	aimPos_Y = 360.0f;
	fireAngle = 0.0f;
	speed = 0.0f;

	speedTime = 0.0f;
	speedLimitTime = 10.0f;
	cameraSpeed = 2.0f;
	bulletStock = 0;
	HitFlag = false;
	sniperShotFlag = false;
	masingunShot = false;
	HitCount = 0;
	ItemNum = 0;
	sceneCamFinish = false;
	sceneCamOk = false;
	sceneCamPlayerOk = false;
	damageFade = false;
	damageFadeYpos = 0;
	CameraPos = Vector3(position.x, position.y, position.z + 15.0f);

	countMoney = 0;
	money = 0;
	moneyMax = 0;
	BaseScene::mMinusMoney = 0;

	//�R���C�_�[�̏����Z�b�g
	SetCollidder(Vector3(0, 0, 0), 1.0f);
	ojyouY = 0.0f;
	ojyouZ = 0.0f;
	ojyouXR = 0.0f;
	ojyouXL = 0.0f;
	ojyouZR = 0.0f;
	ojyouZL = 0.0f;

	item = new ItemHolder();
	StartCamScene();//���o����
}

void Player::Update()
{
	if (HP <= 0)
	{
		ojyouZ += 1.0f;
	}
	mTimer->update();

	mNormalAttackSE->setVol(BaseScene::mMasterSoundVol*BaseScene::mSESoundVol);
	if (moneyMax > money)
	{
		money += 1000;
		BaseScene::mMinusMoney = money;
	}

	
	//�V�[�����o 
#pragma region �V�[��
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
		FrontMove = false;
		BackMove = false;
		if (HP <= 0)
		{
			BaseScene::mMoney -= 500000;
			GameOver = true;
			mDeathSE->setVol(BaseScene::mMasterSoundVol*BaseScene::mSESoundVol);
			mDeathSE->play();
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

		ImGuiDebug();//�f�o�b�O�p
		AngleReset();
		//�L�[��������
		if (Input::getKey(KeyCode::W) || Input::joyVertical(500) > 0)
		{
			TrajectoryPlay();
			speedTime++;
			if (speedTime >= speedLimitTime)
			{
				speedTime = speedLimitTime;
			}
			position += velocity;
			FrontMove = true;
			BackMove = false;
			moveFlag = true;
		}
		else if (Input::getKey(KeyCode::S) || Input::joyVertical(500) < 0)
		{
			TrajectoryPlay();
			speedTime++;
			if (speedTime >= speedLimitTime)
			{
				speedTime = speedLimitTime;
			}
			position -= velocity;
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
		if (Input::getKey(KeyCode::D) || Input::joyHorizontal() > 0)
		{
			angle.y += 2.0f;
		}
		if (Input::getKey(KeyCode::A) || Input::joyHorizontal() < 0)
		{
			angle.y -= 2.0f;
		}

		//�C��
		if (Input::getKey(KeyCode::UPARROW) || Input::joyRightVertical(500) > 0)
		{
			aimPos_Y -= 4.5f;
			CamPos_Y -= 0.04f;
			TargetPos.y += 0.02f;
			fireAngle -= 1.0f;
		}
		if (fireAngle <= -25.0f)
		{
			fireAngle = -25.0f;//���
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
		if (Input::getKey(KeyCode::DOWNARROW) || Input::joyRightVertical(500) < 0)
		{
			aimPos_Y += 4.5f;
			CamPos_Y += 0.04f;
			TargetPos.y -= 0.02f;
			fireAngle += 1.0f;
		}
		if (fireAngle >= 0.0f)
		{
			fireAngle = 0.0f;//����
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
		if (Input::getKey(KeyCode::RIGHTARROW) || Input::joyRightHorizontal() > 0)
		{
			atkAngle += cameraSpeed;
		}
		if (Input::getKey(KeyCode::LEFTARROW) || Input::joyRightHorizontal() < 0)
		{
			atkAngle -= cameraSpeed;
		}
		//�s������
		switch (sceneEffectNum)
		{
		case 1:
			SceneMoveBlock1();
			break;
		case 2:
			SceneMoveBlock2();
			break;
		case 3:
			SceneMoveBlock3();
			break;
			SceneMoveBlock4();
		case 4:
			break;
		default:
			break;
		}
		

		if (Input::getKeyDown(KeyCode::E) || Input::getJoyDown(JoyCode::X))
		{
			Item();
		}
		if (Input::getKeyDown(KeyCode::R) || Input::getJoyDown(JoyCode::RightStickButton))
		{
			ItemNum++;
		}
		if (ItemNum >= 3)
		{
			ItemNum = 0;
		}

		if (Input::getKeyDown(KeyCode::Q) || Input::getJoyDown(JoyCode::LeftButton))
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
			camera->SetTarget(Vector3(position.x, position.y + TargetPos.y, position.z));
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
			if (Input::getKeyDown(KeyCode::SPACE) || Input::getRightTriggerDown())//R�g���K�[
			{
				UseWeapon1();
				bulletStock++;
				shotcnt1 = 0;
				mNormalAttackSE->play();
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
			if (Input::getKey(KeyCode::LEFTSHIFT) || Input::getJoy(JoyCode::RightButton))//Y�{�^��
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

		//���������݂�
		if (bulletStock >= 70)
		{
			bulletStock = 0;
		}

	}

	//���X�i�[�Ɏ��g�̈ʒu���X�V
	listener->setPos(position);
}

void Player::Rend()
{
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
	DirectXManager::GetInstance()->SetData3D();//���f���p���Z�b�g
	if (!sniperShotFlag&&sceneCamPlayerOk)
	{
		playerModel->Draw(modelChanger->GetModelName(3), Vector3(position.x, position.y, position.z), Vector3(0, -atkAngle, 0), Vector3(1.5f, 1.5f, 1.5f));
		playerModel->Draw(modelChanger->GetModelName(4), Vector3(position.x, position.y, position.z), Vector3(0, -angle.y, 0), Vector3(1.5f, 1.5f, 1.5f));

		playerModel->Draw(modelChanger->GetModelName(0), Vector3(position.x - 0.2f, position.y + 3.2f + ojyouZ, position.z), Vector3(ojyouXR, -atkAngle, ojyouZR), Vector3(1.5f, 1.5f, 1.5f));
		playerModel->Draw(modelChanger->GetModelName(1), Vector3(position.x, position.y + ojyouZ, position.z), Vector3(0, -atkAngle, 0), Vector3(1.5f, 1.5f, 1.5f));
		playerModel->Draw(modelChanger->GetModelName(2), Vector3(position.x + 0.2f, position.y + 3.2f + ojyouZ, position.z), Vector3(ojyouXL, -atkAngle, ojyouZL), Vector3(1.5f, 1.5f, 1.5f));
	}


	DirectXManager::GetInstance()->SetData2D();
	if (!sceneCamFinish)
	{
		playerSprite->Draw("Film", Vector3(0, 0, 0), 0.0f, Vector2(1, 1), Vector4(1, 1, 1, 1));
	}
	if (GPS)
	{
		playerSprite->Draw("GamaplaySetumei", Vector3(0, 0, 0), 0.0f, Vector2(1, 1), Vector4(1, 1, 1, 1));
	}
	else
	{
		playerSprite->Draw("GamaplaySetumei", Vector3(0, 0, 0), 0.0f, Vector2(1, 1), Vector4(1, 1, 1, 0));
	}
	if (RSS)
	{
		playerSprite->Draw("RobberySetumei", Vector3(0, 0, 0), 0.0f, Vector2(1, 1), Vector4(1, 1, 1, 1));
	}
	else
	{
		playerSprite->Draw("RobberySetumei", Vector3(0, 0, 0), 0.0f, Vector2(1, 1), Vector4(1, 1, 1, 0));
	}
	if (DSS)
	{
		playerSprite->Draw("DefenceSetumei", Vector3(0, 0, 0), 0.0f, Vector2(1, 1), Vector4(1, 1, 1, 1));
	}	
	else 
	{
		playerSprite->Draw("DefenceSetumei", Vector3(0, 0, 0), 0.0f, Vector2(1, 1), Vector4(1, 1, 1, 0));
	}
	if (BSS)
	{
		playerSprite->Draw("BossSceneSetumei", Vector3(0, 0, 0), 0.0f, Vector2(1, 1), Vector4(1, 1, 1, 1));
	}
	else
	{
		playerSprite->Draw("BossSceneSetumei", Vector3(0, 0, 0), 0.0f, Vector2(1, 1), Vector4(1, 1, 1, 0));
	}

	if (sceneCamOk&&HitFlag)
	{
		playerSprite->Draw("Blood", Vector3(500, 0 - damageFadeYpos, 0), 0.0f, Vector2(1, 1), Vector4(1, 1, 1, damageFade));
		playerSprite->Draw("Blood2", Vector3(0, 500 - damageFadeYpos, 0), 0.0f, Vector2(1, 1), Vector4(1, 1, 1, damageFade));
		playerSprite->Draw("Blood3", Vector3(900, 200 - damageFadeYpos, 0), 0.0f, Vector2(1, 1), Vector4(1, 1, 1, damageFade));
	}

	if (sceneCamOk)
	{
		//playerSprite->Draw("Compas", Vector3(1000, 600, 0), 0.0f, Vector2(1, 1), Vector4(1, 1, 1, 1));
		//playerSprite->Draw("Hari", Vector3(1012, 610, 0), angle.y, Vector2(1, 1), Vector4(1, 1, 1, 1));
		
		
		

		if (!sniperShotFlag)
		{
			playerSprite->Draw("AIM", Vector3((Window::Window_Width / 2) - 32, aimPos_Y, 0), 0.0f, Vector2(1, 1), Vector4(1, 1, 1, 1));
		}
		else
		{
			playerSprite->Draw("AIM_S", Vector3(0, 0, 0), 0.0f, Vector2(1, 1), Vector4(1, 1, 1, 1));
		}
		playerSprite->Draw("HpUi", Vector3(0, 0, 0), 0.0f, Vector2(1, 1), Vector4(1, 1, 1, 1));
		if (HP >= 100)
		{
			playerSprite->Draw("HpGage", Vector3(64, 0, 0), 0.0f, Vector2(1, 1), Vector4(1, 1, 1, 1));
		}
		else
		{
			playerSprite->Draw("HpGage2", Vector3(64, 0, 0), 0.0f, Vector2(1, 1), Vector4(1, 1, 1, 1));
		}

		if (ItemNum == 0)
		{
			playerSprite->Draw("repairIcon", Vector3(1280 - 65, 650, 0), 0.0f, Vector2(1, 1), Vector4(1, 1, 1, 1));
		}
		if (ItemNum == 1)
		{
			playerSprite->Draw("shieldIcon", Vector3(1280 - 65, 650, 0), 0.0f, Vector2(1, 1), Vector4(1, 1, 1, 1));
		}
		if (ItemNum == 2)
		{
			playerSprite->Draw("smokeIcon", Vector3(1280 - 65, 650, 0), 0.0f, Vector2(1, 1), Vector4(1, 1, 1, 1));
		}
		playerSprite->Draw("X", Vector3(1280 - 65, 650, 0), 0.0f, Vector2(1, 1), Vector4(1, 1, 1, 1));
		Sequence::instance().set(HP, Vector2(64, 0), Vector2(64, 64));
		Sequence::instance().set(BaseScene::mMinusMoney, Vector2(1280 - 320, 0), Vector2(32, 32));
		Sequence::instance().drawNumber(DirectXManager::GetInstance()->CmdList());
		DirectXManager::GetInstance()->SetDrawComnd();
		DirectXManager::GetInstance()->SetData2D();
		playerSprite->Draw("WeponUi", Vector3(1280 - 90, 720 - 270, 0), 0.0f, Vector2(1, 1), Vector4(1, 1, 1, 1));
		playerSprite->SetSize("Danyaku2", Vector2(320, 32));
		playerSprite->Draw("Danyaku2", Vector3(1280 - 512 + 94, 0, 0), 0.0f, Vector2(1, 1), Vector4(1, 1, 1, 1));
	}
	
	

	

}

void Player::SetHP(int value)
{
	HP = value;
}

void Player::TrajectoryPlay()
{
	Vector3 axis = angle;
	axis.normalize();
	mTankTraL->setPos(Vector3(position.x - 1.f, position.y, position.z + axis.y));//��Œ���
	mTankTraR->setPos(Vector3(position.x + 1.f, position.y, position.z - axis.y));//��Œ���
	mTankTraL->Play();
	mTankTraR->Play();

	mMoveSE->setVol(BaseScene::mMasterSoundVol * BaseScene::mSESoundVol);
	mMoveSE->play();
}


void Player::OnCollison(BaseCollider* col)
{
	if (!HitFlag&&sceneCamFinish)
	{
		if (col->GetColObject()->GetType() == ObjectType::ENEMYBULLET
			|| col->GetColObject()->GetType() == ObjectType::ENEMY
			|| col->GetColObject()->GetType() == ObjectType::BOSS)
		{
			if (ItemHolder::GetInstance()->GetUseShield())
			{
				ItemHolder::GetInstance()->SetDamege(col->GetColObject()->GetDamage());
			}
			else
			{
				HP -= col->GetColObject()->GetDamage();
				HitFlag = true;
				mHit->setPos(position);//��Œ���
				mHit->Play();

				Random::initialize();
				int count = Random::randomRange(0, 2);
				if (count == 0)
				{
					mDamageSE01->setVol(BaseScene::mMasterSoundVol*BaseScene::mSESoundVol);
					mDamageSE01->play();
				}
				else if (count == 1)
				{
					mDamageSE02->setVol(BaseScene::mMasterSoundVol*BaseScene::mSESoundVol);
					mDamageSE02->play();
				}
			}
		}
	}

	if (col->GetColObject()->GetType() == ObjectType::BLOCK)
	{
		//velocity = velocity * -1;
	
		if (FrontMove)//������C��
		{
			position -= velocity;
			//position -= Vector3(velocity.x,velocity.y,velocity.z - 0.02f);//�O���ړ��̂�
		}
		else if (BackMove)
		{
			position += velocity;
			//position += Vector3(velocity.x, velocity.y, velocity.z + 0.02f);//����ړ��̂�
		}
	}

	if (!getTreasure&&col->GetColObject()->GetType() == ObjectType::TREASURE)
	{
		SetTresureGet(true);//��Q�b�g
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
	ImGui::Checkbox("Flont", &FrontMove);
	ImGui::Checkbox("Back", &BackMove);
	ImGui::SliderInt("HP", &HP, 0, HP);
	ImGui::SliderInt("mainWeapon", &shotcnt1, 0, objM->GetReloadTime());
	ImGui::SliderInt("subWeapon", &shotcnt2, 0, objM->GetReloadTime());
	ImGui::SliderFloat("SPEED", &speed, 0, 100);
	ImGui::SliderInt("SPEEDTime", &speedTime, 0, 100);

#endif  _DEBUG
}
