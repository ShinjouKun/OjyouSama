#pragma once
#include<memory>
#include <list>
#include "../ObjectManager.h"
#include "../../Render/TexRenderer.h"

class SniperEnemy;
class BlowEnemy;
class BirdEnemy;
class CEnemy;
class MortarEnemy;

enum EnemyName
{
	Sniper,
	Blow,
	Captain,
	Bird,
	Mortar,
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

	EnemyCreator(ObjectManager * objManager, std::shared_ptr<TexRenderer> texRender);

	~EnemyCreator();

	//“Gî•ñ‚ğ’Ç‰Á
	void Add(const EnemyData& data);


	void Create();

private:


	ObjectManager * mManager;
	std::shared_ptr<TexRenderer> mTexRender;

	std::list<EnemyData> mEnemyVector;
};