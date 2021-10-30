#include "BlowEnemy.h"
#include "../Collision/SpherCollider.h"
#include "../ConstInfomation/Enemy/EnemyConstInfo.h"
#include "../ConstInfomation/Enemy/BlowEnemyConstInfo.h"

namespace ECI = EnemyConstInfo;
namespace BECI = BlowEnemyConstInfo;

BlowEnemy::BlowEnemy(
	const Vector3 & pos,
	const Vector3 & ang,
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
	breadcrumbMode = ECI::BRRADCRUMB_MODE;
	destructMode = ECI::DESTRUCT_MODE;
	//turnaroundMode = ECI::TURNAROUND_MODE;
	turnaroundMode = true;

	scale = BECI::SCALE;

	objType = ObjectType::ENEMY;
	SetCollidder(new SphereCollider(Vector3(position.x, position.y, position.z), radius));

	//プレイヤー位置(仮)初期化
	searchPoint.position = Vector3().zero;
	searchPoint.radius = radius;

	//センサーの初期化----------------
	fanRotateOrigin = -angle.y - 90.0f;
	fanInfo.position = Vector3(position.x, position.y, position.z);//位置
	fanInfo.fanRange = 180.0f;									   //θの角度
	fanInfo.length = 30.0f;										   //長さ
	fanInfo.rotate = fanRotateOrigin;							   //回転角
	//--------------------------------

	//最初は索敵状態
	actionState = ActionState::SEARCH;

	//マップのクリア
	breadMap.clear();

	attackArea = new AttackArea(position, angle, objManager, modelRender, number);
	attackArea->SetActive(false);

	//巡回ポイントの初期化------------------------------------
	pointCount = sizeof(patrolPoint) / sizeof(patrolPoint[0]);
	patrolPoint[0] = Vector3(position.x, position.y, position.z);
	patrolPoint[1] = Vector3(position.x + 10, position.y, position.z);
	patrolPoint[2] = Vector3(position.x + 10, position.y, position.z + 10);
	patrolPoint[3] = Vector3(position.x, position.y, position.z + 10);
	//----------------------------------------------------------------

	currentTrigger = false;
	previousTri = false;
	onTrigger = false;

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
	/*当たり判定オブジェクトを生成*/
	CreateOneObject();

	///*自爆機能*/
	//DestructMode(BECI::MAX_HP / 2, destructMode);


	///*無敵時間 & 振り向き処理*/
	////Invincible(ECI::REPORT_INTERVAL);//無敵時間
	Invincible(2);

	//int debugHP = GetID();
	//ImGui::SliderInt("MP-------------------", &debugHP, 0, 500);
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

	if (col->GetColObject()->GetType() == ObjectType::BULLET)
	{
		//ダメージを受ける
		HP -= col->GetColObject()->GetDamage();

		/*報告*/
		InitSearch();
	}
}

void BlowEnemy::EnemyImGuiDebug()
{
#ifndef DEBUG

	//ImGui::SliderInt("HP", &HP, 0, 10);
	//ImGui::SliderInt("HP", &destructCount, 0, 1000);
	/*ImGui::Checkbox("Previous", &previousTri);*/

	//float ePos[3] = { fanInfo.position.x,fanInfo.position.y,fanInfo.position.z };
	//float ePos[3] = { angle.x,angle.y,angle.z };
	//ImGui::SliderFloat3("EnemyPosition", ePos, -500.0f, 500.0f);
	//ImGui::SliderFloat("HP", &barrelAngle, -1000.0f, 1000.0f);

#endif // DEBUG
}

void BlowEnemy::Search()
{
	//PatrolPoint(patrolPoint,pointCount);
	SwingDirection(swingRange);
}

void BlowEnemy::Warning()
{
	//ImGui::Text("ActionState == WARNING");

	attackArea->SetActive(false);

	TrackingObject();
}

void BlowEnemy::Attack()
{
	//ImGui::Text("ActionState == ATTACK");

	//Vector3 areaPos = AngleToVectorY(fanInfo.rotate) * attackLength;
	//attackArea->SetActive(true, position + areaPos, -angle);

	attackCount++;

	//数秒ごとに当たり判定を自分の前方に表示する
	if (attackCount > attackTime)
	{
		attackCount = 0;
		attackArea->SetActive(false);

		//攻撃が終わったら警戒状態に戻す
		actionState = ActionState::WARNING;
	}
}

void BlowEnemy::Destruct()
{
	//ImGui::Text("ActionState == DESTRUCT");

	DestructAction(modelRender);
}

void BlowEnemy::CreateOneObject()
{
	//当たり判定オブジェクトを一度だけ生成
	if (!onTrigger)
	{
		objManager->Add(attackArea);
		attackArea->SetActive(false);
		onTrigger = true;
	}
}
