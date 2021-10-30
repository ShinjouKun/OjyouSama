#include "BaseEnemy.h"
#include "../Collision/SpherCollider.h"

#include "ReportArea.h"
#include "TestWayPoint.h"
#include "WayPointManager.h"
#include "BreadCrumbCreater.h"
#include "TestBreadCrumb.h"
#include "../Utility/Timer/Timer.h"

BreadCrumbCreater* BaseEnemy::mBreadCreator = nullptr;
EnemyAI* BaseEnemy::mEnemyAI = nullptr;
ObjectManager* BaseEnemy::mManager = nullptr;

BaseEnemy::~BaseEnemy()
{

}

void BaseEnemy::Init()
{
	Initialize();
	EnemyInit();
}

void BaseEnemy::Update()
{
	mPlayerPosition = mManager->GetPlayer().GetPosition();

	//プレイヤーとの距離が一定以下なら
	if (InsideDistance(mPlayerPosition, 150.0f))
	{
		isActive = true;
	}
	else//そうでないなら
	{
		isActive = false;
	}

	/*表示状態でないなら処理しない*/
	if (!isActive) return;

	if (!trackingPlayer && !trackingBreadcrumb)
	{
		//5:AIから報告主に近い敵のIDリストを受け取る
		for (int i = 0, end = mEnemyAI->GetIDList().size(); i < end; i++)
		{
			//6:自身と受け取ったIDが一致しているかを確認する
			if (GetID() == mEnemyAI->GetIDList()[i])
			{
				if (!InsideDistance(mEnemyAI->GetPositionList(), 5.0f))
				{
					//7:一致していたら、報告主の位置に移動する
					Move(mEnemyAI->GetPositionList());
				}
			}
		}
	}

	//if (goalFlag)
	//{
	//	//角度を一度だけ取得
	//	Vector3 test = this->angle;
	//}

	/*共通の要素*/
	ChangeState(); //状態変更
	SearchObject();//パンくずやプレイヤーを探す
	EnemyUpdate();
}

void BaseEnemy::Rend()
{
	if (!isActive) return;

	//EnemyRend();
}

//ここはオブジェクトにぶつかった時しか入らない。
void BaseEnemy::OnCollison(BaseCollider * col)
{
	if (!isActive) return;

	EnemyOnCollision(col);
}

void BaseEnemy::ImGuiDebug()
{
	if (!isActive) return;

	EnemyImGuiDebug();
}

void BaseEnemy::Initialize()
{
	//number = 0;
	attackCount = 0;

	trackingPlayer = false;
	trackingBreadcrumb = false;

	warningCount = 0;
	invincibleCount = 0;
	destructCount = 0;
	breadCount = 0;
	patrolCount = 0;
	searchCount = 0;
	loopCount = 0;
	moveCount = 0;
	arrayCount = 0;

	isInvincible = false;
	hitSensor = false;
	swingSensor = false;
	isDestruct = false;
	finishSearchWay = false;
	hitReportArea = false;
	moveWayPoint = false;
	goalFlag = false;
	isActive = true;

	lastBreadPos = Vector3().zero;
	previousPos = Vector3().zero;
	hitPos = Vector3().zero;
	hitAngle = Vector3().zero;
	goalPoint = Vector3().zero;
	resultPoint = Vector3().zero;

	breadMap.clear();
	moveList.clear();

	//mTarget.clear();
	//mPointList.clear();
	//mPointList.shrink_to_fit();//メモリの削除

	//mReportArea = nullptr;
}

void BaseEnemy::Move(const Vector3 & otherPosition)
{
	//距離を調べる(Vector3とfloat)
	Vector3 distance = otherPosition - position;
	float length = distance.Length();
	distance = distance.normal();//正規化忘れずに

	//二点間の角度を求める
	float radian = atan2(distance.x, distance.z);
	//回転を反映
	angle.y = Math::toDegrees(radian) + 180.0f;
	fanInfo.rotate = -angle.y - 90.0f;
	barrelAngle = fanInfo.rotate + 90.0f;

	velocity = distance * speed;
	//位置とセンサーを反映
	position += velocity;
	fanInfo.position = position;
}

void BaseEnemy::SetFanInfo(float range, float length)
{
	//角度と長さを再設定
	fanInfo.fanRange = range;//θの角度
	fanInfo.length = length; //長さ
}

