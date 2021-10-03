#include "EnemyTank.h"
#include"../Collision/BaseCollider.h"
#include"../Weapons/NormalBullet.h"
#define ToRad(deg)((deg)*(PI/180.0f))
EnemyTank::EnemyTank(Vector3 pos, Vector3 ang, ObjectManager * obj, shared_ptr<ModelRenderer> m, shared_ptr<ParticleManager> p, shared_ptr<TexRenderer>s, int n)
	:tankModel(m),tankParticle(p),tankSprite(s)
{
	position = pos;
	objM = obj;
	number = n;
}

EnemyTank::~EnemyTank()
{
}

void EnemyTank::Shot()
{
	objM->Add(new NormalBullet(Vector3(position.x, position.y - 0.15f, position.z), Vector3(fireAngle, -atkAngle, 0), objM, tankModel, tankParticle, objType, bulletStock));
	ShotCount = 0;
	ShotFlag = false;
}

void EnemyTank::Move()
{
}

void EnemyTank::Init()
{
	tankAtk = "ATK";
	num = to_string(number);
	numNameAtk = tankAtk + num;

	tankModel->AddModel(numNameAtk, "Resouse/BoxTankATKAR.obj", "Resouse/BoxTankATKAR.png");
	tankModel->SetAncPoint(numNameAtk, Vector3(-2.0f, -2.0f, -2.0f));
	tankHou = "HOU";
	num = to_string(number);
	numNameHou = tankHou + num;

	tankModel->AddModel(numNameHou, "Resouse/BoxTankATKBR.obj", "Resouse/BoxTankATKBR.png");
	tankModel->SetAncPoint(numNameHou, Vector3(-2.0f, -2.0f, -2.0f));

	tankBtm = "BTM";
	num = to_string(number);
	numNameBtm = tankBtm + num;
	tankModel->AddModel(numNameBtm, "Resouse/BoxTankBTMR.obj", "Resouse/BoxTankBTMR.png");
	tankModel->SetAncPoint(numNameBtm, Vector3(-2.0f, -2.0f, -2.0f));

	tankParticleBox = make_shared<ParticleEmitterBox>(tankParticle);
	tankParticleBox->LoadAndSet("KemuriL", "Resouse/tuti.jpg");
	tankParticleBox->LoadAndSet("KemuriR", "Resouse/tuti.jpg");
	HP = 10;
	death = false;
	ShotFlag = false;
	HitFlag = false;
	ShotCount = 0;
	HitCount = 0;
	objType = ObjectType::ENEMY;
	

	angle = Vector3(0.0f, 180.0f, 0.0f);//車体
	atkAngle = 180.0f;//砲塔
	fireAngle = 0.0f;
	bulletStock = number * 10;
	speed = 0.2f;
	batteleS = BatteleStatus::SAFE;
	SetCollidder(Vector3(position.x, position.y, position.z),1.0f);
}

void EnemyTank::Update()
{
	ImGuiDebug();
	if (HitFlag)
	{
		HitCount++;
		if (HitCount >= 30)
		{
			HitCount = 0;
			HitFlag = false;
		}
	}
	if (HP <= 0)
	{
		death = true;
	}
	
	switch (batteleS)
	{
	case BatteleStatus::SAFE:
		Safe();
		break;
	case BatteleStatus::WARNING:
		Warning();
		break;
	case BatteleStatus::DANGER:
		Danger();
		break;
	case BatteleStatus::BATTELE:
		break;
	default:
		break;
	}
}

void EnemyTank::Rend()
{
	DirectXManager::GetInstance()->SetData3D();//モデル用をセット
	tankModel->Draw(numNameAtk, Vector3(position.x, position.y, position.z), Vector3(0, -angle.y, 0), Vector3(1.5f, 1.5f, 1.5f));
	tankModel->Draw(numNameHou, Vector3(position.x, position.y, position.z), Vector3(fireAngle, -angle.y, 0), Vector3(1.5f, 1.5f, 1.5f));
	tankModel->Draw(numNameBtm, Vector3(position.x, position.y, position.z), Vector3(0, -angle.y, 0), Vector3(1.5f, 1.5f, 1.5f));

}

