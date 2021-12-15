#include "Compute.h"
#include "Shader.h"
#include "StructuredBuffer/StructuredBuffer.h"
#include "RWStructuredBuffer/RWStructuredBuffer.h"
#include "../ParticleData/ParticleData.h"
#include "../Emitter/Emitter.h"
#include "../Emitter/EmitterData.h"

#include "../../Device/DirectXManager.h"
#include "../../Device/Window.h"
#include "../../Render/Camera.h"

#include "../ParticleSystem.h"

Compute::Compute(std::string texName):
	mTexName(texName)
{
}

Compute::~Compute()
{
	delete(mCSShader);
	delete(mPSShader);
	delete(mGSShader);
	delete(mVSShader);
	delete(mParticleCSShader);

	delete(mInputEmitterSB);
	delete(mOutputEmitterSB);
	delete(mInputParticleDataSB);
	delete(mOutputParticleSB);
	delete(mParticleStorageSB);

	mHeap->Release();

	mVertBuff->Release();
	mConstBuff->Release();
	mTexBuff->Release();
}

void Compute::finalize()
{
	mEmitterRoot->Release();
	mEmitterPipe->Release();
	mParticleRoot->Release();
	mParticlePipe->Release();
	mParticleDrawRoot->Release();
	mParticleDrawPipe->Release();

	//mCMDList->Release();
	//mCMDAllo->Release();
	//mCQueue->Release();

}

void Compute::init()
{
	mDev = DirectXManager::GetInstance()->Dev();

	createHeap();
	//createCMDList();
	createPiprLine();
	createBuffer();


	//mMatProjection3D = Matrix4::Identity;
	//mMatProjection3D.m[3][0] = -1.0f;
	//mMatProjection3D.m[3][1] = 1.0f;
	//mMatProjection3D.m[0][0] = 2.0f / Window::Window_Width;
	//mMatProjection3D.m[1][1] = -2.0f / Window::Window_Height;


	mMatProjection3D = Matrix4::createPerspectiveFOV(60.f, Window::Window_Width, Window::Window_Height, 0.1f, 3000.0f);
}

void Compute::emitterUpdate(void* data, int dispatch)
{
	return;
	/*
	mInputEmitterSB->update(data, 1);
	//�p�C�v���C���E���[�g�V�O�l�`�����Z�b�g
	mCMDList->SetPipelineState(mEmitterPipe);
	mCMDList->SetComputeRootSignature(mEmitterRoot);

	//�f�X�N���v�^�q�[�v
	mCMDList->SetDescriptorHeaps(1, &mHeap);

	auto gpuHandle = mHeap->GetGPUDescriptorHandleForHeapStart();
	auto handle = mDev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

	mCMDList->SetComputeRootDescriptorTable(0, gpuHandle);

	gpuHandle.ptr += handle;

	mCMDList->SetComputeRootDescriptorTable(1, gpuHandle);

	mCMDList->Dispatch(dispatch, 1, 1);

	mCMDList->Close();

	ID3D12CommandList* com[] = { mCMDList };

	mCQueue->ExecuteCommandLists(1, com);//�����ŃG���[

	DirectXManager::GetInstance()->waitGPU(mCQueue);

	mCMDAllo->Reset();
	mCMDList->Reset(mCMDAllo, nullptr);
	*/
}

