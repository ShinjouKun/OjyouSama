#include "TestEnemy.h"
#include "BreadCrumb.h"
#include "../Collision/SpherCollider.h"
#include "../Device/Input.h"

/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="position">初期位置</param>
/// <param name="angle">初期角度</param>
/// <param name="objManager">オブジェクト管理者</param>
/// <param name="modelRender">モデル描画</param>
/// <param name="texRender">ポリゴン描画</param>
/// <param name="num">識別番号</param>
TestEnemy::TestEnemy(
	Vector3 pos,
	Vector3 ang,
	ObjectManager * objM,
	shared_ptr<ModelRenderer> modelRender,
	shared_ptr<TexRenderer> texRender,
	int num
)
	:modelRender(modelRender)/*, texRender(texRender)*/
{
	//値の代入
	position = pos;//←同意→BaseObject::position = pos;
	angle = ang;   //←同意→BaseObject::angle = ang;
	objManager = objM;
	number = num;
}

/// <summary>
/// デストラクタ
/// </summary>
TestEnemy::~TestEnemy()
{
}

/// <summary>
/// 初期化とモデルの読み込み
/// </summary>
void TestEnemy::Init()
{
#pragma region 変数の初期化(多くなった)

	HP = 2;
	mapCount = 0;
	breadCount = 0;//シーン内のパンくずの数。
	attackCount = 0;
	attackTime = 1 * 60;
	invincibleCount = 0;

	speed = 0.2f;
	radius = 1.0f;
	swingRange = 90.0f;
	barrelAngle = 180.0f;
	turretAngle = 0.0f;
	attackLength = 5.0f;

	death = false;
	isInvincible = false;
	hitSensor = false;
	swingSensor = false;
	trackingPlayer = false;
	trackingBreadcrumb = false;
	oneShot = false;

	angle = Vector3(0.0f, 180.0f, 0.0f);//車体の向き
	lastBreadPos = Vector3(0, 0, 0);
	scale = Vector3(1.5f, 1.5f, 1.5f);

	objType = ObjectType::ENEMY;
	SetCollidder(new SphereCollider(Vector3(position.x, position.y, position.z), radius));

	//プレイヤーの位置(仮)
	searchPoint =
	{
		Vector3(100.0f,-3.0f,-50.0f),//位置
		32.0f                        //半径
	};

	//センサーの初期化
	fanRotateOrigin = -angle.y - 90.0f;
	fanInfo.position = Vector3(position.x, position.y, position.z);//位置
	fanInfo.fanRange = 180.0f;									   //θの角度
	fanInfo.length = 30.0f;										   //長さ
	fanInfo.rotate = fanRotateOrigin;							   //回転角
	SetFanInfo(fanInfo.fanRange, fanInfo.length);//設定の登録！

	actionState = ActionState::SEARCH;

	//マップは最初に初期化しといたほうがいいらしい！
	breadMap.clear();

	//攻撃判定生成
	area = new AttackArea(position, angle, objManager, modelRender, number);

#pragma endregion

#pragma region モデルの読み込み

	//戦車の砲身(上下に移動する部分)Barrel
	tankBarrel = "Barrel";
	num = to_string(number);
	numBarrel = tankBarrel + num;
	modelRender->AddModel(numBarrel, "Resouse/BoxTankATKAR.obj", "Resouse/BoxTankATKAR.png");
	modelRender->SetAncPoint(numBarrel, Vector3(-2.0f, -2.0f, -2.0f));//中心点の変更

	//戦車の砲塔(上の部分)Turret
	tankTurret = "Turret";
	num = to_string(number);
	numTurret = tankTurret + num;
	modelRender->AddModel(numTurret, "Resouse/BoxTankATKBR.obj", "Resouse/BoxTankATKBR.png");
	modelRender->SetAncPoint(numTurret, Vector3(-2.0f, -2.0f, -2.0f));

	//戦車の車体(下の部分)Body
	tankBody = "Body";
	num = to_string(number);
	numBody = tankBody + num;
	modelRender->AddModel(numBody, "Resouse/BoxTankBTMR.obj", "Resouse/BoxTankBTMR.png");
	modelRender->SetAncPoint(numBody, Vector3(-2.0f, -2.0f, -2.0f));

#pragma endregion

}

