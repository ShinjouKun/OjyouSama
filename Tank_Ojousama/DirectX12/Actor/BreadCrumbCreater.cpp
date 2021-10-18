#include "BreadCrumbCreater.h"

#include "BreadCrumb.h"
#include "TestBreadCrumb.h"

BreadCrumbCreater::BreadCrumbCreater(
	ObjectManager* objM,
	shared_ptr<ModelRenderer> mRender
)
{
	objManager = objM;
	modelRender = mRender;
	Initialize();
}

BreadCrumbCreater::~BreadCrumbCreater()
{
}

void BreadCrumbCreater::DropBreadCrumb()
{
	intervalCount++;

	if (intervalCount > intervalTime)
	{
		intervalCount = 0;
		//プレイヤーの位置生成するタイミングだけ取得
		playerPosition = objManager->GetPlayer().GetPosition();

		////新しい欠片を落とす
		//objManager->Add(new BreadCrumb(playerPosition, objManager, modelRender, breadNumber));


		std::shared_ptr<TestBreadCrumb> mBread = std::make_shared<TestBreadCrumb>(playerPosition, breadNumber);
		mBreadList.push_back(mBread);

		breadNumber++;

		ImGui::Text("------------------");
	}

	auto itr = mBreadList.begin();

	while (itr != mBreadList.end())
	{
		(*itr)->DeathInterval();

		if ((*itr)->GetDeathFlag())
		{
			//リストから削除
			itr =  mBreadList.erase(itr);
			
		}
		else
		{
			++itr;
		}
	}
}

std::vector<std::shared_ptr<TestBreadCrumb>> BreadCrumbCreater::GetBreadList() const
{
	return mBreadList;
}

void BreadCrumbCreater::Initialize()
{
	playerPosition = Vector3().zero;
	measureMap.clear();
	breadNumber = 0;
	intervalCount = 0;
	intervalTime = 1 * 60;
	mBreadList.clear();
}
