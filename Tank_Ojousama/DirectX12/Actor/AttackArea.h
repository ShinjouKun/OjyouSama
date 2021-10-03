#pragma once
#include "ObjectManager.h"
#include "BaseObject.h"
#include "../Render/ModelRenderer.h"
#include "../Collision/SpherCollider.h"

//�G�̍U������N���X(�p�[�e�B�N���o����悤�ɂ�����)
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

	/// <summary>
	/// ���S����܂ł̃J�E���g�_�E��
	/// </summary>
	void DeathCountDown();

	/// <summary>
	/// ���S����܂ł̃J�E���g�_�E����ݒ�
	/// </summary>
	/// <param name="value">���S�����邩</param>
	/// <param name="time">���S����܂ł̎���</param>
	void SetDestroy(bool value ,int time);

	/// <summary>
	/// �\����Ԃ̕ύX(������)
	/// </summary>
	/// <param name="value">�\�����</param>
	/// <param name="pos">�ʒu</param>
	/// <param name="ang">�p�x</param>
	void SetActive(bool value, const Vector3& pos = Vector3().zero, const Vector3& ang = Vector3().zero, const Vector3& size = Vector3().one);

	/// <summary>
	/// ���S��Ԃ̕ύX
	/// </summary>
	/// <param name="value">���S���Ă��邩</param>
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

	Vector3 scale;//�傫��

	int number = 0;//���ʔԍ�
	int destroyTime;//�폜����
	int destroyCount;//�폜�J�E���g

	float radius;//���a

	bool isActive;//�\�����
	bool isDestroy;//�폜�t���O

	SphereCollider* spehereCollider;

	string name;   //���f����
	string key;	   //���ʔԍ�
	string keyname;//���O�Ɣԍ��̍���
};