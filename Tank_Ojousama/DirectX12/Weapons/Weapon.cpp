#include "Weapon.h"

Weapon::Weapon()
{
}

Weapon::~Weapon()
{
}

void Weapon::SetBulletType()
{
	switch (setType)
	{
	case PLAYER:
		objType = ObjectType::BULLET;//ÉvÉåÉCÉÑÅ[ÇÃíe	
		break;
	case ENEMY:
	case BOSS:
		objType = ObjectType::ENEMYBULLET;//ìGÇÃíe
		break;
	default:
		break;
	}
}