void TestEnemy::Update()
{
	//当たり判定オブジェクトを一度だけ生成
	if (!oneShot)
	{
		objManager->Add(area);
		oneShot = true;
	}

	Invincible(3);  //無敵時間
	SearchObject();      //オブジェクト検索
	TrackingBreadcrumb();//パンくずを追いかける
	ChangeState();		 //状態変更
	ImGuiDebug();		 //デバッグ表示

	//かぞえるー
	mapCount = breadMap.size();
}

void TestEnemy::Rend()
{
	//モデルの描画
	DirectXManager::GetInstance()->SetData3D();
	//tankM->Draw(numBarrel, Vector3(position.x, position.y, position.z), Vector3(0, -angle.y, 0), scale);
	modelRender->Draw(numBarrel, Vector3(position.x, position.y, position.z), Vector3(0, barrelAngle, 0), scale);
	//tankM->Draw(numTurret, Vector3(position.x, position.y, position.z), Vector3(turretAngle, -angle.y, 0), scale);
	modelRender->Draw(numTurret, Vector3(position.x, position.y, position.z), Vector3(turretAngle, barrelAngle, 0), scale);
	modelRender->Draw(numBody, Vector3(position.x, position.y, position.z), Vector3(0, -angle.y, 0), scale);
}

void TestEnemy::OnCollison(const CollisonInfo & info)
{
	//衝突処理
	if (info.object->GetType() == ObjectType::BULLET ||
		info.object->GetType() == ObjectType::PLAYER)
	{
		//無敵時間で無ければダメージ
		if (isInvincible) return;
		HP--;
		isInvincible = true;
	}

	if (info.object->GetType() == ObjectType::BREADCRUMB)
	{
		//trackingBreadcrumb = false;
	}
}

void TestEnemy::ImGuiDebug()
{
	//扇情報の可視化
	ImGui::SliderFloat("Range(Kakudo)", &fanInfo.fanRange, 0.0f, 300.0f);
	ImGui::SliderFloat("Length(Nagasa)", &fanInfo.length, 0.0f, 300.0f);
	ImGui::SliderFloat("Rotate(KaitenKakudo)", &fanInfo.rotate, -360, 360.0f);

	//敵情報の可視化
	float ePos[3] = { fanInfo.position.x,fanInfo.position.y,fanInfo.position.z };
	//float ePos[3] = { angle.x,angle.y,angle.z };
	ImGui::SliderFloat3("EnemyPosition", ePos, -500.0f, 500.0f);
	ImGui::SliderFloat("--Degree---", &angle.y, -360, 360);
	ImGui::SliderFloat("BarrelAngle", &barrelAngle, -360, 360);

	//扇と点が当たってたらチェックがつくよ。
	ImGui::Checkbox("IsHitFan2Point", &hitSensor);
	ImGui::SliderInt("BreadNumber", &breadCount, 0, 500);
	ImGui::SliderInt("MapCount", &mapCount, 0, 500);

	ImGui::Checkbox("************************", &isInvincible);
}

void TestEnemy::SetFanInfo(float range, float length)
{
	fanInfo.fanRange = range;//θの角度
	fanInfo.length = length;//長さ
}

void TestEnemy::Move(Vector3 otherPosition)
{
	Vector3 distance = otherPosition - position;
	//Vector3 len = position - otherPosition;
	float length = distance.Length();
	distance = distance.normal();//正規化忘れずに

	float radian = atan2(distance.x, distance.z);
	//回転を反映
	angle.y = RadianToDegree(radian) + 180.0f;
	fanInfo.rotate = -angle.y - 90.0f;
	barrelAngle = fanInfo.rotate + 90.0f;
	//velocity = RotateY(angle.y + 90.0f) * speed;

	velocity = distance * speed;
	//位置とセンサーを反映
	position += velocity;
	fanInfo.position = position;
}

void TestEnemy::ChangeState()
{
	//死亡処理に移る
	if (HP <= 0)
	{
		death = true;
		area->SetDeath(true);
	}

	//攻撃中でない & プレイヤーを追っていない & パンくずを辿っていない
	if (!actionState == ActionState::ATTACK && !hitSensor && !trackingBreadcrumb)
	{
		actionState = ActionState::SEARCH;
	}

	switch (actionState)
	{
	case TestEnemy::SEARCH:
		Search();//首振り機能
		break;
	case TestEnemy::WARNING:
		Warning();
		break;
	case TestEnemy::ATTACK:
		Attack();
		break;
	default:
		break;
	}
}

