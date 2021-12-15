#include "ParticleSystem.h"
#include "Emitter/Emitter.h"

#include "../Device/DirectXManager.h"
#include "Compute/Compute.h"

ParticleSystem::ParticleSystem()
{
	init();
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

	mCMDList->Release();
	mCMDAllo->Release();
	mCQueue->Release();

	Compute::finalize();

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
			mEmitters.pop_back();
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

void ParticleSystem::reset()
{
	mPendingEmitter.clear();
	mEmitters.clear();
}

void ParticleSystem::gpuWait()
{
	mCMDList->Close();

	ID3D12CommandList* com[] = { mCMDList };

	mCQueue->ExecuteCommandLists(1, com);

	DirectXManager::GetInstance()->waitGPU(mCQueue);


	mCMDAllo->Reset();
	mCMDList->Reset(mCMDAllo, nullptr);
}

ID3D12GraphicsCommandList * ParticleSystem::getGPUCMDList()
{
	return mCMDList;
}

void ParticleSystem::init()
{
	HRESULT hr;

	auto dev = DirectXManager::GetInstance()->Dev();

	hr = dev->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_COMPUTE, IID_PPV_ARGS(&mCMDAllo));
	assert(SUCCEEDED(hr));

	hr = dev->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_COMPUTE, mCMDAllo, nullptr, IID_PPV_ARGS(&mCMDList));
	assert(SUCCEEDED(hr));

	D3D12_COMMAND_QUEUE_DESC cmdQueueDesc{};
	cmdQueueDesc.Type = D3D12_COMMAND_LIST_TYPE_COMPUTE;
	hr = dev->CreateCommandQueue(&cmdQueueDesc, IID_PPV_ARGS(&mCQueue));
	assert(SUCCEEDED(hr));
}


ParticleSystem* ParticleSystem::mInstance = nullptr;