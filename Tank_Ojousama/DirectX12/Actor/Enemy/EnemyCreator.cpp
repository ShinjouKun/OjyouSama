#include "EnemyCreator.h"
#include "SniperEnemy.h"
#include "BlowEnemy.h"

EnemyCreator::EnemyCreator(ObjectManager * objManager)
	:mManager(objManager)
{
	mEnemyVector.clear();
}

EnemyCreator::~EnemyCreator()
{
	mEnemyVector.clear();
}

void EnemyCreator::Add(const EnemyData & data)
{
	mEnemyVector.emplace_back(data);
}

void EnemyCreator::Create()
{
	if (mEnemyVector.empty()) return;

	auto itr = mEnemyVector.begin();

	switch ((*itr).name)
	{
	case EnemyName::Sniper:
		mManager->Add(new SniperEnemy((*itr).position, (*itr).angle, (*itr).number));
		break;
	case EnemyName::Blow:
		mManager->Add(new BlowEnemy((*itr).position, (*itr).angle, (*itr).number));
		break;

	default:
		break;
	}

	//‚±‚ê‚Åæ“ª‚ªÁ‚³‚ê‚Ü‚·
	mEnemyVector.pop_front();
}
