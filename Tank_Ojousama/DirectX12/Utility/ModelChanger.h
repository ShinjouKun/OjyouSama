#pragma once
#include <vector>
#include <string>
#include "TextEditer.h"
#include "../Render/ModelRenderer.h"

using namespace std;

enum HeadState
{
	Normal,
	Other01,
	Other02,
};

enum BodyState
{
	Light,
	Midium,
	Heavy,
};

enum BottomState
{
	Light_b,
	Midium_b,
	Heavy_b,
};

enum WeaponsState
{
	Cannon,
	MachinGun,
	ShotGun,
	Mine,
};

class ModelChanger
{
public:
	ModelChanger();
	~ModelChanger();

	void Init();
	void Load(shared_ptr<ModelRenderer> playerModel);
	void Save();

	void ChangeHead(HeadState headState);
	void ChangeBody(BodyState bodyState);
	void ChangeBottom(BottomState bottomState);
	void ChangeWeapons1(WeaponsState weaponState);


	HeadState GetHeadState() { return head; }
	BodyState GetBodyState() { return body; }
	BottomState GetBottomState() { return bottom; }
	WeaponsState GetWeaponState1() { return weapons1; }

	void SetHP(int value);//�̗�
	int GetHP() { return hp; }
	void SetUpDamage(int up);
	int GetUpDamage() { return upDamage; }
	void SetSpeed(float value);//���x
	float GetSpeed() { return speed; }
	int GetWeaponNum() { return Weapon; }
	void SetWeaponNum(int num);
	string GetModelName(int num);//0�`2����l,3���(�C���j,4���(�ԑ́j
	void SetBuysNum(int buy);
	int GetBuysNum(int buy)const;
	int hp;//HP
	int upDamage;//�U����
	float speed;
	int Weapon;
private:
	std::vector<string> b;
	std::vector<int> buys;//�w�����X�g
	std::vector<std::string> state;
	std::vector<std::string> modelKey;
	HeadState head;
	BodyState body;
	BottomState bottom;
	WeaponsState weapons1;
	TextEditor* editor;
};