//ここも常に呼ばれてるよ。
void BaseEnemy::ChangeState()
{
	//オブジェクトの追跡開始
	StartTracking();

	if (mDeathFlag)
	{
		death = true;
	}

	//死亡処理に移る
	if (HP <= 0)
	{
		mDeathFlag = true;
	}

	//各状態ごとの処理
	switch (actionState)
	{
	case BaseEnemy::SEARCH:
		Search();//首振り機能
		break;
	case BaseEnemy::WARNING:
		Warning();
		break;
	case BaseEnemy::ATTACK:
		Attack();
		break;
	case BaseEnemy::DESTRUCT:
		Destruct();
		break;
	default:
		break;
	}
}

void BaseEnemy::SearchObject()
{
	//mIntervalCount++;

	//索敵中は検索回数を少なくする
	if (actionState == ActionState::SEARCH)
	{
		//if (mIntervalCount % 60 == 0)
		//{
		//	//シーン上のプレイヤーの存在を確認
		//	SearchPlayer();

		//	//プレイヤーを追っていない & パンくず追跡機能がON　なら処理を実行
		//	if (!trackingPlayer && breadcrumbMode)
		//	{
		//		auto BList = mBreadCreator->GetBreadList();

		//		for (int i = 0, end = static_cast<int>(BList.size()); i < end; i++)
		//		{
		//			SearchBreadCrumbTest(*BList[i]);
		//		}
		//	}
		//}
	}
	//else
	//{
	//	////4で割り切れる値になったら
	//	////(60fpsなので1秒に15回検索)
	//	//if (mIntervalCount % 4 == 0)
	//	//{
	//		//シーン上のプレイヤーの存在を確認
	//		SearchPlayer();

	//		//プレイヤーを追っていない & パンくず追跡機能がON　なら処理を実行
	//		if (!trackingPlayer && breadcrumbMode)
	//		{
	//			//シーン上のパンくずの存在を確認
	//			auto BList = mBreadCreator->GetBreadList();

	//			for (int i = 0, end = static_cast<int>(BList.size()); i < end; i++)
	//			{
	//				SearchBreadCrumbTest(*BList[i]);
	//			}
	//		}
	//	//}
	//}

	//if (mIntervalCount > 60)
	//{
	//	//60超えたら0に戻しておく
	//	mIntervalCount = 0;
	//}

}

void BaseEnemy::SearchPlayer()
{
	//センサーの情報をプレイヤー情報で更新
	searchPoint.radius = 1.0f;
	searchPoint.position = mPlayerPosition;

	//センサーを更新
	hitSensor = IsHitFanToPoint(fanInfo, searchPoint.position, searchPoint.radius);

	//プレイヤーがセンサーの中に入っていたら
	if (hitSensor)
	{
		trackingPlayer = true;     //プレイヤーの追跡を開始する
		trackingBreadcrumb = false;//パンくずの追跡を終了する

		//マップを一旦空にする
		if (!breadMap.empty())
		{
			breadMap.clear();
		}
	}
	else trackingPlayer = false;
}

void BaseEnemy::SearchBreadCrumbTest(const TestBreadCrumb & breadCrumb)
{
	//パンくずを辿っている & プレイヤーを追っている なら処理しない
	if (trackingBreadcrumb && trackingPlayer) return;

	//位置取得 & センサー情報更新
	Vector3 breadPos = breadCrumb.GetPosition();
	searchPoint.position = breadPos;

	//サーチライトに当たっているオブジェクトのみ、マップに格納
	if (IsHitFanToPoint(fanInfo, searchPoint.position))
	{

		//オブジェクトの個体番号を取得
		breadCount = breadCrumb.GetBreadNumber();

		//マップに指定したキーが入っていなかったら追加する
		auto itr = breadMap.find(breadCount);
		if (itr == breadMap.end())
		{

			breadMap.emplace(breadCount, breadPos);
		}
	}
}

void BaseEnemy::StartTracking()
{
	//索敵中からしか振り向かないようにした
	if (actionState == ActionState::SEARCH)
	{
		if (trackingPlayer || trackingBreadcrumb)
		{
			actionState = ActionState::WARNING;
		}
	}
}

