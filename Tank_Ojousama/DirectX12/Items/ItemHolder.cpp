#include "ItemHolder.h"

ItemHolder::ItemHolder()
{
}

ItemHolder::~ItemHolder()
{
}

ItemHolder * ItemHolder::GetInstance()
{
	static ItemHolder instance;
	return &instance;
}

//初期化
void ItemHolder::Init()
{
	count = 0;
	useRepair = false;
	useSmoke = false;
	useShield = false;
}

void ItemHolder::Update()
{
}

//リストを空に
void ItemHolder::Reset()
{
	items.clear();
}

//使う時
void ItemHolder::UseItem(ItemNames item)
{
	
	count = items[item]-1;

	if (count < 0)
	{
		return;
	}
	
	
	items.insert_or_assign(item, count);

	if (item == ItemNames::heal)
	{
		SetUseRepair(true);
	}
	if (item == ItemNames::dome)
	{
		SetUseShield(true);
	}
	if (item == ItemNames::smock)
	{
		SetUseSmoke(true);
	}
}

//アイテム取った時
void ItemHolder::AddItem(ItemNames item)
{
	count = items.count(item)+1;

	items.insert_or_assign( item, count );
}

void ItemHolder::UseUlt(ItemNames item)
{
	count = ults[item] - 1;

	if (count < 0)
	{
		return;
	}


	ults.insert_or_assign(item, count);

	if (item == ItemNames::heal)
	{
		SetUseRepair(true);
	}
	if (item == ItemNames::dome)
	{
		SetUseShield(true);
	}
	if (item == ItemNames::smock)
	{
		SetUseSmoke(true);
	}
}

void ItemHolder::AddUlt(ItemNames item)
{
	count = ults.count(item) + 1;

	ults.insert_or_assign(item, count);
}

void ItemHolder::SetUseRepair(bool value)
{
	useRepair = value;
}

void ItemHolder::SetUseSmoke(bool value)
{
	useSmoke = value;
}

void ItemHolder::SetUseShield(bool value)
{
	useShield = value;
}

int ItemHolder::GetItemNum(ItemNames item)
{
	return items[item];
}

void ItemHolder::SetDamege(int value)
{
	damege = value;
}
