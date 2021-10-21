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

//�A�C�e���̃x�[�X�I�u�W�F�N�g

class Item :public BaseObject
{
public:
	Item();
	~Item();

private:

	void SetActive(bool value);

	virtual ItemNames GetItemNames() { return itemName; }

	ObjectManager* objM;
	shared_ptr<ModelRenderer>itemModel;
	//�A�C�e����ޕ���+�����o���p
	int number = 0;
	string name;
	string num;
	string numName;
	ItemNames itemName;//�A�C�e����ޔ��ʗp
	//�A�C�e���̎g�p�t���O
	bool active;
	//�A�C�e���̎擾
	bool isGet;
	//�������Ԏw��p
	int alive;

	//BaseObject ����Čp��
	virtual void Init() = 0;
	virtual void Update() = 0;
	virtual void Rend() = 0;
	virtual void ImGuiDebug() = 0;
	virtual void OnCollison(BaseCollider * col) = 0;
};