void BaseEnemy::TrackingBreadcrumb()
{
	//プレイヤーを追跡中 なら処理しない
	if (trackingPlayer) return;

	//パンくずを追跡中でない && マップが空でない
	if (!trackingBreadcrumb && !breadMap.empty())
	{
		int lastKey = 0;
		for (auto itr = breadMap.begin(), end = breadMap.end(); itr != end; ++itr)
		{
			//最初から最後まで上書きするから、結果的に最後の要素が得られる？
			lastKey = itr->first;
		}

		//最後の要素を取得
		lastBreadPos = breadMap[lastKey];

		//マップを一旦空にする
		if (!breadMap.empty())
		{
			breadMap.clear();
		}

		//移動状態にする
		trackingBreadcrumb = true;
	}
}

void BaseEnemy::Invincible(int time)
{
	//無敵時間中でない なら処理をしない
	if (!isInvincible) return;

	time *= 60;

	invincibleCount++;

	//振り向き開始
	TurnAround(time);

	//無敵時間終了
	if (invincibleCount > time)
	{
		invincibleCount = 0;
		isInvincible = false;
	}
}

void BaseEnemy::SwingDirection(float range)
{
	//現在の向きを最新に更新
	fanRotateOrigin = -angle.y - 90.0f;

	//基準は-270 最大は-360 最小は-180
	if (swingSensor)//左回転
	{
		fanInfo.rotate++;
		if (fanInfo.rotate > fanRotateOrigin + swingRange)
		{
			swingSensor = false;
		}
	}
	else//右回転
	{
		fanInfo.rotate--;
		if (fanInfo.rotate < fanRotateOrigin - swingRange)
		{
			swingSensor = true;
		}
	}

	//砲塔も回転
	barrelAngle = fanInfo.rotate + 90.0f;
}

void BaseEnemy::PatrolPoint(const Vector3 points[], int pointCount)
{
	//配列の番号の位置を取得
	Vector3 targetPosition = points[patrolCount];

	//移動
	Move(targetPosition);

	//到着したとする
	if (InsideDistance(targetPosition, 0.1f))
	{
		//ゴールにト着したら(右辺は、配列の要素数を求めています)
		if (patrolCount + 1 == pointCount)
		{
			patrolCount = 0;
		}
		else
		{
			patrolCount++;
		}
	}
}

void BaseEnemy::TrackingObject()
{
	//プレイヤーを追っているとき
	if (trackingPlayer)
	{
		//移動
		Move(searchPoint.position);

		//対象との距離が以下になったら到着完了とする。
		if (InsideDistance(searchPoint.position, attackLength))
		{
			actionState = ActionState::ATTACK;
		}

	}
	else
	{
		// パンくずを追いかける
		TrackingBreadcrumb();

		if (trackingBreadcrumb)
		{
			//移動
			Move(lastBreadPos);

			//対象との距離が以下になったら到着完了とする。
			if (InsideDistance(lastBreadPos, 1.0f))
			{
				trackingBreadcrumb = false;
			}
		}
	}

	//一定時間で索敵状態に戻る
	if (!trackingPlayer && !trackingBreadcrumb)
	{
		warningCount++;

		if (warningCount > warningTime)
		{
			warningCount = 0;
			actionState = ActionState::SEARCH;
		}
	}
}

void BaseEnemy::DestructAction(shared_ptr<ModelRenderer> modelRender)
{
	/*自爆カウントを増やす*/
	destructCount++;

	//一定時間プレイヤーを追いかけたら、大きめの当たり判定を出して死ぬ。
	if (destructCount > 2 * 60)
	{
		Vector3 areaPos = AngleToVectorY(fanInfo.rotate) * attackLength;
		destructArea->SetActive(true, position, -angle, Vector3(5, 5, 5));
		destructArea->SetDestroy(true, 2);
		HP = 0;
	}

	//移動
	Move(searchPoint.position);

	//一度だけ実行
	if (!isDestruct)
	{
		isDestruct = true;
		speed *= 2.0f;

		destructArea = new AttackArea(position, angle, mManager, modelRender, number);
		mManager->Add(destructArea);
	}
}

void BaseEnemy::InitSearch()
{
	//すでに報告している or 現在移動中 なら表示しない
	if (isInvincible || moveWayPoint || trackingPlayer) return;

	isInvincible = true;

	//振り向き機能追加
	DicideTurnAround();

	//死亡処理に移る
	if (HP <= 0)
	{
		mDeathFlag = true;
		return;
	}

	//2:自分の位置情報をAIに転送
	mEnemyAI->SetValue(position);
}

