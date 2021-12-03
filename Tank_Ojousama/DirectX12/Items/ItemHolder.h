#pragma once
#include <unordered_map>
#include "Item.h"

using namespace std;

class ItemHolder
{
public:
	ItemHolder();
	~ItemHolder();

	void Init();
	void Update();
	void Reset();
	void UseItem();
	void AddItem(ItemNames item);
	void UseUlt();
	void AddUlt(ItemNames item);

	void SetUseFlag(bool value);
	bool GetUseFlag() { return useItems; };

private:
	//ƒAƒCƒeƒ€‚Ì•Û”
	unordered_map<ItemNames, int> items;
	unordered_map<ItemNames, int> ults;
	int count;
	bool useItems;
	ItemNames itemName;
	ItemNames ultName;
};