#pragma once
#include"../Render/ParticleManager.h"
#include "BaseObject.h"
#include"ObjectManager.h"
#include "../Render/ModelRenderer.h"
#include "../Render/ParticleManager.h"
#include"../Render/TexRenderer.h"
//�G���t�̐X�p�̒��{�X
//���_�h�q�^�Ȃ̂Ńv���C���[���͈͊O�ɂł��猴�_�ɖ߂�
struct Canp
{
	Vector3 CanpPoint;//���_
	float CanpRadius;//���a
};
//�ǐ՗p�Z���T�[
struct SenserPoint_G //�_�@���m��������
{
	Vector3 Point;
	float SenserRadius;//���a
};
struct SenserFan_G //��@���E
{
	Vector3 FanPoint;//�ʒu�@��_
	float FanArea;//�͈�
	float FanLength;//����
	float FanAngle;//����p
};
//�댯�x
enum GolemBatteleStatus
{
	SAFE_G,//���S(�ҋ@)
	BATTELE_G,//�퓬
	RETURN_G//�A�ҁi�v���C���[�����_�͈͂���O�ꂽ���j
};
class GolemEnemy:public BaseObject
{
public:
	GolemEnemy(Vector3 position,
		Vector3 angle,
		ObjectManager * objManager,
		shared_ptr<ModelRenderer>modelRender,
		shared_ptr<TexRenderer>texRender,
		shared_ptr<ParticleManager> effect,
		int num);
	~GolemEnemy();

private:
	//�Z���T�[�֘A
	void Senser();//�Z���T�[�X�V
	bool SenserIn(SenserFan_G fan, Vector3 point);//���E���ɉ������������H
	//�s��
	bool CanpOut();//�h�q�n�_����o�����H
	bool Distance(const Vector3 & otherPos, float dist);
	void Safe();
	void Battele();
	void PredecessorMove();//�߂Â�����
	void ReturnMove();//���_�ɖ߂铮��
	void AvoidMove();//�����铮��
	void ProximityAttack();//�ߐ�
	void LangeAttack();//������
	void SpecialAttack();//�K�E�Z
	Vector3 GetEnemyVec(const Vector3& vec);//�G�̃x�N�g��
	Vector3 GetCampVec(const Vector3& vec);//���_�̃x�N�g��
	// BaseObject ����Čp������܂���

	virtual void Init() override;

	virtual void Update() override;

	virtual void Rend() override;

	virtual void ImGuiDebug() override;

	virtual void OnCollison(BaseCollider * col) override;
private:
	ObjectManager* objM;
	shared_ptr<ModelRenderer>Model;
	shared_ptr<ParticleManager>Particle;
	shared_ptr<ParticleEmitterBox>ParticleBox;
	shared_ptr<TexRenderer>Sprite;

	GolemBatteleStatus batteleS;
	SenserPoint_G point;
	Canp canp;

	//���G���Ԃ̂���
	bool HitFlag;
	int HitCount;
	//�X�e�[�^�X
	float ySpeed;
	int speedTime;
	float maxSpeed;
	float speedLimitTime;
	Vector3 ArmPosR;
	Vector3 ArmPosL;
	int HP;
	int ProximityPoint;//�ߐڍU����
	int LangePoint;//�������U����
	int SpecialPoint;//�K�E�Z�Ȃ�
	bool AttackFlag;//���ʂ̍U���p�t���O
	int AttackCount;
	int attackMoveCount;//�U�����I���܂ł̃J�E���g�i�߉��j
	Vector3 angleVec;//�ړ��������
	//���������p
	int number = 0;
	string Body;//�L�[���[�h�o�^
	string ArmR;
	string ArmL;
	string num;//�����p�i���o�[
	string numNameBody;//���̕�����
	string numNameArmR;
	string numNameArmL;//���̕�����
	int Count;
	Vector3 bodyAngle;//Y��//�`��
	float ArmAngleR;//�r
	float zR;
	float ArmAngleL;//�r
	float zL;
	bool moveFlag;
	bool avmoveFlag;
	int avmoveCount;//�����_���ړ��J�E���g
	int moveCount;//�퓬���̐ڋ߉񐔐����p�J�E���g
	int rndCount;
};
