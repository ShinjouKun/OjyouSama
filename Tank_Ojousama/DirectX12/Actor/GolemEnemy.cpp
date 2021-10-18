#include "GolemEnemy.h"
#include"../Collision/SpherCollider.h"
#include "../Collision/BaseCollider.h"
#include"../ConstInfomation/Enemy/GolemEnemyConstInfo.h"
namespace GECI = GolemEnemyConstInfo;
#define ToRad(deg)((deg)*(PI/180.0f))
GolemEnemy::GolemEnemy(Vector3 pos, Vector3 ang, ObjectManager * obj, shared_ptr<ModelRenderer> modelRender, shared_ptr<TexRenderer> texRender, shared_ptr<ParticleManager> effect, int num)
	:Model(modelRender),Sprite(texRender),Particle(effect)
{
	position = pos;
	angle = ang;
	objM = obj;
}

GolemEnemy::~GolemEnemy()
{
}

void GolemEnemy::Senser()
{
	SenserFan_G fan =
	{
		Vector3(position.x,position.y,position.z),
		180.0f,//視野角
		80.0f,//視認距離
		-bodyAngle - 90.0f
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
				case GolemBatteleStatus::SAFE_G:
					batteleS = GolemBatteleStatus::BATTELE_G;
					break;
				case GolemBatteleStatus::BATTELE_G:
					break;
				case GolemBatteleStatus::RETURN_G:
					//batteleS = GolemBatteleStatus::BATTELE_G;
					break;
				default:
					break;
				}
				break;
			}
			else
			{
				batteleS = GolemBatteleStatus::SAFE_G;//見失ったら警戒を解く
			}
		}
	}
}

bool GolemEnemy::SenserIn(SenserFan_G fan, Vector3 point)
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

bool GolemEnemy::CanpOut()
{
	Vector3 distance = this->position - canp.CanpPoint;
	float length = distance.Length();

	
	if (length >canp.CanpRadius)
	{
		return false;//拠点範囲内
	}

	return true;//拠点外
}

bool GolemEnemy::Distance(const Vector3 & otherPos, float dist)
{
	Vector3 distance = otherPos - position;
	float length = distance.Length();

	//範囲外だったらここで終わる
	if (length > dist)
	{
		return false;
	}

	return true;
}

void GolemEnemy::Safe()
{
}

void GolemEnemy::Battele()
{
	//攻撃カウント開始
	AttackCount++;
	if (AttackCount >= 240)
	{
		ProximityAttack();
		AttackFlag = true;//攻撃!!
		AttackCount = 0;
	}
	PredecessorMove();
	
}

void GolemEnemy::PredecessorMove()//接近する動き
{
	//angleVec = Vector3(0, 0, 0);
	//angleVec = GetEnemyVec(angleVec);
	//bodyAngle = atan2(-angleVec.x, -angleVec.z)*180.0f / PI;
	//velocity = RotateY(bodyAngle + 90.0f)*speed;
	//if (!Distance(point.Point, 20.0f))//一定以上接近しない
	//{
	//	position += velocity;
	//	ArmPosL += velocity;
	//	ArmPosR += velocity;
	//}

	//if (CanpOut())//拠点範囲外にでたら戻る
	//{
	//	batteleS = GolemBatteleStatus::RETURN_G;
	//}
}

void GolemEnemy::ReturnMove()
{
	angleVec = Vector3(0, 0, 0);
	angleVec = GetCampVec(angleVec);
	bodyAngle = atan2(-angleVec.x, -angleVec.z)*180.0f / PI;
	velocity = RotateY(bodyAngle + 90.0f)*speed;

	position -= velocity;
	ArmPosL -= velocity;
	ArmPosR -= velocity;
}

void GolemEnemy::AvoidMove()
{
	//移動点をランダムでとって動き回る
}

void GolemEnemy::ProximityAttack()
{
	angleVec = Vector3(0, 0, 0);
	angleVec = GetEnemyVec(angleVec);
	bodyAngle = atan2(-angleVec.x, -angleVec.z)*180.0f / PI;
	velocity = RotateY(bodyAngle + 90.0f)*speed;
	position += velocity;//胴体
	ArmPosL += velocity;
	ArmPosR += velocity;
	//殴る
}