void* Compute::particleUpdate(void* data, int dataSize)
{
	auto cmdList = ParticleSystem::instance().getGPUCMDList();

	mInputParticleDataSB->update(data, dataSize);

	//�p�C�v���C���E���[�g�V�O�l�`�����Z�b�g
	cmdList->SetPipelineState(mParticlePipe);
	cmdList->SetComputeRootSignature(mParticleRoot);

	//�f�X�N���v�^�q�[�v
	cmdList->SetDescriptorHeaps(1, &mHeap);

	auto gpuHandle = mHeap->GetGPUDescriptorHandleForHeapStart();
	auto handle = mDev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

	gpuHandle.ptr += handle * 2;

	cmdList->SetComputeRootDescriptorTable(0, gpuHandle);

	gpuHandle.ptr += handle;

	cmdList->SetComputeRootDescriptorTable(1, gpuHandle);

	gpuHandle.ptr += handle;

	cmdList->SetComputeRootDescriptorTable(2, gpuHandle);

	cmdList->Dispatch(MAX_PARTICLE_SIZE, 1, 1);

	ParticleSystem::instance().gpuWait();

	/*
	mCMDList->Close();

	ID3D12CommandList* com[] = { mCMDList };

	mCQueue->ExecuteCommandLists(1, com);

	DirectXManager::GetInstance()->waitGPU(mCQueue);


	mCMDAllo->Reset();
	mCMDList->Reset(mCMDAllo, nullptr);
	*/
	return mParticleStorageSB->getResourceOnCPU();
}

void Compute::particleDraw(int dataSize)
{
	//�r���{�[�h�s��̐���
	Matrix4 matBillboard = Matrix4::Identity;
	Vector3 zaxis = Vector3::normalize(Camera::target - Camera::eye);
	Vector3 xaxis = Vector3::normalize(Vector3::cross(Camera::up, zaxis));
	Vector3 yaxis = Vector3::cross(zaxis, xaxis);
	float temp[4][4] =
	{
		{ xaxis.x, xaxis.y, xaxis.z, 0.0f },
		{ yaxis.x, yaxis.y, yaxis.z, 0.0f },
		{ zaxis.x, zaxis.y, zaxis.z, 0.0f },
		{ 0.0f, 0.0f, 0.0f, 1.0f }
	};
	matBillboard = Matrix4(temp);

	ParticleConstBuff* constBuff = nullptr;
	HRESULT hr = mConstBuff->Map(0, nullptr, (void**)&constBuff);
	assert(SUCCEEDED(hr));
	constBuff->mat = Camera::matView * mMatProjection3D;
	constBuff->matbillboard = matBillboard;
	mConstBuff->Unmap(0, nullptr);

	auto data = mOutputParticleSB->getResourceOnCPU();
	std::vector<ParticleDrawData> v;
	v.assign((ParticleDrawData*)data, (ParticleDrawData*)data + dataSize);

	ParticleVertex* vertMap;
	mVertBuff->Map(0, nullptr, (void**)&vertMap);
	for (int i = 0, end = v.size(); i < end; ++i)
	{
		vertMap->pos = v[i].pos;
		vertMap->color = v[i].color;
		vertMap->size = v[i].size;
		vertMap->rotate = v[i].rotate;
		++vertMap;
	}
	//vertMap = (ParticleDrawData*)data;
	mVertBuff->Unmap(0, nullptr);

	auto* cmdList = DirectXManager::GetInstance()->CmdList();

	//�p�C�v���C�����Z�b�g
	cmdList->SetPipelineState(mParticleDrawPipe);

	//���[�g�V�O�l�`�����Z�b�g
	cmdList->SetGraphicsRootSignature(mParticleDrawRoot);

	//�v���~�e�B�u�`����Z�b�g
	cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_POINTLIST);

	//���_�o�b�t�@���Z�b�g
	cmdList->IASetVertexBuffers(0, 1, &mVbView);

	//�f�X�N���v�^�q�[�v���Z�b�g
	cmdList->SetDescriptorHeaps(1, &mHeap);

	auto gpuHandle = mHeap->GetGPUDescriptorHandleForHeapStart();
	auto handle = mDev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

	gpuHandle.ptr += handle * 5;

	//�萔�o�b�t�@�r���[���Z�b�g
	cmdList->SetGraphicsRootDescriptorTable(0, gpuHandle);
	gpuHandle.ptr += handle;

	//�V�F�[�_���\�[�X�r���[���Z�b�g
	cmdList->SetGraphicsRootDescriptorTable(1, gpuHandle);

	//�`��R�}���h
	//cmdList->DrawInstanced(static_cast<UINT>(std::distance(v.begin(), v.end())), 1, 0, 0);
	cmdList->DrawInstanced(dataSize, 1, 0, 0);
}

