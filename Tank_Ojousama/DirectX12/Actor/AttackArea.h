#pragma once
#include "ObjectManager.h"
#include "BaseObject.h"
#include "../Render/ModelRenderer.h"

class AttackArea : public BaseObject
{
public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="pos">�ʒu</param>
	/// <param name="ang">�p�x</param>
	/// <param name="objM">�I�u�W�F�N�g�Ǘ���</param>
	/// <param name="modelR">���f���`��</param>
	/// <param name="num">���ʔԍ�</param>
	AttackArea(
		Vector3 pos,
		Vector3 ang,
		ObjectManager* objM,
		shared_ptr<ModelRenderer>modelR,
		int num
	);

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	~AttackArea();

	void SetPosition(Vector3 pos);

	void SetActive(bool value,Vector3 pos = Vector3(0,0,0),Vector3 ang = Vector3(0, 0, 0));

	bool GetActive();

	void SetDeath(bool value);

private:
	virtual void Init() override;
	virtual void Update() override;
	virtual void Rend() override;
	virtual void OnCollison(const CollisonInfo & info) override;
	virtual void ImGuiDebug() override;

private:
	ObjectManager* objManager;
	shared_ptr<ModelRenderer> modelRender;
	Vector3 scale;

	int number = 0;

	float radius;//���a

	bool isActive;//�\�����

	string name;   //���f����
	string key;	   //���ʔԍ�
	string keyname;//���O�Ɣԍ��̍���
};