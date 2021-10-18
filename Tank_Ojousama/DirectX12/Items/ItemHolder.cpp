#include "ItemHolder.h"

//������
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

//���X�g�����
void ItemHolder::Reset()
{
	items.clear();
}

//�g����
void ItemHolder::UseItem(ItemNames item)
{
	if (items.count(item) <= 0)
	{
		return;
	}
	count = items.count(item) - 1;

	items.insert_or_assign(item, count);
}

//�A�C�e���������
void ItemHolder::AddItem(ItemNames item)
{
	count = items.count(item)+1;

	items.insert_or_assign( item, count );
}