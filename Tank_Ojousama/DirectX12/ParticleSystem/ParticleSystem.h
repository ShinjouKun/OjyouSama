#pragma once

#include <d3d12.h>
#include <d3dx12.h>
#include <dxgi1_6.h>
#include <d3dcompiler.h>
#include <wrl.h>
using namespace Microsoft::WRL;
#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")

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

	void gpuWait();

	ID3D12GraphicsCommandList* getGPUCMDList();

private:
	void init();

private:
	//�R�s�[�֎~
	ParticleSystem(const ParticleSystem&) = delete;
	ParticleSystem& operator=(const ParticleSystem&) = delete;
	ParticleSystem(ParticleSystem&&) = delete;
	ParticleSystem& operator=(ParticleSystem&&) = delete;

private:
	static ParticleSystem* mInstance;
	//std::vector<std::shared_ptr<Emitter>> mEmitters;
	//std::list<std::shared_ptr<Emitter>> mPendingEmitter;
	std::vector<Emitter*> mEmitters;
	std::list<Emitter*> mPendingEmitter;

	ID3D12GraphicsCommandList* mCMDList;
	ID3D12CommandAllocator* mCMDAllo;
	ID3D12CommandQueue* mCQueue;
};