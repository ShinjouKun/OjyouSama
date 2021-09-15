#pragma once
#include "../Actor/ObjectManager.h"
#include "../Actor/BaseObject.h"
#include "../Render/ModelRenderer.h"

//�A�C�e���̃x�[�X�I�u�W�F�N�g

class Item :public BaseObject
{
public:
	Item(Vector3 pos, Vector3 ang, ObjectManager * obj, shared_ptr<ModelRenderer> m, int aliv, int n);
	~Item();

private:

	virtual void Init() override;
	virtual void Update() override;
	virtual void Rend() override;

	ObjectManager* objM;
	shared_ptr<ModelRenderer>itemModel;
	//�A�C�e����ޕ���
	int number = 0;
	string name;
	string num;
	string numName;

	//�������Ԏw��p
	int alive;
	virtual void OnCollison(BaseCollider* info) override;

	virtual void ImGuiDebug() override;
};