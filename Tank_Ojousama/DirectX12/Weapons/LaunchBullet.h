#pragma once
#include "Weapon.h"


//�ł��グ�e(�����C�p)��܂Ȃ�ȋO��
class LaunchBullet : public Weapon
{
public:

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="position">�����ʒu</param>
	/// <param name="targetPosition">�ڕW�ʒu</param>
	/// <param name="objectManager">�I�u�W�F�N�g�}�l�[�W���[</param>
	/// <param name="modelRender">���f�������_�[</param>
	/// <param name="particleManager">�p�[�e�B�N���}�l�[�W���[</param>
	/// <param name="objectType">�^�C�v</param>
	/// <param name="num">���ʔԍ�</param>
	LaunchBullet(
		const Vector3& position,
		const Vector3& targetPosition,
		ObjectManager * objectManager,
		std::shared_ptr<ModelRenderer> modelRender,
		shared_ptr<ParticleManager> particleManager,
		ObjectType objectType,
		int num,
		const bool isApple = false
	);

	~LaunchBullet();

	/*�e�̎�ނ������S�ɕύX(Init�ŏ����Ă�)*/
	void ChangeApple(const bool value);

private:

	// Weapon ����Čp������܂���
	virtual void Init() override;
	virtual void Update() override;
	virtual void Rend() override;
	virtual void ImGuiDebug() override;
	virtual void OnCollison(BaseCollider * col) override;

	/*�e�̓���*/
	void Move();

private:

	ObjectManager * mObjectManager;
	std::shared_ptr<ModelRenderer> mModelRender;
	shared_ptr<ParticleManager> mParticleManager;
	shared_ptr<ParticleEmitterBox> mParticleEmitter;

	Vector3 mTargetPosition;//�ˌ��ʒu
	Vector3 mScale;         //�T�C�Y
	Vector3 mCircleScale;

	int BULLET_DAMAGE = 10;//�_���[�W
	float test = 0;

	float MOVE_SPEED = 1.0f;//�ړ����x
	float mVelocityY = 1.0f;//�c�̈ړ���
	float mHalfLength;//�����ʒu�ƖڕW�ʒu�̋����̔��������߂�

	bool mAppleFlag;
	bool mDrawArea;//�x���I�u�W�F�N�g��`�悷�邩

	string PARTICLE_NAME = "Bom";//�G�t�F�N�g�̖��O
	string EXPLOSION = "ExBom";//�v���C���[�ɓ����������̃G�t�F�N�g

	//�����n�_�p�I�u�W�F�N�g
	string mBlockName = "BLOCK";
	string numBlock;

	//�����S
	string mAppleName = "Apple";
	string mAppleNum;
};