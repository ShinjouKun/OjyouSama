#include "ObjectManager.h"
#include"../Collision/CollisonManager.h"

ObjectManager::ObjectManager()
{
	IdCount = 0;
}

ObjectManager::~ObjectManager()
{
	Claer();
}

void ObjectManager::Claer()
{
	IdCount = 0;
	for (auto addObj:addObjectList)
	{
		if (addObj != nullptr)
			delete addObj;
	}
	addObjectList.clear();
	for (auto obj : objectList)
	{
		if (obj != nullptr)
			delete obj;
	}
	objectList.clear();
}

void ObjectManager::Add(BaseObject * addObj)
{
		IdCount++;
		addObj->SetID(IdCount);
	
	addObjectList.emplace_back(addObj);//追加
}

void ObjectManager::Update()
{
	AddListUpdate();
	ObjectManagerUpdate();
	RemoveListUpdate();
}

void ObjectManager::AddListUpdate()
{
	for (auto addObj : addObjectList)
	{
		//オブジェクトリストに移して初期化処理を実行
		objectList.emplace_back(addObj);
		addObj->Init();
	}
	addObjectList.clear();
}

void ObjectManager::ObjectManagerUpdate()//主に当たり判定など
{
	
	for (auto obj1:objectList)
	{
		if (obj1 == nullptr || obj1->GetDeath())continue;
			obj1->Update();
			obj1->CollisonUpdate();
	}
	CollisonManager::GetInstance()->CheckAllCollisons();//一括更新
}

void ObjectManager::RemoveListUpdate()
{
	auto itr = objectList.begin();
	while (itr != objectList.end())
	{
		//nullチェック
		if ((*itr) == nullptr)
		{
			itr = objectList.erase(itr);
		}
		else if((*itr)->GetDeath())
		{
			BaseObject* deleteObj = (*itr);
			itr = objectList.erase(itr);
			delete deleteObj;
		}
		else
		{
			itr++;
		}
	}
	
}

void ObjectManager::Draw()
{
	for (auto obj : objectList)
	{
		if (obj == nullptr)continue;
		if (!obj->GetDeath())
			obj->Rend();
		else
		{
			continue;
		}
	}
}

//void ObjectManager::DropBreadCrumb(int status)
//{
//	//キー入力で落とす
//	if (status == 1)
//	{
//		if (Input::KeyDown(DIK_P))
//		{
//			//新しい欠片を落とす。
//			objM->Add(new BreadCrumb(BaseObject::getp, objM, playerModel, breadNumber));
//			//落としたら番号を1つ進める
//			breadNumber++;
//		}
//	}
//	//距離で落とす
//	else if (status == 2)
//	{
//		//パンくずを落とす。
//		//移動中でなければ落とさない。
//		if (!moveFlag) return;
//
//		//最初に欠片を落とす
//		//リストの中身が空だったら1つ落とすみたいな
//		if (measureMap.empty())
//		{
//			//新しい欠片を落とす。
//			objM->Add(new BreadCrumb(position, objM, playerModel, breadNumber));
//			//落としたらリストに代入
//			measureMap[breadNumber] = position;
//			breadNumber++;
//		}
//
//		//前に落とした欠片の位置。
//		//リストから持ってくる必要あり。
//		//現在の番号を数えて、その番号-1の値を持ってくるみたいな
//		Vector3 previousBread = measureMap[breadNumber - 1];
//
//		//前の欠片との距離を計算。
//		float distance = (previousBread - position).Length();
//
//		//指定の数値
//		float value = 10.0f;
//
//		//前に落とした欠片と、現在の位置が指定の数値以上離れたら
//		if (distance > value)
//		{
//			//新しい欠片を落とす。
//			objM->Add(new BreadCrumb(position, objM, playerModel, breadNumber));
//			//落としたらリストに代入
//			measureMap[breadNumber] = position;
//			breadNumber++;
//		}
//	}
//	//時間で落とす
//	else if (status == 3)
//	{
//		intervalCount++;
//
//		if (intervalCount > intervalTime * 60)
//		{
//			intervalCount = 0;
//
//			//新しい欠片を落とす。
//			objM->Add(new BreadCrumb(position, objM, playerModel, breadNumber));
//			breadNumber++;
//		}
//	}
//}

std::vector<BaseObject*> ObjectManager::getUseList()
{
	return objectList;
}

BaseObject & ObjectManager::GetPlayer() const
{
	BaseObject* player = nullptr;

	for (auto& type : objectList)
	{
		if (type->GetType() == ObjectType::PLAYER)
		{
			player = type;
		}
	}

	return *player;
}


BaseObject & ObjectManager::GetGolem() const
{
	BaseObject* boss = nullptr;

	for (auto& type : objectList)
	{
		if (type->GetType() == ObjectType::BOSS)
		{
			boss = type;
		}
	}

	return *boss;
}

BaseObject & ObjectManager::GetEnemy() const
{
	BaseObject* enemy = nullptr;

	for (auto& type : objectList)
	{
		if (type->GetType() == ObjectType::ENEMY)
		{
			enemy = type;
		}
	}

	return *enemy;
}

vector<BaseObject*> ObjectManager::GetEnemyList() const
{
	std::vector<BaseObject*> eL;
	for (int i = 0;i < objectList.size();i++)
	{
		if (objectList[i]->GetType()== ObjectType::ENEMY)
		{
			eL.push_back(objectList[i]);
		}
	}
	return eL;
}


BaseObject & ObjectManager::GetCastle() const
{
	BaseObject* castle = nullptr;

	for (auto& type : objectList)
	{
		if (type->GetType() == ObjectType::DEFENCEPOINT)
		{
			castle = type;
		}
	}

	return *castle;
}

