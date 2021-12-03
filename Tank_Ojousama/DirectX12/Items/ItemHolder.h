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
	void UseItem();
	void AddItem(ItemNames item);
	void UseUlt();
	void AddUlt(ItemNames item);

	void SetUseFlag(bool value);
	bool GetUseFlag() { return useItems; };

private:

	void Init();
	void Update();
	void Reset();
	

private:
	//ÉAÉCÉeÉÄÇÃï€éùêî
	unordered_map<ItemNames, int> items;
	unordered_map<ItemNames, int> ults;
	int count;
	bool useItems;
	ItemNames itemName;
	ItemNames ultName;
};