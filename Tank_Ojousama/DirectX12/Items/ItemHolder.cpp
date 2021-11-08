#include "ItemHolder.h"

ItemHolder::ItemHolder()
{
}

ItemHolder::~ItemHolder()
{
}

//初期化
void ItemHolder::Init()
{
	count = 0;
	useItems = false;
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

	SetUseFlag(true);
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

	SetUseFlag(true);
}

void ItemHolder::AddUlt(ItemNames item)
{
	count = ults.count(item) + 1;

	ults.insert_or_assign(item, count);
}

void ItemHolder::SetUseFlag(bool value)
{
	useItems = value;
}
