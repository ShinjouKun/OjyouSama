#include "CollisonManager.h"
#include"BaseCollider.h"
#include"Collision.h"
//#include"Vector3.h"
#include "../Math/Vector3.h"
CollisonManager * CollisonManager::GetInstance()
{
	static CollisonManager instance;
	return &instance;
}

void CollisonManager::CheckAllCollisons()
{
	forward_list<BaseCollider*>::iterator objectA;
	forward_list<BaseCollider*>::iterator objectB;

	//�S�Ă̑g�ݍ��킹�ő������������
	objectA = colliders.begin();
	for (;objectA != colliders.end();++objectA)
	{
		objectB = objectA;
		++objectB;
		for (;objectB != colliders.end();++objectB)
		{
			BaseCollider* colA = *objectA;
			BaseCollider* colB = *objectB;

			//�ǂ�������̏ꍇ
			if (colA->GetCollType() == SPHERE_COLLISON &&
				colB->GetCollType() == SPHERE_COLLISON&&colA != colB)
			{
				Sphere* spherA = dynamic_cast<Sphere*>(colA);
				Sphere* spherB = dynamic_cast<Sphere*>(colB);
				Vector3 inter;
				if (Collision::StoSColl(*spherA, *spherB, &inter))
				{
					colA->OnCollison(CollisonInfo(colB->GetObject(), colB, inter));
					colB->OnCollison(CollisonInfo(colA->GetObject(), colA, inter));
				}

			}
		}
	}
}