void TestEnemy::SearchObject()
{
	//シーン上のオブジェクトを全て検索して...
	for (auto& type : objManager->getUseList())
	{
		//シーン上にプレイヤーが存在することを確認する
		if (type->GetType() == ObjectType::PLAYER)
		{
			SearchPlayer(type);
		}

		//プレイヤーとは別で、シーン上のパンくずの存在を確認...
		if (type->GetType() == ObjectType::BREADCRUMB)
		{
			SearchBreadCrumb(type);
		}
	}
}

void TestEnemy::SearchPlayer(BaseObject * player)
{
	//センサーの情報をプレイヤー情報で更新
	searchPoint.position = player->GetPosition();
	searchPoint.radius = 1.0f;

	//センサーを更新
	hitSensor = IsHitFanToPoint(fanInfo, searchPoint.position,searchPoint.radius);

	if (actionState == ActionState::ATTACK) return;

	//ここで初めて、プレイヤーと当たっていたらの処理に入る。
	if (hitSensor)
	{
		trackingPlayer = true;
		trackingBreadcrumb = false;

		//追跡状態に移行
		actionState = ActionState::WARNING;

		//移動
		Move(searchPoint.position);

		Vector3 distance = fanInfo.position - searchPoint.position;
		float length = distance.Length();

		//対象との距離が1.0以下になったら到着完了とする。
		if (length < attackLength)
		{
			actionState = ActionState::ATTACK;
		}
	}
	else
	{
		trackingPlayer = false;
	}
}

void TestEnemy::SearchBreadCrumb(BaseObject * breadcrumb)
{
	//位置取得 & センサー情報更新
	Vector3 breadPos = breadcrumb->GetPosition();
	searchPoint.position = breadPos;

	//パンくずを辿っている & プレイヤーを追いかけている時は処理しない
	if (trackingBreadcrumb && trackingPlayer) return;

	//サーチライトに当たっているオブジェクトのみ、マップに格納
	if (IsHitFanToPoint(fanInfo, searchPoint.position))
	{
		//存在するオブジェクトの個体番号を取得
		//breadCount = breadcrumb->GetNumber();
		breadCount = breadcrumb->GetID();

		auto itr = breadMap.find(breadCount);
		//マップに指定したキーが入っていなかったら
		if (itr == breadMap.end())
		{
			breadMap.emplace(breadCount, breadPos);
		}

		////「tyr_emplace」 = キーが存在しない場合のみ、要素を格納する
		//breadMap.try_emplace(breadCount, type->GetPosition());
	}
}

void TestEnemy::TrackingBreadcrumb()
{
	//プレイヤーを追跡中は処理しない
	if (trackingPlayer) return;

	//移動していない && マップが空でない
	if (!trackingBreadcrumb && !breadMap.empty())
	{
		int lastKey = 0;
		for (auto itr = breadMap.begin(); itr != breadMap.end(); ++itr)
		{
			//最初から最後まで上書きするから、結果的に最後の要素が得られる？
			lastKey = itr->first;
		}

		////最初の要素を取得
		//auto begin = breadMap.begin();
		//lastKey = begin->first;

		//最後の要素を取得
		lastBreadPos = breadMap[lastKey];

		if (!breadMap.empty())
		{
			//マップを一旦空にする
			breadMap.clear();
		}

		//移動状態にする
		trackingBreadcrumb = true;
	}

	if (trackingBreadcrumb)
	{
		//移動
		Move(lastBreadPos);

		//警戒状態に移行
		actionState = ActionState::WARNING;

		Vector3 distance = fanInfo.position - lastBreadPos;
		float length = distance.Length();

		//対象との距離が1.0以下になったら到着完了とする。
		if (length < 1.0f)
		{
			trackingBreadcrumb = false;
		}
	}
}

void TestEnemy::Search()
{
	//設定の初期化
	SetFanInfo();

	ImGui::Text("ActionState == SEARCH");

	//現在の向きと扇の向きを更新
	fanRotateOrigin = -angle.y - 90.0f;

	//基準は-270 最大は-360 最小は-180
	if (swingSensor)
	{
		fanInfo.rotate++;
		if (fanInfo.rotate > fanRotateOrigin + swingRange)//-179
		{
			swingSensor = false;
		}
	}
	else
	{
		fanInfo.rotate--;
		if (fanInfo.rotate < fanRotateOrigin - swingRange)//-361
		{
			swingSensor = true;
		}
	}

	//砲塔も回転
	barrelAngle = fanInfo.rotate + 90.0f;
}

