#include "ItemHolder.h"

//初期化
void ItemHolder::Init()
{
	count + 0;
	if (!items.empty())
	{
		items.clear();
	}
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
	if (items.count(item) <= 0)
	{
		return;
	}
	count = items.count(item) - 1;

	items.insert_or_assign(item, count);
}

//アイテム取った時
void ItemHolder::AddItem(ItemNames item)
{
	count = items.count(item)+1;

	items.insert_or_assign( item, count );
}