void EnemyTank::Senser()
{
	SenserFan fan =
	{
		Vector3(position.x,position.y,position.z),
		160.0f,//視野角
		30.0f,//視認距離
		-angle.y - 90.0f
	};

	for (auto& o : objM->getUseList())
	{

		if (o->GetType() != ObjectType::PLAYER)continue;

		point =
		{
			Vector3(o->GetPosition().x,o->GetPosition().y,o->GetPosition().z),
			32.0f
		};

		bool isSenserIn = SenserIn(fan, point.Point);
		{
			if (isSenserIn)
			{
				switch (batteleS)
				{
				case BatteleStatus::SAFE:
					batteleS = BatteleStatus::WARNING;
					break;
				case BatteleStatus::WARNING:
					Attack();
					break;
				case BatteleStatus::DANGER:
					Escape();
					break;
				default:
					break;
				}
				break;
			}
			else
			{
				batteleS = BatteleStatus::SAFE;//見失ったら警戒を解く
			}
		}
	}
}

void EnemyTank::Attack()
{
	angleVec = Vector3(0, 0, 0);
	angleVec = GetEnemyVec(angleVec);
	angle.y = atan2(-angleVec.x, -angleVec.z)*180.0f / PI;
	velocity = RotateY(angle.y + 90.0f)*speed;
	position += velocity;
	FrontMove = true;
	BackMove = false;
}

void EnemyTank::Escape()
{
	angleVec = Vector3(0, 0, 0);
	angleVec = GetEnemyVec(angleVec);
	angle.y = atan2(-angleVec.x, -angleVec.z)*180.0f / PI;
	velocity = RotateY(angle.y + 90.0f)*speed;
	position -= velocity;
	BackMove = true;
	FrontMove = false;
}

void EnemyTank::Safe()
{
	//センサー更新
	Senser();
	velocity = RotateY(angle.y + 90.0f)*speed;
	position += velocity;
}

void EnemyTank::Warning()
{
	Senser();
}

void EnemyTank::Danger()
{
	Senser();
}

void EnemyTank::StatusChack()
{
}

bool EnemyTank::SenserIn(SenserFan fan, Vector3 point)
{
	//点と扇のベクトル
	Vector3 VecFtP =
	{
		point.x - fan.FanPoint.x,
		point.y - fan.FanPoint.y,
		point.z - fan.FanPoint.z
	};
	//ベクトルの長さの算出
	float VecLen = sqrtf((VecFtP.x*VecFtP.x) + (VecFtP.y*VecFtP.y) + (VecFtP.z*VecFtP.z));
	if (fan.FanLength < VecLen)
	{
		return false;//当たっていない
	}
	float rotateRad = ToRad(fan.FanAngle);
	Vector3 arcDir =
	{
		1.0f,
		0.0f,
		0.0f,
	};
	Vector3 rotateArcDir
	{
		arcDir.x*cosf(rotateRad) + arcDir.z*sinf(rotateRad),
		0.0f,
		arcDir.x*1.0f*sinf(rotateRad) + arcDir.z*cosf(rotateRad)
	};
	Vector3 normalFtP =
	{
		VecFtP.x / VecLen,
		VecFtP.y / VecLen,
		VecFtP.z / VecLen
	};

	float dot = normalFtP.x*rotateArcDir.x + normalFtP.y*rotateArcDir.y + normalFtP.z*rotateArcDir.z;
	float fanCos = cosf(ToRad(fan.FanArea / 2.0f));
	if (fanCos > dot)
	{
		return false;
	}
	return true;
}

Vector3 EnemyTank::GetEnemyVec(Vector3 vec)
{
	Vector3 EnemyPos = point.Point - position;
	return EnemyPos.normal();
}

Vector3 EnemyTank::GetFriendPos(int id)
{
	for (auto ai : objM->getUseList())
	{
		if (ai->GetType() == ObjectType::ENEMY)
		{
			if (ai->GetID() != id)
				return ai->GetPosition();
		}
	}
	return Vector3(0, 0, 0);
}

void EnemyTank::OnCollison(BaseCollider* col)
{
	
	if (col->GetColObject()->GetType() == ObjectType::BULLET)
	{
		HP -= col->GetColObject()->GetDamage();
		HitFlag = true;
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
	if (col->GetColObject()->GetType() == ObjectType::PLAYER)
	{
		batteleS = BatteleStatus::DANGER;
	}
}

void EnemyTank::ImGuiDebug()
{
	ImGui::SliderInt("EnemyHp", &HP, 0,HP);
}