void Compute::barrier(ID3D12Resource * p, D3D12_RESOURCE_STATES before, D3D12_RESOURCE_STATES after)
{
	//mCMDList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(p, before, after, 0));
}

HRESULT Compute::createCMDList()
{
	HRESULT hr = S_OK;
	/*
	hr = mDev->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_COMPUTE, IID_PPV_ARGS(&mCMDAllo));
	assert(SUCCEEDED(hr));

	hr = mDev->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_COMPUTE, mCMDAllo, nullptr, IID_PPV_ARGS(&mCMDList));
	assert(SUCCEEDED(hr));

	D3D12_COMMAND_QUEUE_DESC cmdQueueDesc{};
	cmdQueueDesc.Type = D3D12_COMMAND_LIST_TYPE_COMPUTE;
	hr = mDev->CreateCommandQueue(&cmdQueueDesc, IID_PPV_ARGS(&mCQueue));
	assert(SUCCEEDED(hr));
	*/
	return hr;
}

HRESULT Compute::createHeap()
{
	//�q�[�v�𐶐�
	D3D12_DESCRIPTOR_HEAP_DESC heapDesc{};
	heapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	heapDesc.NodeMask = 0;
	heapDesc.NumDescriptors = 512;//�V�F�[�_���\�[�X�r���[�̐�
	heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;

	return mDev->CreateDescriptorHeap(&heapDesc, IID_PPV_ARGS(&mHeap));
}

HRESULT Compute::createPiprLine()
{
	HRESULT hr = S_OK;
	//�G�~�b�^�[�X�V
	mCSShader = new Shader();
	mCSShader->loadCS("ComputeEmitterShader.hlsl");
	//�p�[�e�B�N���X�V
	mParticleCSShader = new Shader();
	mParticleCSShader->loadCS("ComputeParticleUpdateShader.hlsl");
	//�p�[�e�B�N���`��
	mPSShader = new Shader();
	mPSShader->loadPS("ParticlePixelShader.hlsl", "");
	mGSShader = new Shader();
	mGSShader->loadGS("ParticleGeometryShader.hlsl", "");
	mVSShader = new Shader();
	mVSShader->loadVS("ParticleVertexShader.hlsl", "");

	ComPtr<ID3DBlob> error;
	D3D12_COMPUTE_PIPELINE_STATE_DESC psoDesc{};

#pragma region �G�~�b�^�[�X�V�p

	if (!mEmitterPipe)
	{
		//���[�g�V�O�l�`��
		D3D12_STATIC_SAMPLER_DESC sampler = {};
		sampler.Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;
		sampler.AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
		sampler.AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
		sampler.AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
		sampler.MipLODBias = 0;
		sampler.MaxAnisotropy = 0;
		sampler.ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
		sampler.BorderColor = D3D12_STATIC_BORDER_COLOR_OPAQUE_BLACK;
		sampler.MinLOD = 0.0f;
		sampler.MaxLOD = D3D12_FLOAT32_MAX;
		sampler.ShaderRegister = 0;
		sampler.RegisterSpace = 0;
		sampler.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;

		CD3DX12_DESCRIPTOR_RANGE1 range[2];
		CD3DX12_ROOT_PARAMETER1 rootParam[2];
		UINT maxCbvDescriptor = 8;
		UINT maxSrvDescriptor = 32;
		UINT maxUavDescritor = 8;
		UINT offsetInDescriptorsFromTableStartCB = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;
		UINT offsetInDescriptorsFromTableStartSRV = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;
		UINT offsetInDescriptorsFromTableStartUAV = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

		range[0].Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0, 0, D3D12_DESCRIPTOR_RANGE_FLAG_DESCRIPTORS_VOLATILE);
		range[1].Init(D3D12_DESCRIPTOR_RANGE_TYPE_UAV, 1, 0, 0, D3D12_DESCRIPTOR_RANGE_FLAG_DATA_VOLATILE);

		rootParam[0].InitAsDescriptorTable(1, &range[0]);
		rootParam[1].InitAsDescriptorTable(1, &range[1]);
		//rootParam[2].InitAsDescriptorTable(1, &range[2]);

		// Allow input layout and deny uneccessary access to certain pipeline stages.
		D3D12_ROOT_SIGNATURE_FLAGS rootSignatureFlags =
			D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT |
			D3D12_ROOT_SIGNATURE_FLAG_DENY_HULL_SHADER_ROOT_ACCESS |
			D3D12_ROOT_SIGNATURE_FLAG_DENY_DOMAIN_SHADER_ROOT_ACCESS |
			D3D12_ROOT_SIGNATURE_FLAG_DENY_GEOMETRY_SHADER_ROOT_ACCESS;

		CD3DX12_VERSIONED_ROOT_SIGNATURE_DESC rootSignatureDesc;
		rootSignatureDesc.Init_1_1(_countof(rootParam), rootParam, 0, nullptr);

		ComPtr<ID3DBlob> signature;
		//ComPtr<ID3DBlob> error;

		D3DX12SerializeVersionedRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1, &signature, &error);
		hr = mDev->CreateRootSignature(0, signature->GetBufferPointer(), signature->GetBufferSize(), IID_PPV_ARGS(&mEmitterRoot));

		assert(SUCCEEDED(hr));

		//�p�C�v���C���X�e�[�g
		//D3D12_COMPUTE_PIPELINE_STATE_DESC psoDesc{};
		psoDesc.pRootSignature = mEmitterRoot;
		psoDesc.CS = CD3DX12_SHADER_BYTECODE(mCSShader->getCompiledBlob());
		psoDesc.Flags = D3D12_PIPELINE_STATE_FLAG_NONE;
		psoDesc.NodeMask = 0;

		hr = mDev->CreateComputePipelineState(&psoDesc, IID_PPV_ARGS(&mEmitterPipe));

		assert(SUCCEEDED(hr));
	}

