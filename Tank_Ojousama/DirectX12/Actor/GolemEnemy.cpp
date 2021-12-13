#include "GolemEnemy.h"
#include"../Collision/SpherCollider.h"
#include "../Collision/BaseCollider.h"
#include"../ConstInfomation/Enemy/GolemEnemyConstInfo.h"
#include<random>
//武器
#include"../Weapons/StoneWeapon.h"
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


void GolemEnemy::Init()
{
	SetActive(false);
	startFlag = false;
	mDeathAnimation = false;
	mDeadFlag = false;

	mRiseTime = std::make_shared<Timer>();
	mRiseTime->setTime(1.0f);
	mDeathTime = std::make_shared<Timer>();
	mDeathTime->setTime(1.0f);
	mDeathStep = DeathAnimationStep::RISE_SKY;
	maxSpeed = 2.0f;
	speedTime = 0.0f;
	speedLimitTime = 360.0f;
	bulletStock = number * 10;
	//モデル
	Body = "Body";
	num = to_string(number);
	numNameBody = Body + num;

	Model->AddModel(numNameBody, "Resouse/gorem_body.obj", "Resouse/gorem.png");
	Model->SetAncPoint(numNameBody, Vector3(0.0f, -2.0f, 0.0f));
	ArmR = "ArmR";
	num = to_string(number);
	numNameArmR = ArmR + num;

	Model->AddModel(numNameArmR, "Resouse/gorem_hands_R.obj", "Resouse/gorem.png");
	Model->SetAncPoint(numNameArmR, Vector3(0.0f, -3.0f, 0.0f));

	ArmL = "ArmL";
	num = to_string(number);
	numNameArmL = ArmL + num;
	Model->AddModel(numNameArmL, "Resouse/gorem_hands_L.obj", "Resouse/gorem.png");
	Model->SetAncPoint(numNameArmL, Vector3(0.0f, -3.0f, 0.0f));
	//ステータス対応
	ArmPosL = Vector3(position.x - 0.1f, position.y + 5.0f, position.z);
	ArmPosR = Vector3(position.x + 0.1f, position.y + 5.0f, position.z);
	canp.CanpPoint = Vector3(0.0f, 4.0f, -70.0f);
	canp.CanpRadius = 120.0f;
	HP = 100;
	speed = 0.2f;
	death = false;
	AttackFlag = false;
	AttackCount = 0;
	HitFlag = false;
	HitCount = 0;
	objType = ObjectType::BOSS;
	bodyAngle = Vector3(0, 180.0f, 0);
	ArmAngleR = 0.0f;
	zR = 0.0f;
	ArmAngleL = 0.0f;
	zL = 0.0f;
	rndCount = 0;
	avmoveCount = 0;
	attackMoveCount = 0;
	moveCount = 0;
	damage = 0;//近距離系
	targetAngleX = 0.0f;
	stoneShotFlag = false;//石投げ用
	guardFlag = false;
	batteleS = GolemBatteleStatus::SAFE_G;
	SetCollidder(Vector3(0, 0, 0), 2.0f);

}

void GolemEnemy::Senser()
{
	SenserFan_G fan =
	{
		Vector3(position.x,position.y,position.z),
		360.0f,//視野角
		90.0f,//視認距離
		-bodyAngle.y - 90.0f
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
					batteleS = GolemBatteleStatus::BATTELE_G;
					break;
				default:
					break;
				}
				break;
			}
			else if(!AttackFlag)
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
	if (!objM->GetPlayer().GetSceneFinish())
	{
		position.y = -6.0f;
		ArmPosL.y = -6.0f;
		ArmPosR.y = -6.0f;
	}
}

void GolemEnemy::Battele()
{
	//攻撃カウント開始
	if (position.y >= 4.0f)
	{
		position.y = 4.0f;
		ArmPosL.y = 8.0f;
		ArmPosR.y = 8.0f;
		/*zL = 0.0f;
		zR = 0.0f;*/
	}
	else
	{
		position.y += 1.0f;
		ArmPosL.y += 1.5f;
		ArmPosR.y += 1.5f;
		zL = 20.0f;
		zR = -20.0f;
	}
	
	moveCount++;
	AttackCount++;
	if (AttackCount >= 240)//四秒
	{
		AttackFlag = true;//攻撃!!
	}
	if (AttackFlag)
	{
		
		if (Distance(point.Point, 25.0f))//標的が近ければ
		{
			ProximityAttack();//近距離
		}
		else
		{
			LangeAttack();//遠距離
		}
		
		
	}
	else if(moveCount >= 90)
	{
		PredecessorMove();
	}
	
}