void GolemEnemy::LangeAttack()
{
	//岩を投げる
	zR = 15.0f;
	zL = -15.0f;
	ArmAngleL += 30.0f;
	ArmAngleR += 30.0f;
}

void GolemEnemy::SpecialAttack()
{
	//腕を飛ばす
	ArmAngleL = 70.0f;
	ArmAngleR = -70.0f;
	bodyAngle += 10.0f;
	ArmPosL += velocity;
	ArmPosR += velocity;
}

Vector3 GolemEnemy::GetCampVec(const Vector3 & vec)
{
	Vector3 CampPos = canp.CanpPoint - position;
	return CampPos.normal();
}

Vector3 GolemEnemy::GetEnemyVec(const Vector3 & vec)
{
	Vector3 EnemyPos = point.Point - position;
	return EnemyPos.normal();
}

void GolemEnemy::Init()
{
	//モデル
	Body = "Body";
	num = to_string(number);
	numNameBody = Body + num;

	Model->AddModel(numNameBody, "Resouse/GolemBody.obj", "Resouse/GolemBody.png");
	Model->SetAncPoint(numNameBody, Vector3(-2.0f, -2.0f, -2.0f));
	ArmR = "ArmR";
	num = to_string(number);
	numNameArmR = ArmR + num;

	Model->AddModel(numNameArmR, "Resouse/GolemArm_R.obj", "Resouse/GolemArm_R.png");
	Model->SetAncPoint(numNameArmR, Vector3(-2.0f, -4.0f, -2.0f));

	ArmL = "ArmL";
	num = to_string(number);
	numNameArmL = ArmL + num;
	Model->AddModel(numNameArmL, "Resouse/GolemArm_L.obj", "Resouse/GolemArm_L.png");
	Model->SetAncPoint(numNameArmL, Vector3(-2.0f, -4.0f, -2.0f));
	//ステータス対応
	ArmPosL = Vector3(position.x - 0.1f, position.y + 5.0f, position.z);
	ArmPosR = Vector3(position.x + 0.1f, position.y + 5.0f, position.z);
	canp.CanpPoint = Vector3(100.0f, 4.0f, 120.0f);
	canp.CanpRadius = 150.0f;
	HP = 300;
	speed = 0.2f;
	death = false;
	AttackFlag = false;
	AttackCount = 0;
	HitFlag = false;
	HitCount = 0;
	objType = ObjectType::ENEMY;
	bodyAngle = 180.0f;
	ArmAngleR = 0.0f;
	zR = 0.0f;
	ArmAngleL = 0.0f;
	zL = 0.0f;
	batteleS = GolemBatteleStatus::SAFE_G;
	SetCollidder(Vector3(position.x, position.y, position.z), 2.0f);
}

void GolemEnemy::Update()
{
	SpecialAttack();
	ImGuiDebug();
	Senser();
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
	case GolemBatteleStatus::SAFE_G:
		Safe();
		break;
	case GolemBatteleStatus::BATTELE_G:
		Battele();
		break;
	case GolemBatteleStatus::RETURN_G:
		ReturnMove();
		break;
	default:
		break;
	}
}

void GolemEnemy::Rend()
{
	DirectXManager::GetInstance()->SetData3D();//モデル用をセット
	Model->Draw(numNameBody, Vector3(position.x, position.y, position.z), Vector3(0, -bodyAngle, 0), Vector3(3.0f,3.0f,3.0f));
	Model->Draw(numNameArmR, Vector3(ArmPosR.x,ArmPosR.y,ArmPosR.z), Vector3(ArmAngleR, -bodyAngle, zR), Vector3(4.0f, 4.0f, 4.0f));
	Model->Draw(numNameArmL, Vector3(ArmPosL.x, ArmPosL.y, ArmPosL.z), Vector3(ArmAngleL, -bodyAngle, zL), Vector3(4.0f, 4.0f, 4.0f));
}

void GolemEnemy::ImGuiDebug()
{
}

void GolemEnemy::OnCollison(BaseCollider * col)
{
	if (col->GetColObject()->GetType() == ObjectType::BULLET)
	{
		HP -= col->GetColObject()->GetDamage();
		HitFlag = true;
	}
}