#pragma endregion //�G�~�b�^�[�X�V�p

#pragma region �p�[�e�B�N���X�V�p

	if (!mParticlePipe)
	{
		CD3DX12_DESCRIPTOR_RANGE1 range2[3];
		CD3DX12_ROOT_PARAMETER1 rootParam2[3];

		range2[0].Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0, 0, D3D12_DESCRIPTOR_RANGE_FLAG_DESCRIPTORS_VOLATILE);
		range2[1].Init(D3D12_DESCRIPTOR_RANGE_TYPE_UAV, 1, 0, 0, D3D12_DESCRIPTOR_RANGE_FLAG_DATA_VOLATILE);
		range2[2].Init(D3D12_DESCRIPTOR_RANGE_TYPE_UAV, 1, 1, 0, D3D12_DESCRIPTOR_RANGE_FLAG_DATA_VOLATILE);

		rootParam2[0].InitAsDescriptorTable(1, &range2[0]);
		rootParam2[1].InitAsDescriptorTable(1, &range2[1]);
		rootParam2[2].InitAsDescriptorTable(1, &range2[2]);

		CD3DX12_VERSIONED_ROOT_SIGNATURE_DESC rootSignatureDesc2;
		rootSignatureDesc2.Init_1_1(_countof(rootParam2), rootParam2, 0, nullptr);

		ComPtr<ID3DBlob> signature2;

		hr = D3DX12SerializeVersionedRootSignature(&rootSignatureDesc2, D3D_ROOT_SIGNATURE_VERSION_1, &signature2, &error);
		assert(SUCCEEDED(hr));

		hr = mDev->CreateRootSignature(0, signature2->GetBufferPointer(), signature2->GetBufferSize(), IID_PPV_ARGS(&mParticleRoot));

		assert(SUCCEEDED(hr));

		psoDesc.pRootSignature = mParticleRoot;
		psoDesc.CS = CD3DX12_SHADER_BYTECODE(mParticleCSShader->getCompiledBlob());
		psoDesc.Flags = D3D12_PIPELINE_STATE_FLAG_NONE;
		psoDesc.NodeMask = 0;

		hr = mDev->CreateComputePipelineState(&psoDesc, IID_PPV_ARGS(&mParticlePipe));

		assert(SUCCEEDED(hr));
	}
