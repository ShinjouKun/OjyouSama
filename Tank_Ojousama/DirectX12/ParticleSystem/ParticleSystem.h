#pragma once

#include <memory>
#include <list>
#include <vector>

class Emitter;
//�����ł̓G�~�b�^�[�������T�|�[�g����
class ParticleSystem
{
private:
	ParticleSystem();

public:
	~ParticleSystem();

	static ParticleSystem& instance();
	//�I������
	void finalize();
	//�ǉ�
	void add(Emitter* add);
	//�X�V
	void update();
	//�`��
	void draw();

	//���g�����ׂď���
	void reset();

private:
	//�R�s�[�֎~
	ParticleSystem(const ParticleSystem&) = delete;
	ParticleSystem& operator=(const ParticleSystem&) = delete;
	ParticleSystem(ParticleSystem&&) = delete;
	ParticleSystem& operator=(ParticleSystem&&) = delete;

private:
	static ParticleSystem* mInstance;
	std::vector<std::shared_ptr<Emitter>> mEmitters;
	std::list<std::shared_ptr<Emitter>> mPendingEmitter;
};