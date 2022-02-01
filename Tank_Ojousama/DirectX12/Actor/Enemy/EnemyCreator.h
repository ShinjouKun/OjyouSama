#pragma once
#include<memory>
#include <list>
#include "../ObjectManager.h"

class SniperEnemy;
class BlowEnemy;

enum EnemyName
{
	Sniper,
	Blow,
};

struct EnemyData
{
	EnemyName name;
	Vector3 position;
	Vector3 angle;
	int number;
};

class EnemyCreator
{
public:

	EnemyCreator(ObjectManager * objManager);

	~EnemyCreator();

	//“Gî•ñ‚ğ’Ç‰Á
	void Add(const EnemyData& data);


	void Create();

private:


	ObjectManager * mManager;

	std::list<EnemyData> mEnemyVector;
};