//void BaseEnemy::GetAllWayPoint()
//{
//	////全てのポイントのフラグを初期化
//	//mManager->ResetFlag();
//
//	//mPointList.clear();
//	//mPointList.resize(mManager->GetTestPointList().size());
//	//mPointList = mManager->GetTestPointList();
//
//	////移動リストのメモリを確保
//	//moveList.reserve(mPointList.size());
//
//	//mManager->Remove();
//	//mWay.reset();//←上手く消せてるかわからない
//}
//
//shared_ptr<TestWayPoint> BaseEnemy::NearWayPointStartTest(const Vector3 & point) const
//{
//	float distance = 0;
//	float mostDistance = 0;
//	shared_ptr<TestWayPoint> wayPoint;
//
//	for (int i = 0, end = static_cast<int>(mPointList.size()); i < end; i++)
//	{
//		Vector3 dist = mPointList[i]->GetPosition() - point;
//		distance = dist.Length();
//
//		//まだ使用していないもののみ処理を行う
//		if (!mPointList[i]->GetUseFlag())
//		{
//			if (mostDistance == 0 || mostDistance > distance)
//			{
//				mostDistance = distance;
//				wayPoint = mPointList[i];
//			}
//		}
//	}
//
//	//使用済みに変更
//	wayPoint->SetUseFlag(true);
//
//	return wayPoint;
//}
//
//void BaseEnemy::SerachWayPoint()
//{
//	//報告範囲に当たっていない or ゴールに到着している or 自分が報告している(isInvisible) なら処理しない
//	if (!hitReportArea || finishSearchWay || isInvincible) return;
//
//	loopCount++;
//
//	SearchPointToArrayTest(static_cast<int>(mTarget.size()));
//
//	resultPoint = NearWayPointArrayTest(mTarget, goalPoint)->GetPosition();
//
//	searchCount++;
//	moveList.push_back(resultPoint);
//
//	if (!finishSearchWay)
//	{
//		ClearFlag();
//	}
//}
//
//void BaseEnemy::SearchPointToArrayTest(int length)
//{
//	//自分から近いポイントを配列の数分取得する
//	for (int i = 0; i < length; i++)
//	{
//		mTarget[i] = NearWayPointStartTest(moveList[searchCount]);
//	}
//}
//
//shared_ptr<TestWayPoint> BaseEnemy::NearWayPointArrayTest(const vector<shared_ptr<TestWayPoint>>& trans, const Vector3 & goal)
//{
//	float distance = 0;
//	float mostDistance = 0;
//	shared_ptr<TestWayPoint> wayPoint;
//
//	for (int i = 0, end = static_cast<int>(mTarget.size()); i < end; i++)
//	{
//		Vector3 dist = goal - trans[i]->GetPosition();
//		distance = dist.Length();
//
//		//vectorには、検索済みのやつしか入らないはず...
//		//if (mTarget[i]->GetUseFlag())
//		{
//			if (mostDistance == 0 || mostDistance > distance)
//			{
//				mostDistance = distance;
//				wayPoint = trans[i];
//			}
//		}
//
//		//ゴールを発見したら
//		if (trans[i]->GetPosition().x == goal.x &&
//			trans[i]->GetPosition().y == goal.y &&
//			trans[i]->GetPosition().z == goal.z)
//		{
//			finishSearchWay = true;
//			wayPoint = trans[i];
//			break;
//		}
//	}
//
//	if (!finishSearchWay)
//	{
//		//ゴールしてないときは使用済みにする
//		wayPoint->SetUseFlag(true);
//	}
//
//	return wayPoint;
//}
//
//void BaseEnemy::ClearFlag()
//{
//
//	for (int i = 0, end = static_cast<int>(mPointList.size()); i < end; i++)
//	{
//		mPointList[i]->SetUseFlag(false);
//	}
//}

void BaseEnemy::WayPointMove()
{
	Vector3 otherPosition = moveList[moveCount];
	moveWayPoint = true;

	//ゴールとの距離が近すぎたら移動しない。
	if (!InsideDistance(otherPosition, 10.0f))
	{
		Move(otherPosition);
	}
	else
	{
		InitWayPoint();
	}

	//到着したとする
	if (InsideDistance(otherPosition, 8.0f))
	{
		if (static_cast<int>(moveList.size()) == moveCount + 1)
		{
			goalFlag = true;
			//InitWayPoint();
		}
		else
		{
			moveCount++;
		}
	}
}