void GolemEnemy::PredecessorMove()//接近する動き
{
	if (moveCount <= 240)
	{
		angleVec = Vector3(0, 0, 0);
		angleVec = GetEnemyVec(angleVec);
		bodyAngle.y = atan2(-angleVec.x, -angleVec.z)*180.0f / PI;
		velocity = RotateY(bodyAngle.y + 90.0f)*speed;
		if (!Distance(point.Point, 25.0f))//一定以上接近しない
		{
			position += velocity;
			ArmPosL += velocity;
			ArmPosR += velocity;
		}
		else if (Distance(point.Point, 20.0f))//相手から接近してきた場合下がる
		{
			position -= velocity;
			ArmPosL -= velocity;
			ArmPosR -= velocity;
		}

		if (CanpOut())//拠点範囲外にでたら戻る
		{
			batteleS = GolemBatteleStatus::RETURN_G;
		}
	}
	else
	{
		moveCount = 0;
	}
	
}

void GolemEnemy::ReturnMove()
{
	angleVec = Vector3(0, 0, 0);
	angleVec = GetCampVec(angleVec);
	bodyAngle.y = atan2(-angleVec.x, -angleVec.z)*180.0f / PI;
	velocity = RotateY(bodyAngle.y + 90.0f)*speed;

	position -= velocity;
	ArmPosL -= velocity;
	ArmPosR -= velocity;
}

void GolemEnemy::AvoidMove()
{
	
	if (!avmoveFlag)
	{
		//移動点をランダムでとって動き回る
		SenserFan_G fan =
		{
			Vector3(position.x,position.y,position.z),
			180.0f,//視野角
			30.0f,//視認距離
			-bodyAngle.y - 90.0f
		};

		/*random_device rnd;
		default_random_engine eng(rnd());
		uniform_int_distribution<int>dist(-30.0f, 30.0f);*/


		random_device rndang;
		default_random_engine engang(rndang());
		uniform_int_distribution<int>distang(0,4);

		/*Vector3 moveP;
		moveP.z = dist(eng);*/
		int a = distang(engang);
		float ang;
		switch (a)
		{
		case 0:
			ang = 0.0f;
			break;
		case 1:
			ang = 30.0f;
			break;
		case 2:
			ang = -30.0f;
			break;
		case 3:
			ang = 90.0f;
			break;
		case 4:
			ang = -90.0f;
			break;
		default:
			break;
		}
		bodyAngle.y = ang;
		velocity = Vector3(0, 0, -1);
		velocity *= Matrix4::RotateY(-bodyAngle.y);
		avmoveFlag = true;
	}
	else
	{
		avmoveCount++;
		position += velocity*speed;
		ArmPosL += velocity*speed;
		ArmPosR += velocity*speed;
		if (avmoveCount >= 40)
		{
			rndCount = 0;
			avmoveCount = 0;
			avmoveFlag = false;
		}
	}
	
}

void GolemEnemy::ProximityAttack()
{
	damage = 20;
	attackMoveCount++;
	
	if (attackMoveCount <= 20)//追尾制限
	{
		speed = 0.25f;
		zR = 35.0f;
		ArmAngleR = 180.0f;
		zL = -35.0f;
		ArmAngleL = 180.0f;
		bodyAngle.x = -20.0f;
		angleVec = Vector3(0, 0, 0);
		angleVec = GetEnemyVec(angleVec);
		bodyAngle.y = atan2(-angleVec.x, -angleVec.z)*180.0f / PI;
		velocity = RotateY(bodyAngle.y + 90.0f)*speed;
	}
	
	if (attackMoveCount >= 60)//一定時間で攻撃終了
	{
		bodyAngle.x += 1.2f;
		ArmAngleR += 4.0f;
		ArmAngleL += 4.0f;
		if (ArmAngleR >= 360.0f)
		{
			speed = 0.2f;
			damage = 0;
			zR = 0.0f;
			zL = 0.0f;
			bodyAngle.x = 0;
			ArmAngleL = 0.0f;
			ArmAngleR = 0.0f;
			AttackCount = 0;
			attackMoveCount = 0;
			AttackFlag = false;
		}
		
	}

	if (!Distance(point.Point, 5.0f))//一定以上接近しない
	{
		position += velocity;
		ArmPosL += velocity;
		ArmPosR += velocity;
	}
	else
	{
		bodyAngle.x += 1.2f;
		ArmAngleR += 4.0f;
		ArmAngleL += 4.0f;
		if (ArmAngleR >= 360.0f)
		{
			damage = 0;
			bodyAngle.x = 0;
			zR = 0.0f;
			zL = 0.0f;
			ArmAngleL = 0.0f;
			ArmAngleR = 0.0f;
			AttackCount = 0;
			attackMoveCount = 0;
			AttackFlag = false;
		}
	}
	
}

