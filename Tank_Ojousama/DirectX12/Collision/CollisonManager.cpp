#include "CollisonManager.h"
#include"BaseCollider.h"
#include"Collision.h"
#include "../Math/Vector3.h"
#include"../Collision/OctTreeManager.h"
bool CollisonManager::OctTreeSet()
{
	//‹óŠÔì¬
	S8Tree = new OctTreeManager();
	if (!S8Tree->Init(
		2,//ŠK‘wƒŒƒxƒ‹
		Vector3(-500, 0, -1000),
		Vector3(1000, 10, 1000)))
	{
		return 0;
	}
}
CollisonManager * CollisonManager::GetInstance()
{
	static CollisonManager instance;
	return &instance;
}

OctTreeManager * CollisonManager::GetInstanceOct()
{
	return S8Tree;
}

void CollisonManager::CheckAllCollisons()
{
	vector<BaseCollider*> List;
	S8Tree->GetAllCollisonList(List);

	for (int i = 0; i < List.size(); i += 2)
	{
		BaseCollider* colA = List[i];
		BaseCollider* colB = List[i + 1];

		//‚Ç‚¿‚ç‚à‹…‚Ìê‡
		if (colA->GetCollType() == SPHERE_COLLISON &&
			colB->GetCollType() == SPHERE_COLLISON &&
			colA != colB)
		{
			Sphere* spherA = dynamic_cast<Sphere*>(colA);
			Sphere* spherB = dynamic_cast<Sphere*>(colB);
			Vector3 inter;

			if (Collision::StoSColl(*spherA, *spherB, &inter))
			{
				colA->OnCollison(colB);
				colB->OnCollison(colA);
			}
			else if (colA->GetColObject()->GetActive() && colB->GetColObject()->GetActive())
			{
				colA->GetColObject()->SetActive(false);
				colB->GetColObject()->SetActive(false);
			}
		}
		else if (colA->GetCollType() == AABB_COLLISON &&
			colB->GetCollType() == AABB_COLLISON &&
			colA != colB)
		{
			AABB* aabbA = dynamic_cast<AABB*>(colA);
			AABB* aabbB = dynamic_cast<AABB*>(colB);


			if (Collision::AABBColl(*aabbA, *aabbB))
			{
				colA->OnCollison(colB);
				colB->OnCollison(colA);
			}
			else if (colA->GetColObject()->GetActive() && colB->GetColObject()->GetActive())
			{
				colA->GetColObject()->SetActive(false);
				colB->GetColObject()->SetActive(false);
			}
		}
		else if (colA->GetCollType() == SPHERE_COLLISON &&
			colB->GetCollType() == AABB_COLLISON &&
			colA != colB)
		{
			Sphere* spher = dynamic_cast<Sphere*>(colA);
			AABB* aabb = dynamic_cast<AABB*>(colB);


			if (Collision::StoAABBCol(*spher, *aabb))
			{
				colA->OnCollison(colB);
				colB->OnCollison(colA);
			}
			else if (colA->GetColObject()->GetActive() && colB->GetColObject()->GetActive())
			{
				colA->GetColObject()->SetActive(false);
				colB->GetColObject()->SetActive(false);
			}
		}

	}

}