//void BaseEnemy::Report(shared_ptr<ModelRenderer> modelRender)
//{
//	//すでに報告している or 現在移動中 なら表示しない
//	if (isInvincible || moveWayPoint) return;
//
//	isInvincible = true;
//
//	//振り向き機能追加---------------------------------------------
//	DicideTurnAround();
//
//	//死亡処理に移る
//	if (HP <= 0)
//	{
//		mDeathFlag = true;
//		return;
//	}
//
//	//mReportArea = new ReportArea(position, objM, modelRender, number);
//	//mManager->Add(new ReportArea(position, mManager, modelRender, testNumber));
//	//testNumber++;
//}

void BaseEnemy::InitWayPoint()
{
	//ウェイポイント移動していなかったら処理しない
	if (!moveWayPoint) return;

	moveList.clear();

	goalPoint = Vector3().zero;
	resultPoint = Vector3().zero;

	finishSearchWay = false;
	hitReportArea = false;
	moveWayPoint = false;
	goalFlag = false;

	//mTarget.clear();
	//mPointList.clear();

	searchCount = 0;
	loopCount = 0;
	moveCount = 0;
	arrayCount = 0;
}

void BaseEnemy::DicideTurnAround()
{
	hitPos = position;
	hitAngle = angle;

	//索敵状態でない or 振り向き機能がOFF なら処理をしない
	if (actionState != ActionState::SEARCH || !turnaroundMode) return;

	//当たった位置と、現在の自分の向きを一時保存する。
	hitPos = mPlayerPosition;
	hitAngle = angle;
	actionState = ActionState::WARNING;
}

void BaseEnemy::TurnAround(int time)
{
	//振り向き機能がOFF or プレイヤーを追っている or 自爆状態 なら処理をしない
	if (!turnaroundMode || trackingPlayer || isDestruct || trackingBreadcrumb || moveWayPoint) return;

	//①距離を調べる
	Vector3 distance = hitPos - position;
	//②正規化忘れずに
	distance = distance.normal();
	//③二点間の角度を求める
	float radian = atan2(distance.x, distance.z);
	//④回転させる角度 = 当たった場所との角度 - 現在の自分の角度
	float rotValue = (Math::toDegrees(radian) + 180) - hitAngle.y;
	//⑤回転させたい秒数で割る
	float rotSecond = rotValue / time;
	//⑥現在時間をかける
	float rotateTime = rotSecond * invincibleCount;
	//⑦結果 = 現在の自分の角度 + 回転させる角度
	angle.y = hitAngle.y + rotateTime;
	fanInfo.rotate = (-angle.y - 90.0f);
	barrelAngle = (fanInfo.rotate + 90.0f);
}

void BaseEnemy::DestructMode(int hpLine, bool destructMode)
{
	//自爆モードがOFF なら処理しない。
	if (!destructMode) return;

	//体力が一定以下になったら自爆モードになる。
	if (HP <= hpLine)
	{
		actionState = ActionState::DESTRUCT;
	}
}

Vector3 BaseEnemy::AngleToVectorY(float angle)const
{
	Vector3 vector = Vector3(0, 0, 0);

	//角度をラジアン角に戻す
	float radian = Math::toRadians(angle);

	float x = cosf(radian);
	float z = sinf(radian);

	return Vector3(x, 0, z).normal();
}

float BaseEnemy::VectorToAngleY(const Vector3 & vector) const
{
	float angle = atan2f(vector.x, vector.z);

	angle = Math::toDegrees(angle);

	return angle;
}

bool BaseEnemy::InsideDistance(const Vector3 & otherPos, float dist)
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

bool BaseEnemy::IsHitFanToPoint(const FanInfomation & fan, const Vector3 & point, const float radius) const
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
	float rotateRadian = Math::toRadians(fan.rotate);

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
	float fanCos = cosf(Math::toRadians(fan.fanRange / 2.0f));

	//⑩　点が扇の範囲内にあるか比較をする
	if (fanCos > dot)
	{
		//当たっていない
		return false;
	}

	//ここまで到達したら当たっている
	return true;
}

void BaseEnemy::SetBreadCreator(BreadCrumbCreater * breadCreator)
{
	mBreadCreator = breadCreator;
}

int BaseEnemy::GetHP() const
{
	return HP;
}

bool BaseEnemy::GetDeathFlag() const
{
	return mDeathFlag;
}

void BaseEnemy::SetEnemyAi(EnemyAI * enemyAI)
{
	mEnemyAI = enemyAI;
}

void BaseEnemy::SetObjectManager(ObjectManager * manager)
{
	mManager = manager;
}
