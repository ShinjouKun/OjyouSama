#include "SniperEnemy.h"
#include "../Weapons/NormalBullet.h"
#include "../Collision/SpherCollider.h"
#include "../ConstInfomation/Enemy/EnemyConstInfo.h"
#include "../ConstInfomation/Enemy/SniperEnemyConstInfo.h"

namespace ECI = EnemyConstInfo;
namespace SECI = SniperEnemyConstInfo;

SniperEnemy::SniperEnemy(
	const Vector3& pos,
	const Vector3& ang,
	ObjectManager * objM,
	shared_ptr<ModelRenderer> modelRender,
	shared_ptr<TexRenderer> texRender,
	shared_ptr<ParticleManager> effect,
	int num
) : modelRender(modelRender), effectManager(effect)
{
	position = pos;
	angle = ang;
	objManager = objM;
	number = num;
}

SniperEnemy::~SniperEnemy()
{
}

void SniperEnemy::EnemyInit()
{
#pragma region 変数の初期化

	HP = ECI::MAX_HP * SECI::MAX_HP;
	warningTime = ECI::WARNING_TIME * SECI::WARNING_TIME * 60;
	attackTime = ECI::ATTACK_TIME * SECI::ATTACK_TIME * 60;

	speed = ECI::MOVE_SPEED * SECI::MOVE_SPEED;
	radius = ECI::RADIUS * SECI::RADIUS;
	swingRange = ECI::SWING_RANGE * SECI::SWING_RANGE;
	barrelAngle = SECI::FAN_RANGE;
	turretAngle = 0.0f;
	attackLength = ECI::ATTACK_LENGTH * SECI::ATTACK_LENGTH;

	death = false;
	trackingPlayer = false;
	trackingBreadcrumb = false;
	breadcrumbMode = ECI::BRRADCRUMB_MODE;
	destructMode = ECI::DESTRUCT_MODE;
	turnaroundMode = ECI::TURNAROUND_MODE;

	angle = Vector3(0.0f, 180.0f, 0.0f);//車体の向き
	scale = SECI::SCALE;

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

#pragma endregion

#pragma region モデルの読み込み

	//戦車の砲身(上下に移動する部分)Barrel
	tankBarrel = SECI::TANK_BARREL_NAME;
	num = to_string(number);
	numBarrel = tankBarrel + num;
	modelRender->AddModel(numBarrel, "Resouse/BoxTankATKAR.obj", "Resouse/BoxTankATKAR.png");
	modelRender->SetAncPoint(numBarrel, Vector3(-2.0f, -2.0f, -2.0f));//中心点の変更

	//戦車の砲塔(上の部分)Turret
	tankTurret = SECI::TANK_TURRET_NAME;
	num = to_string(number);
	numTurret = tankTurret + num;
	modelRender->AddModel(numTurret, "Resouse/BoxTankATKBR.obj", "Resouse/BoxTankATKBR.png");
	modelRender->SetAncPoint(numTurret, Vector3(-2.0f, -2.0f, -2.0f));

	//戦車の車体(下の部分)Body
	tankBody = SECI::TANK_BODY_NAME;
	num = to_string(number);
	numBody = tankBody + num;
	modelRender->AddModel(numBody, "Resouse/BoxTankBTMR.obj", "Resouse/BoxTankBTMR.png");
	modelRender->SetAncPoint(numBody, Vector3(-2.0f, -2.0f, -2.0f));

#pragma endregion
}

void SniperEnemy::EnemyUpdate()
{
	DestructMode(SECI::MAX_HP / 2, destructMode);//自爆機能
	ChangeState();		              //状態変更
	ImGuiDebug();		              //デバッグ表示
	Invincible(ECI::REPORT_INTERVAL);//無敵時間
	SearchObject(objManager);         //オブジェクト検索
}

void SniperEnemy::EnemyRend()
{
	//モデルの描画
	DirectXManager::GetInstance()->SetData3D();
	modelRender->Draw(numBarrel, Vector3(position.x, position.y, position.z), Vector3(0, barrelAngle, 0), scale);
	modelRender->Draw(numTurret, Vector3(position.x, position.y, position.z), Vector3(turretAngle, barrelAngle, 0), scale);
	modelRender->Draw(numBody, Vector3(position.x, position.y, position.z), Vector3(0, -angle.y, 0), scale);
}

void SniperEnemy::EnemyOnCollision(BaseCollider * col)
{
	if (col->GetColObject()->GetType() == ObjectType::BREADCRUMB)
	{
		trackingBreadcrumb = false;
	}

	if (col->GetColObject()->GetType() == ObjectType::BULLET)
	{
		//ダメージを受ける
		HP -= col->GetColObject()->GetDamage();

		Report(objManager, modelRender);
	}
}

void SniperEnemy::EnemyImGuiDebug()
{
	//ImGui::SliderInt("HP", &destructCount, 0, 1000);
}

void SniperEnemy::Search()
{
	//ImGui::Text("ActionState == SEARCH");

	//首振り索敵型
	SwingDirection(swingRange);
}

void SniperEnemy::Warning()
{
	//ImGui::Text("ActionState == WARNING");

	//追跡機能
	TrackingObject();
}

void SniperEnemy::Attack()
{
	//ImGui::Text("ActionState == Attack");

	attackCount++;

	if (attackCount > attackTime)
	{
		attackCount = 0;
		Vector3 firePos = AngleToVectorY(fanInfo.rotate);

		//弾を発射！！
		objManager->Add(new NormalBullet(position + firePos, Vector3(0, -angle.y, 0), objManager, modelRender, effectManager, objType, bulletNumber));
		bulletNumber++;
		actionState = ActionState::WARNING;
	}
}

void SniperEnemy::Destruct()
{
	//ImGui::Text("ActionState == DESTRUCT");

	//DestructAction(objManager, modelRender);
}
