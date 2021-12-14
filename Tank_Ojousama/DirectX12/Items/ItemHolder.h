#pragma once
#include <unordered_map>
#include "Item.h"

using namespace std;

class ItemHolder
{
public:
	ItemHolder();
	~ItemHolder();

	static ItemHolder* GetInstance();
	void UseItem(ItemNames item);
	void AddItem(ItemNames item);
	void UseUlt(ItemNames ult);
	void AddUlt(ItemNames item);

	void SetUseRepair(bool value);
	void SetUseSmoke(bool value);
	void SetUseShield(bool value);
	bool GetUseRepair() { return useRepair; };
	bool GetUseSmoke() { return useSmoke; };
	bool GetUseShield() { return useShield; };
	int GetItemNum(ItemNames item);
	void SetDamege(int value);
	int GetDamege() { return damege; }

private:

	void Init();
	void Update();
	void Reset();
	

private:
	//ÉAÉCÉeÉÄÇÃï€éùêî
	unordered_map<ItemNames, int> items;
	unordered_map<ItemNames, int> ults;
	int count;
	int damege;
	bool useRepair;
	bool useSmoke;
	bool useShield;
};