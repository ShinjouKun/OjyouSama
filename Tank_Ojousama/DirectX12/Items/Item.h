#pragma once
#include "../Actor/ObjectManager.h"
#include "../Actor/BaseObject.h"
#include "../Render/ModelRenderer.h"

enum ItemNames//�A�C�e�����ʗp
{
	heal,
	dome,
	smock,
};

enum ItemState
{
	Normal,
	High,
};

//�A�C�e���̃x�[�X�I�u�W�F�N�g

class Item :public BaseObject
{
public:
	Item();
	~Item();

	virtual void SetActive(bool value);

	virtual void SetIsGet(bool value);
	
	ItemNames GetItemNames() { return itemName; }
	
	ItemState GetItemState() { return itemState; }

	//�A�C�e���̎g�p�t���O
	bool active;
	//�A�C�e���̎擾
	bool isGet;

protected:

	ObjectManager* objM;
	shared_ptr<ModelRenderer>itemModel;
	//�A�C�e����ޕ���+�����o���p
	int number = 0;
	string name;
	string num;
	string numName;
	ItemNames itemName;//�A�C�e����ޔ��ʗp
	ItemState itemState;//�A�C�e���̃E���g���ǂ����̔���
	//�������Ԏw��p
	int alive;

	//BaseObject ����Čp��
	virtual void Init() = 0;
	virtual void Update() = 0;
	virtual void Rend() = 0;
	virtual void ImGuiDebug() = 0;
	virtual void OnCollison(BaseCollider * col) = 0;
};