#include "BlowEnemy.h"
#include "../Collision/SpherCollider.h"
#include "../ConstInfomation/Enemy/EnemyConstInfo.h"
#include "../ConstInfomation/Enemy/BlowEnemyConstInfo.h"

namespace ECI = EnemyConstInfo;
namespace BECI = BlowEnemyConstInfo;

BlowEnemy::BlowEnemy(
	Vector3 pos,
	Vector3 ang,
	ObjectManager * objM,
	shared_ptr<ModelRenderer> modelRender,
	shared_ptr<TexRenderer> texRender,
	shared_ptr<ParticleManager> effect,
	int num
) :modelRender(modelRender), effectManager(effect)
{
	position = pos;
	angle = ang;
	objManager = objM;
	number = num;
}

BlowEnemy::~BlowEnemy()
{
	attackArea->SetDeath(true);
}

void BlowEnemy::EnemyInit()
{
#pragma region 変数の初期化

	HP = ECI::MAX_HP * BECI::MAX_HP;
	warningTime = ECI::WARNING_TIME * BECI::WARNING_TIME * 60;
	attackTime = ECI::ATTACK_TIME * BECI::ATTACK_TIME * 60;

	speed = ECI::MOVE_SPEED * BECI::MOVE_SPEED;
	radius = ECI::RADIUS * BECI::RADIUS;
	swingRange = ECI::SWING_RANGE * BECI::SWING_RANGE;
	barrelAngle = BECI::FAN_RANGE;
	turretAngle = 0.0f;
	attackLength = ECI::ATTACK_LENGTH * BECI::ATTACK_LENGTH;
	
	death = false;
	isInvincible = false;
	hitSensor = false;
	swingSensor = false;
	trackingPlayer = false;
	trackingBreadcrumb = false;
	oneShot = false;
	isDestruct = false;
	breadcrumbMode = ECI::BRRADCRUMB_MODE;
	destructMode = ECI::DESTRUCT_MODE;
	turnaroundMode = ECI::TURNAROUND_MODE;

	angle = Vector3(0.0f, 180.0f, 0.0f);//車体の向き
	lastBreadPos = Vector3().zero;
	scale = BECI::SCALE;
	hitPos = Vector3().zero;
	hitAngle = Vector3().zero;

	objType = ObjectType::ENEMY;
	SetCollidder(new SphereCollider(Vector3(position.x, position.y, position.z), radius));

	//プレイヤー位置(仮)初期化
	searchPoint.position = Vector3().zero;
	searchPoint.radius = radius;

	//センサーの初期化
	fanRotateOrigin = -angle.y - 90.0f;
	fanInfo.position = Vector3(position.x, position.y, position.z);//位置
	fanInfo.fanRange = 180.0f;									   //θの角度
	fanInfo.length = 30.0f;										   //長さ
	fanInfo.rotate = fanRotateOrigin;							   //回転角

	//最初は索敵状態
	actionState = ActionState::SEARCH;

	//マップのクリア
	breadMap.clear();

	attackArea = new AttackArea(position, angle, objManager, modelRender, number);

	//配列の要素数を求める
	pointCount = sizeof(patrolPoint) / sizeof(patrolPoint[0]);
	currentPointNumber = 0;

	patrolPoint[0] = Vector3(position.x, position.y, position.z);
	patrolPoint[1] = Vector3(position.x + 10, position.y, position.z);
	patrolPoint[2] = Vector3(position.x + 10, position.y, position.z + 10);
	patrolPoint[3] = Vector3(position.x, position.y, position.z + 10);

#pragma endregion

#pragma region モデルの読み込み

	//戦車の砲身(上下に移動する部分)Barrel
	tankBarrel = BECI::TANK_BARREL_NAME;
	num = to_string(number);
	numBarrel = tankBarrel + num;
	modelRender->AddModel(numBarrel, "Resouse/BoxTankATKAR.obj", "Resouse/BoxTankATKAR.png");
	modelRender->SetAncPoint(numBarrel, Vector3(-2.0f, -2.0f, -2.0f));//中心点の変更

	//戦車の砲塔(上の部分)Turret
	tankTurret = BECI::TANK_TURRET_NAME;
	num = to_string(number);
	numTurret = tankTurret + num;
	modelRender->AddModel(numTurret, "Resouse/BoxTankATKBR.obj", "Resouse/BoxTankATKBR.png");
	modelRender->SetAncPoint(numTurret, Vector3(-2.0f, -2.0f, -2.0f));

	//戦車の車体(下の部分)Body
	tankBody = BECI::TANK_BODY_NAME;
	num = to_string(number);
	numBody = tankBody + num;
	modelRender->AddModel(numBody, "Resouse/BoxTankBTMR.obj", "Resouse/BoxTankBTMR.png");
	modelRender->SetAncPoint(numBody, Vector3(-2.0f, -2.0f, -2.0f));

#pragma endregion
}

