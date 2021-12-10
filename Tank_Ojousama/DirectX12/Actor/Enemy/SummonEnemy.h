#pragma once
#include "../BaseObject.h"
#include "../ObjectManager.h"
#include "../../Render/ModelRenderer.h"
#include "../../Render/ParticleManager.h"

class Timer;
class Sound;

//�{�X�ɏ��������G���G
class SummonEnemy : public BaseObject
{
public:

	SummonEnemy(
		const Vector3 & position,
		const Vector3 & angle,
		ObjectManager * objManager,
		std::shared_ptr<ModelRenderer> modelRender,
		shared_ptr<ParticleManager> effectManager,
		int num
	);

	~SummonEnemy();

private:

	/*�e�N���X����p��*/
	virtual void Init() override;
	virtual void Update() override;
	virtual void Rend() override;
	virtual void ImGuiDebug() override;
	virtual void OnCollison(BaseCollider * col) override;

	/*������Ԃ��Ď�*/
	void CheckAlive();

	/*�ړ�*/
	void Move();

	/*���s�A�j���[�V����*/
	void MoveAnimation();

	/*�|�ꂱ�ݍU��*/
	void DownAttack();

	/*�|�ꂱ��*/
	void AttackStep_FallDown();
	/*���b��~*/
	void AttackStep_Wait();
	/*�N���オ��*/
	void AttackStep_GetUp();

	/*���S���̉��o*/
	void DeathAnimation();

	/*��ɏ㏸*/
	void DeathAnimeStep_RiseSky();
	/*����&���b��~*/
	void DeathAnimeStep_Explosion();

	/*�w�苗���ȓ����ǂ����𒲂ׂ�*/
	bool WithinDistance(const Vector3& targetPosition, const float distance);

private:

	/*�U���A�j���[�V�����̑J�ڏ�*/
	enum AttackStep
	{
		FALL_DOWN,//�|�ꂱ��
		WAIT,     //�|�ꂽ�܂ܒ�~
		GET_UP,   //�N���オ��
	};
	AttackStep mAttackStep;

	/*���S�A�j���[�V�����̑J�ڏ�*/
	enum DeathAnimationStep
	{
		RISE_SKY, //���ɕ����オ��
		EXPLOSION,//���ł���
	};
	DeathAnimationStep mDeathStep;

	ObjectManager * mObjManager;

	shared_ptr<ModelRenderer> mModelRender;
	shared_ptr<ParticleManager> mEffectManager;
	shared_ptr<Timer> mGetupTimer;//�|�ꂱ��ł��鎞��
	shared_ptr<Timer> mRiseTime;  //�㏸����
	shared_ptr<Timer> mDeathTime; //���S�Ɏ��ʂ܂ł̎���
	shared_ptr<Sound> mAttackSE;  //�U������SE
	shared_ptr<Sound> mDamageSE;  //�_���[�W���󂯂�����SE
	shared_ptr<Sound> mDeathSE;   //���S�����Ƃ���SE
	shared_ptr<ParticleEmitterBox> mParticleEmitter;//�����̃G�t�F�N�g

	Vector3 mScale;           //�傫��
	Vector3 mPlayerPosition;  //�v���C���[�̈ʒu
	Vector3 mPreviousPosition;//�O�t���[���̈ʒu

	const float SEARCH_LENGTH = 50.0f;//���̋����ȓ��ɂȂ�����ǐՂ���
	const float FIND_LRNGTH = 20.0f;  //���̋����ȓ��ɂȂ�����ˌ�����
	const float ATTACK_LENGTH = 5.0f; //���̋����ȓ��ɂȂ�����U������
	float mFireAngle = 0.0f;//�U�����̌���
	float mDownSpeed = 3.0f;//�|�ꂱ�ރA�j���[�V�����̑��x

	bool mWithinPlayerFlag;//�͈͓��Ƀv���C���[�����邩�H
	bool mAttackFlag;      //�U������
	bool mStep;            //true = �E, false = ��
	bool mDeathAnimation;  //���S�A�j���[�V�������J�n���邩
	bool mDeadFlag;        //���ʏu�Ԃ�true�ɂȂ�t���O(�ی�)

	string mMyNumber;       //���ʔԍ�
	string mSummon;         //���f����
	string mSummonNum;      //���f���� + ���ʔԍ�
	string EXPLOSION_EFFECT;//�G�t�F�N�g��
};