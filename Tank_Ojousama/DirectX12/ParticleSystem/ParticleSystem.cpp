#include "ParticleSystem.h"
#include "Emitter/Emitter.h"

ParticleSystem::ParticleSystem()
{
}

ParticleSystem::~ParticleSystem() = default;

ParticleSystem & ParticleSystem::instance()
{
	if (!mInstance)
	{
		mInstance = new ParticleSystem();
	}
	return *mInstance;
}

void ParticleSystem::finalize()
{
	mPendingEmitter.clear();
	mEmitters.clear();
	if (mInstance)
	{
		delete(mInstance);
	}
	mInstance = nullptr;
}

void ParticleSystem::add(Emitter * add)
{
	mPendingEmitter.emplace_back(add);
}

void ParticleSystem::update()
{
	if (!mPendingEmitter.empty())
	{
		//�ǉ��������̂�vector�Ɉڂ�
		std::copy(mPendingEmitter.begin(), mPendingEmitter.end(), std::back_inserter(mEmitters));
		mPendingEmitter.clear();
	}

	for (int i = 0, end = static_cast<int>(mEmitters.size()); i < end; ++i)
	{
		if (!mEmitters[i]->isActive())
		{
			//��������ł�����͂��c�c
			std::swap(mEmitters[i], mEmitters.back());
			--end;
			if (i == end)break;//�Ō�ɂȂ��Ă����甲����
		}
		mEmitters[i]->update();
	}
}

void ParticleSystem::draw()
{
	//�e�G�~�b�^�[����p�[�e�B�N�����`�悳���
	for (int i = 0, end = static_cast<int>(mEmitters.size()); i < end; ++i)
	{
		mEmitters[i]->draw();
	}
}




ParticleSystem* ParticleSystem::mInstance = nullptr;