void BlowEnemy::EnemyUpdate()
{
	CreateOneObject();                //1度だけ実行される処理
	DestructMode(BECI::MAX_HP / 2, destructMode);//自爆機能
	ChangeState();		              //状態変更
	ImGuiDebug();		              //デバッグ表示
	Invincible(BECI::INVINCIBLE_TIME);//無敵時間
	SearchObject(objManager);         //オブジェクト検索

	//落ちているパンくずを数える
	mapCount = static_cast<int>(breadMap.size());

}

void BlowEnemy::EnemyRend()
{
	//モデルの描画
	DirectXManager::GetInstance()->SetData3D();
	modelRender->Draw(numBarrel, Vector3(position.x, position.y, position.z), Vector3(0, barrelAngle, 0), scale);
	modelRender->Draw(numTurret, Vector3(position.x, position.y, position.z), Vector3(turretAngle, barrelAngle, 0), scale);
	modelRender->Draw(numBody, Vector3(position.x, position.y, position.z), Vector3(0, -angle.y, 0), scale);
}

void BlowEnemy::EnemyOnCollision(BaseCollider * col)
{
	if (col->GetColObject()->GetType() == ObjectType::BREADCRUMB)
	{
		trackingBreadcrumb = false;
	}

	if (col->GetColObject()->GetType() == ObjectType::BULLET ||
		col->GetColObject()->GetType() == ObjectType::PLAYER)
	{
		Damage(1,objManager);
	}
}

void BlowEnemy::EnemyImGuiDebug()
{
	//ImGui::SliderInt("HP", &HP, 0, 10);
	//ImGui::SliderInt("HP", &destructCount, 0, 1000);
	//ImGui::Checkbox("Destruct", &isDestruct);

	//float ePos[3] = { fanInfo.position.x,fanInfo.position.y,fanInfo.position.z };
	float ePos[3] = { angle.x,angle.y,angle.z };
	ImGui::SliderFloat3("EnemyPosition", ePos, -500.0f, 500.0f);
}

void BlowEnemy::Search()
{
	ImGui::Text("ActionState == SEARCH");

	PatrolPoint(patrolPoint,pointCount);
}

void BlowEnemy::Warning()
{
	ImGui::Text("ActionState == WARNING");

	attackArea->SetActive(false);

	TrackingObject();
}

void BlowEnemy::Attack()
{
	ImGui::Text("ActionState == ATTACK");

	attackArea->SetActive(false);

	attackCount++;

	//数秒ごとに当たり判定を自分の前方に表示する
	if (attackCount > attackTime)
	{
		attackCount = 0;
		Vector3 areaPos = AngleToVectorY(fanInfo.rotate) * attackLength;
		attackArea->SetActive(true, position + areaPos, -angle);

		//攻撃が終わったら警戒状態に戻す
		actionState = ActionState::WARNING;
	}
}

void BlowEnemy::Destruct()
{
	ImGui::Text("ActionState == DESTRUCT");

	DestructAction(objManager, modelRender);
}

void BlowEnemy::CreateOneObject()
{
	//当たり判定オブジェクトを一度だけ生成
	if (!oneShot)
	{
		objManager->Add(attackArea);
		oneShot = true;
	}
}
