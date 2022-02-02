#include "EnemyCreator.h"
#include "SniperEnemy.h"
#include "BlowEnemy.h"
#include "BirdEnemy.h"
#include "CEnemy.h"
#include "MortarEnemy.h"

EnemyCreator::EnemyCreator(ObjectManager * objManager, std::shared_ptr<TexRenderer> texRender)
	:mManager(objManager),mTexRender(texRender)
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

	/*	Captain,
	Bird,
	Mortar,*/

	switch ((*itr).name)
	{
	case EnemyName::Sniper:
		mManager->Add(new SniperEnemy((*itr).position, (*itr).angle, (*itr).number));
		break;
	case EnemyName::Blow:
		mManager->Add(new BlowEnemy((*itr).position, (*itr).angle, (*itr).number));
		break;
	case EnemyName::Captain:
		mManager->Add(new CEnemy((*itr).position, (*itr).angle, (*itr).number));
		break;
	case EnemyName::Bird:
		mManager->Add(new BirdEnemy((*itr).position, (*itr).angle,mTexRender, (*itr).number));
		break;
	case EnemyName::Mortar:
		mManager->Add(new MortarEnemy((*itr).position, (*itr).angle, (*itr).number));
		break;
	default:
		break;
	}

	//‚±‚ê‚Åæ“ª‚ªÁ‚³‚ê‚Ü‚·
	mEnemyVector.pop_front();
}