void TestEnemy::Warning()
{
	//この時は索敵範囲を大きくしたい。

	SetFanInfo(120.0f);

	ImGui::Text("ActionState == WARNING");


}

void TestEnemy::Attack()
{
	SetFanInfo(120.0f);
	ImGui::Text("ActionState == TRACKING");

	Vector3 areaPos = AngleToVectorY(fanInfo.rotate) * attackLength;
	area->SetActive(true, position + areaPos, angle);

	attackCount++;

	if (attackCount > attackTime)
	{
		attackCount = 0;
		area->SetActive(false);
		actionState = ActionState::WARNING;
	}
}

void TestEnemy::Invincible(int time)
{
	if (!isInvincible) return;

	time *= 60;

	invincibleCount++;

	//無敵時間終了
	if (invincibleCount > time)
	{
		invincibleCount = 0;
		isInvincible = false;
	}
}

void TestEnemy::ChangeDirection()
{
	//移動方向を向くようにしたテスト。

	Vector3 diff = position - previousPos;
	previousPos = position;
	diff.normalize();
	float radian = atan2(diff.x, diff.z);
	//回転を反映
	angle.y = RadianToDegree(radian) + 180.0f;
}

float TestEnemy::DegreeToRadian(float angle)
{
	float radian;
	radian = angle * (PI / 180.0f);

	return radian;
}

float TestEnemy::RadianToDegree(float radian)
{
	float degree;
	degree = radian * (180.0f / PI);

	return degree;
}

Vector3 TestEnemy::AngleToVectorY(float angle)
{
	Vector3 vector;

	//角度をラジアン角に戻す
	float radian = DegreeToRadian(angle);

	float x = cosf(radian);
	float z = sinf(radian);

	vector = Vector3(x, 0, z);
	//単位ベクトル化
	vector.normalize();

	return vector;
}

float TestEnemy::VectorToAngleY(Vector3 vector)
{
	float angle = atan2f(vector.x, vector.z);

	RadianToDegree(angle);

	return angle;
}

bool TestEnemy::IsHitFanToPoint(FanInfomation fan, Vector3 point, float radius)
{
	//①　点と扇中心点のベクトルを求める
	Vector3 vectorFanToPoint =
	{
		point.x - fan.position.x,
		point.y - fan.position.y,
		point.z - fan.position.z,
	};

	//②　①のベクトルの長さを求める
	float vectorLength = sqrtf(
		(vectorFanToPoint.x * vectorFanToPoint.x) +
		(vectorFanToPoint.y * vectorFanToPoint.y) +
		(vectorFanToPoint.z * vectorFanToPoint.z)
	);

	//③　ベクトルの長さと、扇の範囲の長さを比較
	if (fan.length + radius < vectorLength)
	{
		//扇の方が小さい場合はあたっていない
		return false;
	}



	//④　円弧の方向ベクトルを求める
	float rotateRadian = DegreeToRadian(fan.rotate);

	//⑤　0度の扇の角度の単位ベクトル(arc = 円弧)
	Vector3 arcDirection = Vector3(1, 0, 0);

	//⑥　ベクトルを回転させる(x,z)
	Vector3 rotateArcDirection
	{
		arcDirection.x * cosf(rotateRadian) + arcDirection.z * -sinf(rotateRadian),
		0.0f,
		arcDirection.x * 1.0f * sinf(rotateRadian) + arcDirection.z * cosf(rotateRadian)
	};

	//⑦　扇と点のベクトルを単位ベクトル化
	Vector3 normalFanToPoint =
	{
		vectorFanToPoint.x / vectorLength,
		vectorFanToPoint.y / vectorLength,
		vectorFanToPoint.z / vectorLength,
	};

	//⑧　内積の計算
	float dot =
		normalFanToPoint.x * rotateArcDirection.x +
		normalFanToPoint.y * rotateArcDirection.y +
		normalFanToPoint.z * rotateArcDirection.z;

	//⑨　扇の範囲をcosの値にする
	float fanCos = cosf(DegreeToRadian(fan.fanRange / 2.0f));

	//⑩　点が扇の範囲内にあるか比較をする
	if (fanCos > dot)
	{
		//当たっていない
		return false;
	}

	//ここまで到達したら当たっている
	return true;
}
