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
	
	addObjectList.emplace_back(addObj);//�ǉ�
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
		//�I�u�W�F�N�g���X�g�Ɉڂ��ď��������������s
		objectList.emplace_back(addObj);
		addObj->Init();
	}
	addObjectList.clear();
}

void ObjectManager::ObjectManagerUpdate()//��ɓ����蔻��Ȃ�
{
	
	for (auto obj1:objectList)
	{
		if (obj1 == nullptr || obj1->GetDeath())continue;
			obj1->Update();
			obj1->CollisonUpdate();
	}
	CollisonManager::GetInstance()->CheckAllCollisons();//�ꊇ�X�V
}

void ObjectManager::RemoveListUpdate()
{
	auto itr = objectList.begin();
	while (itr != objectList.end())
	{
		//null�`�F�b�N
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
//	//�L�[���͂ŗ��Ƃ�
//	if (status == 1)
//	{
//		if (Input::KeyDown(DIK_P))
//		{
//			//�V�������Ђ𗎂Ƃ��B
//			objM->Add(new BreadCrumb(BaseObject::getp, objM, playerModel, breadNumber));
//			//���Ƃ�����ԍ���1�i�߂�
//			breadNumber++;
//		}
//	}
//	//�����ŗ��Ƃ�
//	else if (status == 2)
//	{
//		//�p�������𗎂Ƃ��B
//		//�ړ����łȂ���Η��Ƃ��Ȃ��B
//		if (!moveFlag) return;
//
//		//�ŏ��Ɍ��Ђ𗎂Ƃ�
//		//���X�g�̒��g���󂾂�����1���Ƃ��݂�����
//		if (measureMap.empty())
//		{
//			//�V�������Ђ𗎂Ƃ��B
//			objM->Add(new BreadCrumb(position, objM, playerModel, breadNumber));
//			//���Ƃ����烊�X�g�ɑ��
//			measureMap[breadNumber] = position;
//			breadNumber++;
//		}
//
//		//�O�ɗ��Ƃ������Ђ̈ʒu�B
//		//���X�g���玝���Ă���K�v����B
//		//���݂̔ԍ��𐔂��āA���̔ԍ�-1�̒l�������Ă���݂�����
//		Vector3 previousBread = measureMap[breadNumber - 1];
//
//		//�O�̌��ЂƂ̋������v�Z�B
//		float distance = (previousBread - position).Length();
//
//		//�w��̐��l
//		float value = 10.0f;
//
//		//�O�ɗ��Ƃ������ЂƁA���݂̈ʒu���w��̐��l�ȏ㗣�ꂽ��
//		if (distance > value)
//		{
//			//�V�������Ђ𗎂Ƃ��B
//			objM->Add(new BreadCrumb(position, objM, playerModel, breadNumber));
//			//���Ƃ����烊�X�g�ɑ��
//			measureMap[breadNumber] = position;
//			breadNumber++;
//		}
//	}
//	//���Ԃŗ��Ƃ�
//	else if (status == 3)
//	{
//		intervalCount++;
//
//		if (intervalCount > intervalTime * 60)
//		{
//			intervalCount = 0;
//
//			//�V�������Ђ𗎂Ƃ��B
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

