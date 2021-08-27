#pragma once
#include <memory>
#include "ObjectManager.h"
#include "BaseObject.h"
#include "../Render/ModelRenderer.h"

class BreadCrumb :public BaseObject
{
public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="pos">�ʒu</param>
	/// <param name="objManager">�I�u�W�F�N�g�Ǘ���</param>
	/// <param name="modelRender">���f���`��</param>
	/// <param name="num">���ʔԍ�</param>
	BreadCrumb(
		Vector3 pos,
		ObjectManager * objManager,
		shared_ptr<ModelRenderer> modelRender,
		int num
	);

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	~BreadCrumb();

	int GetNumber()
	{
		return keyNumber;
	}

	void SetPosition(Vector3 pos)
	{
		position = pos;
	}

private:
	virtual void Init() override;
	virtual void Update() override;
	virtual void Rend() override;
	virtual void OnCollison(BaseCollider* info) override;
	virtual void ImGuiDebug() override;

private:
	ObjectManager* objectManager;
	shared_ptr<ModelRenderer> modelRender;
	ObjectType objectType;
	Vector3 scale;

	int lifeCount;//�����J�E���g
	int lifeTime; //��������

	float radius;//���a

	//�����o���悤
	int keyNumber = 0;
	string key;    //���ʔԍ�
	string name;   //���f����
	string keyName;//���f����+�ԍ�

};
