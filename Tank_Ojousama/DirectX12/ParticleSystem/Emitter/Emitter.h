#pragma once

#include "EmitterData.h"
#include "../ParticleData/ParticleData.h"

#include <list>
#include <vector>
#include <string>

class Timer;
class Compute;

/*
4096
8192
16384
32768
65536
131072
262144
*/
constexpr int MAX_PARTICLE_SIZE = 8192;//�p�[�e�B�N���̍ő��//65536

//�e�G�~�b�^�[��gpu�p�[�e�B�N�������
class Emitter
{
public:
	Emitter(const Vector3& pos, const ParticleSystems& ps, const Burst& burst, std::string texName = "Resouse/particle.jpg");
	~Emitter();

	void update();

	void draw();

	bool isActive() const;

	void setPos(const Vector3& pos);
	void setVec(const Vector3& vec);
	void setRandomVec(const Vector3& vec);
	void setParticleSystems(const ParticleSystems& ps);
	void setAmountOfChangeInParticles(const AmountOfChangeInParticles& aocip);
	void setParticleRandomState(const ParticleRandomState& prs);
	void setParticleRotateState(const ParticleRotateState& prs);
	void setIsGo();
	void setStop();

	void death();

private:
	void add();

private:
	//�R�s�[�֎~
	Emitter(const Emitter&) = delete;
	Emitter& operator=(const Emitter&) = delete;
	Emitter(Emitter&&) = delete;
	Emitter& operator=(Emitter&&) = delete;

private:
	bool mActive;//�����Ă��邩�ǂ���
	bool mEnd;//�I���t���O
	bool mIsGo;//��~�@�\���g���Ȃ�
	bool mStop;//��~�����邩�ǂ���
	Vector3* mPos;//�G�~�b�^�[�̈ʒu
	Vector3* mVec;//�p�[�e�B�N�p�x�N�g��
	Vector3* mRandomVec;//�����_���v�f�p
	int mDelayTimer;
	ParticleSystems mParticleSystems;//�p�[�e�B�N���V�X�e��

	float mRateOverTime = 0.f;//
	float mRateOverDistance = 0.f;//
	Burst mBurst;
	AmountOfChangeInParticles mAOCIP;//�ω���
	ParticleRandomState mPRS;//�����_����
	ParticleRotateState mPS;//�p�[�e�B�N���̉�]�p

	bool isDeath;

	Timer* mTimer;
	Compute* mCompute;
private:
	//gpgpu�ɕK�v�ȃf�[�^
	std::vector<ParticleData> mDataList;
	std::list<ParticleData> mPendingDataList;
	void* mData;//compute�������f�[�^
};