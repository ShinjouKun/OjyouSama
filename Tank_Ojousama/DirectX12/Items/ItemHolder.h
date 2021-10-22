#pragma once
#include <unordered_map>
#include "Item.h"

using namespace std;

class ItemHolder
{
public:

	void Init();
	void Update();
	void Reset();
	void UseItem(ItemNames item);
	void AddItem(ItemNames item);

	void SetUseFlag(bool value);
	bool GetUseFlag() { return useItems; };

private:
	//�A�C�e���̕ێ���
	unordered_map<ItemNames, int> items;
	int count;
	bool useItems;
};