#pragma endregion //�p�[�e�B�N���X�V�p

#pragma region �p�[�e�B�N���`��p

	if (!mParticleDrawPipe)
	{
		//�O���t�B�b�N�X�p�C�v���C���̗���̐ݒ�
		D3D12_GRAPHICS_PIPELINE_STATE_DESC gpipeline{ };

		gpipeline.VS = CD3DX12_SHADER_BYTECODE(mVSShader->getCompiledBlob());
		gpipeline.GS = CD3DX12_SHADER_BYTECODE(mGSShader->getCompiledBlob());
		gpipeline.PS = CD3DX12_SHADER_BYTECODE(mPSShader->getCompiledBlob());

		D3D12_INPUT_ELEMENT_DESC inputLayout[]
		{
			{
				"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,
				D3D12_APPEND_ALIGNED_ELEMENT,
				D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
			},
			{

				"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0,
				D3D12_APPEND_ALIGNED_ELEMENT,
				D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
			},
			{
				"TEXCOORD", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,
				D3D12_APPEND_ALIGNED_ELEMENT,
				D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
			},
			{
				"TEXCOORD", 1, DXGI_FORMAT_R32G32B32_FLOAT, 0,
				D3D12_APPEND_ALIGNED_ELEMENT,
				D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
			},
		};

		gpipeline.InputLayout.pInputElementDescs = inputLayout;
		gpipeline.InputLayout.NumElements = 4;

		// �T���v���}�X�N
		gpipeline.SampleMask = D3D12_DEFAULT_SAMPLE_MASK; // �W���ݒ�
		//���X�^���C�U�X�e�[�g
		gpipeline.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);//��������W�����Z�b�g
		gpipeline.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;//�w�ʃJ�����O���Ȃ�

		gpipeline.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID;

		// �f�v�X�X�e���V���X�e�[�g
		gpipeline.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
		gpipeline.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ZERO;
		gpipeline.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS_EQUAL;//D3D12_COMPARISON_FUNC_NOT_EQUAL

		// �����_�[�^�[�Q�b�g�̃u�����h�ݒ�
		D3D12_RENDER_TARGET_BLEND_DESC blenddesc{};
		blenddesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;	// RBGA�S�Ẵ`�����l����`��
		blenddesc.BlendEnable = true;
		blenddesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;
		blenddesc.SrcBlendAlpha = D3D12_BLEND_ONE;
		blenddesc.DestBlendAlpha = D3D12_BLEND_ZERO;

		blenddesc.BlendOp = D3D12_BLEND_OP_ADD;
		blenddesc.SrcBlend = D3D12_BLEND_ONE;
		blenddesc.DestBlend = D3D12_BLEND_ONE;

		// �u�����h�X�e�[�g�̐ݒ�
		gpipeline.BlendState.RenderTarget[0] = blenddesc;

		// �[�x�o�b�t�@�̃t�H�[�}�b�g
		gpipeline.DSVFormat = DXGI_FORMAT_D32_FLOAT;

		// �}�`�̌`��ݒ�
		gpipeline.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_POINT;

		// �`��Ώ�
		gpipeline.NumRenderTargets = 1;

		gpipeline.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM; // 0�`255�w���RGBA

		gpipeline.SampleDesc.Count = 1; // 1�s�N�Z���ɂ�1��T���v�����O

		// ���[�g�p�����[�^

		CD3DX12_DESCRIPTOR_RANGE1 range3[2];
		CD3DX12_ROOT_PARAMETER1 rootParam3[2];

		range3[0].Init(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, 1, 0);
		range3[1].Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0);


		rootParam3[0].InitAsDescriptorTable(1, &range3[0], D3D12_SHADER_VISIBILITY_ALL);
		rootParam3[1].InitAsDescriptorTable(1, &range3[1], D3D12_SHADER_VISIBILITY_ALL);

		// �X�^�e�B�b�N�T���v���[
		CD3DX12_STATIC_SAMPLER_DESC samplerDesc = CD3DX12_STATIC_SAMPLER_DESC(0);

		// ���[�g�V�O�l�`���̐ݒ�
		CD3DX12_VERSIONED_ROOT_SIGNATURE_DESC rootSignatureDesc3;
		rootSignatureDesc3.Init_1_1(_countof(rootParam3), rootParam3, 1, &samplerDesc, D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

		ComPtr<ID3DBlob> signature3;

		hr = D3DX12SerializeVersionedRootSignature(&rootSignatureDesc3, D3D_ROOT_SIGNATURE_VERSION_1, &signature3, &error);
		assert(SUCCEEDED(hr));

		hr = mDev->CreateRootSignature(0, signature3->GetBufferPointer(), signature3->GetBufferSize(), IID_PPV_ARGS(&mParticleDrawRoot));

		assert(SUCCEEDED(hr));

		gpipeline.pRootSignature = mParticleDrawRoot;

		hr = mDev->CreateGraphicsPipelineState(&gpipeline, IID_PPV_ARGS(&mParticleDrawPipe));
	}

