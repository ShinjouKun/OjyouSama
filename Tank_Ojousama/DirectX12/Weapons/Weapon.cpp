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
		objType = ObjectType::BULLET;//�v���C���[�̒e	
		break;
	case ENEMY:
	case BOSS:
		objType = ObjectType::ENEMYBULLET;//�G�̒e
		break;
	default:
		break;
	}
}
