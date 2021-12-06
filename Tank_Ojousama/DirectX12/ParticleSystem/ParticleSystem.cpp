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
		//追加したものをvectorに移す
		std::copy(mPendingEmitter.begin(), mPendingEmitter.end(), std::back_inserter(mEmitters));
		mPendingEmitter.clear();
	}

	for (int i = 0, end = static_cast<int>(mEmitters.size()); i < end; ++i)
	{
		if (!mEmitters[i]->isActive())
		{
			//多分これでいけるはず……
			std::swap(mEmitters[i], mEmitters.back());
			--end;
			if (i == end)break;//最後になっていたら抜ける
		}
		mEmitters[i]->update();
	}
}

void ParticleSystem::draw()
{
	//各エミッターからパーティクルが描画される
	for (int i = 0, end = static_cast<int>(mEmitters.size()); i < end; ++i)
	{
		mEmitters[i]->draw();
	}
}




ParticleSystem* ParticleSystem::mInstance = nullptr;