void GolemEnemy::LangeAttack()
{
	
	if (!stoneShotFlag)
	{
		bulletStock++;
		objM->Add(new StoneWeapon(Vector3(position.x, position.y+1.0f, position.z),
			Vector3(0, -bodyAngle.y, 0),
			objM, Model, Particle, objType, bulletStock));
		stoneShotFlag = true;
	}
	attackMoveCount++;
	//岩を投げる
	angleVec = Vector3(0, 0, 0);
	angleVec = GetEnemyVec(angleVec);
	//targetAngleX = 10;
	bodyAngle.y = atan2(-angleVec.x, -angleVec.z)*180.0f / PI;
	zR = 15.0f;
	zL = -15.0f;
	ArmAngleL -= 10;
	ArmAngleR -= 10;
	
	if (ArmAngleR <= -130.0f)
	{
		ArmAngleL = -130;
		ArmAngleR = -130;
	}

	if (attackMoveCount >= 120)
	{
		zR = 0.0f;
		zL = 0.0f;
		ArmAngleL = 0.0f;
		ArmAngleR = 0.0f;
		AttackCount = 0;
		attackMoveCount = 0;
		stoneShotFlag = false;
		AttackFlag = false;
	}
}

void GolemEnemy::SpecialAttack()
{
	attackMoveCount++;
	//zR = -35.0f;
	//zL = 35.0f;
	bodyAngle.x = 20.0f;
	ArmAngleL -= 20;
	ArmAngleR -= 20;
	//岩を投げる
	
	if (!stoneShotFlag)
	{
		angleVec = Vector3(0, 0, 0);
		angleVec = GetEnemyVec(angleVec);
		targetAngleX = 10;
		bodyAngle.y = atan2(-angleVec.x, -angleVec.z)*180.0f / PI;
		velocity = RotateY(bodyAngle.y + 90.0f)*speed;


		bulletStock++;
		objM->Add(new StoneWeapon(Vector3(position.x, position.y + 1.0f, position.z),
			Vector3(targetAngleX, -bodyAngle.y+60.0f, 0),
			objM, Model, Particle, objType, bulletStock));

		objM->Add(new StoneWeapon(Vector3(position.x, position.y + 1.0f, position.z),
			Vector3(targetAngleX, -bodyAngle.y, 0),
			objM, Model, Particle, objType, bulletStock+1));

		objM->Add(new StoneWeapon(Vector3(position.x, position.y + 1.0f, position.z),
			Vector3(targetAngleX, -bodyAngle.y-60.0f, 0),
			objM, Model, Particle, objType, bulletStock+2));
		stoneShotFlag = true;
	}

	if (!Distance(point.Point, 5.0f)&&stoneShotFlag)//一定以上接近しない
	{
		position += velocity;
		ArmPosL += velocity;
		ArmPosR += velocity;
	}
	else if (attackMoveCount >= 90)
	{
		zR = 0.0f;
		zL = 0.0f;
		bodyAngle.x = 0.0f;
		ArmAngleL = 0.0f;
		ArmAngleR = 0.0f;
		AttackCount = 0;
		attackMoveCount = 0;
		stoneShotFlag = false;
		AttackFlag = false;
	}
	
}

void GolemEnemy::DicideTurnAround()
{
	hitPos = position;
	hitAngle = angle;
}

Vector3 GolemEnemy::GetCampVec(const Vector3 & vec)
{
	Vector3 CampPos = canp.CanpPoint - position;
	return CampPos.normal();
}

