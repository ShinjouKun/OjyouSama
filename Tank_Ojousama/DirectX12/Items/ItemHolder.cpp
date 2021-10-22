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
void ItemHolder::UseItem(ItemNames item)
{
	
	count = items.count(item) - 1;

	if (count <= 0)
	{
		return;
	}
	items.insert_or_assign(item, count);

	SetUseFlag(true);
}

//�A�C�e���������
void ItemHolder::AddItem(ItemNames item)
{
	count = items.count(item)+1;

	items.insert_or_assign( item, count );
}

void ItemHolder::SetUseFlag(bool value)
{
	useItems = value;
}
