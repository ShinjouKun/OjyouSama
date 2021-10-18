#pragma once
#include "ObjectManager.h"
#include "BaseObject.h"
#include "../Render/ModelRenderer.h"
#include "../Collision/SpherCollider.h"

class ReportArea : public BaseObject
{
public:

	ReportArea(
		const Vector3 & pos,
		ObjectManager* objectManager,
		shared_ptr<ModelRenderer> modelRenderer,
		int num
	);

	~ReportArea();

	//�\����Ԃ̐؂�ւ�
	void SetActice(bool value);

	//���낷
	void SetDeath(bool value);

private:
	virtual void Init() override;
	virtual void Update() override;
	virtual void Rend() override;
	virtual void OnCollison(BaseCollider* info) override;
	virtual void ImGuiDebug() override;

private:
	ObjectManager* objManager;
	shared_ptr<ModelRenderer> modelRender;

	Vector3 scale;
	Vector3 offsetScale;

	SphereCollider* spehereCollider;

	string name;   //���f����
	string key;	   //���ʔԍ�
	string keyname;//���O�Ɣԍ��̍���

	float radius;//���a
	float reportRange;//�񍐔͈�

	bool isActive;//�\�����

};