void GolemEnemy::TurnAround(int time)
{
}

void GolemEnemy::Guard()
{
	attackMoveCount++;
	zR = 35.0f;
	zL = -35.0f;
	ArmAngleL = -90;
	ArmAngleR = -90;

	if (attackMoveCount >= 120)
	{
		zR = 0.0f;
		zL = 0.0f;
		ArmAngleL = 0.0f;
		ArmAngleR = 0.0f;
		AttackCount = 0;
		attackMoveCount = 0;
		guardFlag = false;
	}
	guardFlag = true;
}

void GolemEnemy::CheckAlive()
{
	if (HP <= 0)
	{
		mDeathAnimation = true;
	}

	if (mDeadFlag)
	{
		death = true;
	}

	/*死亡アニメーションを開始*/
	DeathAnimation();
}

void GolemEnemy::DeathAnimation()
{
	//仮死状態でない　なら処理しない
	if (!mDeathAnimation) return;

	switch (mDeathStep)
	{
	case GolemEnemy::RISE_SKY:
		DeathAnimeStep_RiseSky();
		break;
	case GolemEnemy::EXPLOSION:
		DeathAnimeStep_Explosion();
		break;
	default:
		break;
	}
}

void GolemEnemy::DeathAnimeStep_RiseSky()
{
	mRiseTime->update();

	//時間になっていなければ
	if (!mRiseTime->isTime())
	{
		//回転
		angle.y += 50.0f;
		//上昇
		position.y += 0.2f;
	}
	else
	{
		//時間になったら(1フレームだけ呼ばれる)
		//ここでSEを鳴らしたり、爆発させたりする
		//エフェクト発射


		mDeathStep = DeathAnimationStep::EXPLOSION;
	}
}

void GolemEnemy::DeathAnimeStep_Explosion()
{
	mDeathTime->update();

	if (mDeathTime->isTime())
	{
		mDeadFlag = true;
	}
}

Vector3 GolemEnemy::GetEnemyVec(const Vector3 & vec)
{
	Vector3 EnemyPos = point.Point - position;
	return EnemyPos.normal();
}


void GolemEnemy::Update()
{

	/*生存状態を監視*/
	CheckAlive();

	//仮死状態なら処理しない
	if (mDeathAnimation) return;
	speedTime ++;
	if (speedTime >= speedLimitTime)
	{
		speedTime= 0;
	}
	ImGuiDebug();
	Senser();
	//ySpeed = Easing::ease_in_cubic(sin(speedTime), 0, maxSpeed, 3.0f);
	//position.y += ySpeed;
	if (HitFlag)
	{
		HitCount++;
		if (HitCount >= 30)
		{
			HitCount = 0;
			HitFlag = false;
		}
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
	//球数上限を設け
	if (bulletStock >= 30)
	{
		bulletStock = 0;
	}
}

void GolemEnemy::Rend()
{
		DirectXManager::GetInstance()->SetData3D();//モデル用をセット
		Model->Draw(numNameBody, Vector3(position.x, position.y, position.z), Vector3(bodyAngle.x, -bodyAngle.y, 0), Vector3(3.0f, 3.0f, 3.0f));
		Model->Draw(numNameArmR, Vector3(ArmPosR.x, ArmPosR.y, ArmPosR.z), Vector3(ArmAngleR, -bodyAngle.y, zR), Vector3(3.0f, 3.0f, 3.0f));
		Model->Draw(numNameArmL, Vector3(ArmPosL.x, ArmPosL.y, ArmPosL.z), Vector3(ArmAngleL, -bodyAngle.y, zL), Vector3(3.0f, 3.0f, 3.0f));
}

void GolemEnemy::ImGuiDebug()
{
}

void GolemEnemy::OnCollison(BaseCollider * col)
{
	if (col->GetColObject()->GetType() == ObjectType::BULLET)
	{
		if (!guardFlag)
		{
			HP -= col->GetColObject()->GetDamage();
		}
		HitFlag = true;
	}

	if (col->GetColObject()->GetType() == ObjectType::PLAYER&&AttackFlag)
	{
		bodyAngle.x = 0.0f;
		AttackFlag = false;
	}
	if (col->GetColObject()->GetType() == ObjectType::CAMEAR)
	{
		SetActive(true);
	}
}