#pragma endregion //�p�[�e�B�N���`��p


	return hr;
}

HRESULT Compute::createBuffer()
{
	HRESULT hr;

	auto cpuHandle = mHeap->GetCPUDescriptorHandleForHeapStart();
	auto handle = mDev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

	//�G�~�b�^�[�C���v�b�g
	mInputEmitterSB = new StructuredBuffer();
	//mInputEmitterSB->setDevice(mDev);
	mInputEmitterSB->init(sizeof(EmitterData), 1, nullptr);

	cpuHandle.ptr += handle;
	//�G�~�b�^�[�A�E�g�v�b�g
	mOutputEmitterSB = new RWStructuredBuffer();
	//mOutputEmitterSB->setDevice(mDev);
	mOutputEmitterSB->init(sizeof(ParticleData), MAX_PARTICLE_SIZE, nullptr);

	cpuHandle.ptr += handle;
	//�p�[�e�B�N���C���v�b�g
	mInputParticleDataSB = new StructuredBuffer();
	//mInputParticleDataSB->setDevice(mDev);
	mInputParticleDataSB->init(sizeof(ParticleData), MAX_PARTICLE_SIZE, nullptr);
	mInputParticleDataSB->RegistShaderResourceView(cpuHandle, 0);

	cpuHandle.ptr += handle;
	//�p�[�e�B�N���`��p
	mOutputParticleSB = new RWStructuredBuffer();
	//mOutputParticleSB->setDevice(mDev);
	mOutputParticleSB->init(sizeof(ParticleDrawData), MAX_PARTICLE_SIZE, nullptr);
	mOutputParticleSB->RegistUnorderAccessView(cpuHandle, 0);

	cpuHandle.ptr += handle;
	//�p�[�e�B�N���ۊǗp
	mParticleStorageSB = new RWStructuredBuffer();
	//mParticleStorageSB->setDevice(mDev);
	mParticleStorageSB->init(sizeof(ParticleData), MAX_PARTICLE_SIZE, nullptr);
	mParticleStorageSB->RegistUnorderAccessView(cpuHandle, 0);

	cpuHandle.ptr += handle;

	ParticleVertex vert[] =
	{
		{{0.0f,0.0f,0.0f}}
	};

	//�ő���̃o�b�t�@�𐶐�
	hr = mDev->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(sizeof(vert)*MAX_PARTICLE_SIZE),//sizeof(vert[0])*vert.size()
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&mVertBuff));

	assert(SUCCEEDED(hr));

	ParticleVertex* vertMap;
	mVertBuff->Map(0, nullptr, (void**)&vertMap);
	memcpy(vertMap, vert, sizeof(vert));
	mVertBuff->Unmap(0, nullptr);

	// ���_�o�b�t�@�r���[�̍쐬
	mVbView.BufferLocation = mVertBuff->GetGPUVirtualAddress();
	mVbView.SizeInBytes = sizeof(vert)*MAX_PARTICLE_SIZE;
	mVbView.StrideInBytes = sizeof(vert[0]);

	// �萔�o�b�t�@�̐���
	hr = mDev->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD), 	// �A�b�v���[�h�\
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer((sizeof(ParticleConstBuff) + 0xff)&~0xff),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&mConstBuff));

	assert(SUCCEEDED(hr));

	//�萔�o�b�t�@�r���[�̊��蓖��
	D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc{};
	cbvDesc.BufferLocation = mConstBuff->GetGPUVirtualAddress();
	cbvDesc.SizeInBytes = static_cast<UINT>(mConstBuff->GetDesc().Width);
	mDev->CreateConstantBufferView(&cbvDesc, cpuHandle);
	cpuHandle.ptr += handle;

	//�摜�ǂݍ���
	TexMetadata metadata{};
	ScratchImage scratchImg{};



	wchar_t wfxFilePath[256] = { L"" };
	mbstowcs(wfxFilePath, mTexName.c_str(), 256);

	hr = LoadFromWICFile(
		wfxFilePath,
		WIC_FLAGS_NONE,
		&metadata, scratchImg);

	assert(SUCCEEDED(hr));

	const Image* img = scratchImg.GetImage(0, 0, 0); // ���f�[�^���o
	// ���\�[�X�ݒ�
	CD3DX12_RESOURCE_DESC texresDesc = CD3DX12_RESOURCE_DESC::Tex2D(
		metadata.format,
		metadata.width,
		static_cast<UINT>(metadata.height),
		static_cast<UINT16>(metadata.arraySize),
		static_cast<UINT16>(metadata.mipLevels)
	);

	// �e�N�X�`���p�o�b�t�@�̐���
	hr = mDev->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_CPU_PAGE_PROPERTY_WRITE_BACK, D3D12_MEMORY_POOL_L0),
		D3D12_HEAP_FLAG_NONE,
		&texresDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ, // �e�N�X�`���p�w��
		nullptr,
		IID_PPV_ARGS(&mTexBuff));

	assert(SUCCEEDED(hr));

	// �e�N�X�`���o�b�t�@�Ƀf�[�^�]��
	hr = mTexBuff->WriteToSubresource(
		0,
		nullptr, // �S�̈�փR�s�[
		img->pixels,    // ���f�[�^�A�h���X
		static_cast<UINT>(img->rowPitch),  // 1���C���T�C�Y
		static_cast<UINT>(img->slicePitch) // 1���T�C�Y
	);

	assert(SUCCEEDED(hr));

	//�V�F�[�_���\�[�X�r���[�����蓖��
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
	D3D12_RESOURCE_DESC resDesc = mTexBuff->GetDesc();

	srvDesc.Format = resDesc.Format;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;//2D�e�N�X�`��
	srvDesc.Texture2D.MipLevels = 1;
	mDev->CreateShaderResourceView(
		mTexBuff,
		&srvDesc,
		cpuHandle
	);


	return hr;
}

ID3D12RootSignature* Compute::mEmitterRoot = nullptr;
ID3D12PipelineState* Compute::mEmitterPipe = nullptr;
ID3D12RootSignature* Compute::mParticleRoot = nullptr;
ID3D12PipelineState* Compute::mParticlePipe = nullptr;
ID3D12RootSignature* Compute::mParticleDrawRoot = nullptr;
ID3D12PipelineState* Compute::mParticleDrawPipe = nullptr;