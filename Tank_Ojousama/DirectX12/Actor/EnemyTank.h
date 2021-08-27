#pragma once
#include "BaseObject.h"
#include"ObjectManager.h"
#include<memory>
#include "../Render/ModelRenderer.h"
#include "../Render/ParticleManager.h"
#include"../Render/TexRenderer.h"

//�ǐ՗p�Z���T�[
struct SenserPoint //�_�@���m��������
{
	Vector3 Point;
	float SenserRadius;//���a
};
struct SenserFan //��@���E
{
	Vector3 FanPoint;//�ʒu�@��_
	float FanArea;//�͈�
	float FanLength;//����
	float FanAngle;//����p
};
//�댯�x
enum BatteleStatus
{
	SAFE,//���S
	WARNING,//�x��
	DANGER,//�댯
	BATTELE//�퓬
};

class EnemyTank :public BaseObject
{
public:
	EnemyTank(Vector3 pos, Vector3 ang, ObjectManager * obj, shared_ptr<ModelRenderer> m, shared_ptr<ParticleManager>p, shared_ptr<TexRenderer>s, int num);
	~EnemyTank();
	void Shot();
	void Move();
private:


	// BaseObject ����Čp������܂���
	virtual void Init() override;

	virtual void Update() override;

	virtual void Rend() override;

	void Senser();//�Z���T�[�X�V
	void Attack();
	void Escape();
	//�X�e�[�^�X�ʁi����Ȃ������j
	void Safe();
	void Warning();
	void Danger();
	void StatusChack();
	bool SenserIn(SenserFan fan, Vector3 point);//���E���ɉ������������H

	Vector3 GetEnemyVec(Vector3 vec);//�G�̃x�N�g��
	Vector3 GetFriendPos(int id);//������ID���l����GetPos�����s

	ObjectManager* objM;
	shared_ptr<ModelRenderer>tankModel;
	shared_ptr<ParticleManager>tankParticle;
	shared_ptr<ParticleEmitterBox>tankParticleBox;
	shared_ptr<TexRenderer>tankSprite;
	int HP;
	bool ShotFlag;
	int ShotCount;
	bool HitFlag;//���G���Ԃ̂���
	int HitCount;
	int bulletStock;//�e�������ς���������

	Vector3 angleVec;
	//���
	BatteleStatus batteleS;
	SenserPoint point;

	//���������p
	int number = 0;
	string tankAtk;//�L�[���[�h�o�^
	string tankHou;
	string tankBtm;
	string num;//�����p�i���o�[
	string numNameAtk;//���̕�����
	string numNameHou;
	string numNameBtm;//���̕�����
	int Count;
	float atkAngle;//�C���p//�`��
	float fireAngle;//�C�g
	bool moveFlag;
	bool FrontMove;
	bool BackMove;

	// BaseObject ����Čp������܂���
	virtual void OnCollison(BaseCollider* col) override;

	// BaseObject ����Čp������܂���
	virtual void ImGuiDebug() override;
};