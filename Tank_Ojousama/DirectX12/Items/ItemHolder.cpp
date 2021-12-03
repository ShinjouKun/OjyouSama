#include "ItemHolder.h"

ItemHolder::ItemHolder()
{
}

ItemHolder::~ItemHolder()
{
}

//������
void ItemHolder::Init()
{
	count = 0;
	useItems = false;
}

void ItemHolder::Update()
{
}

//���X�g�����
void ItemHolder::Reset()
{
	items.clear();
}

//�g����
void ItemHolder::UseItem()
{
	
	count = items[itemName]-1;

	if (count < 0)
	{
		return;
	}
	
	
	items.insert_or_assign(itemName, count);

	SetUseFlag(true);
}

//�A�C�e���������
void ItemHolder::AddItem(ItemNames item)
{
	count = items.count(item)+1;
	itemName = item;

	items.insert_or_assign( item, count );
}

void ItemHolder::UseUlt()
{
	count = ults[ultName] - 1;

	if (count < 0)
	{
		return;
	}


	ults.insert_or_assign(ultName, count);

	SetUseFlag(true);
}

void ItemHolder::AddUlt(ItemNames item)
{
	count = ults.count(item) + 1;
	ultName = item;

	ults.insert_or_assign(ultName, count);
}

void ItemHolder::SetUseFlag(bool value)
{
	useItems = value;
}
