#include "Compute.h"
#include "Shader.h"
#include "StructuredBuffer/StructuredBuffer.h"
#include "RWStructuredBuffer/RWStructuredBuffer.h"
#include "../ParticleData/ParticleData.h"

#include "../../Device/DirectXManager.h"

Compute::Compute()
{
}

Compute::~Compute()
{
	finalize();
}

void Compute::finalize()
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
	mEmitterRoot->Release();
	mEmitterPipe->Release();
	mParticleRoot->Release();
	mParticlePipe->Release();
	mParticleDrawRoot->Release();
	mParticleDrawPipe->Release();
}

void Compute::init()
{
	mDev = DirectXManager::GetInstance()->Dev();

	createHeap();
	createCMDList();
	createPiprLine();
	createBuffer();


	//最後
	/*
	mInputSB = new StructuredBuffer();
	mInputSB->setDevice(dev);
	mInputSB->init(sizeof(TestDatasState), mTestDataList.size(), mTestDataList.data());

	mOutputSB = new RWStructuredBuffer();
	mOutputSB->setDevice(dev);
	mOutputSB->init(sizeof(TestDatasState), mTestDataList.size(), nullptr);

	auto cpuHandle = mHeap->GetCPUDescriptorHandleForHeapStart();
	auto handle = DX12Manager::instance().getDev()->getDescriptorHandleIncrementSize();

	mInputSB->RegistShaderResourceView(cpuHandle, 0);
	cpuHandle.ptr += handle;

	mOutputSB->RegistUnorderAccessView(cpuHandle, 0);
	cpuHandle.ptr += handle;
	*/
}

void Compute::emitterUpdate(int dispatch)
{
	//バリア設定
	barrier(mInputEmitterSB->getResource(), D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_NON_PIXEL_SHADER_RESOURCE);
	barrier(mOutputEmitterSB->getResource(), D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_NON_PIXEL_SHADER_RESOURCE);

	mCMDList->Dispatch(dispatch, 1, 1);
	
	//バリアを戻す
	barrier(mOutputEmitterSB->getResource(), D3D12_RESOURCE_STATE_NON_PIXEL_SHADER_RESOURCE, D3D12_RESOURCE_STATE_COPY_DEST);
	barrier(mInputEmitterSB->getResource(), D3D12_RESOURCE_STATE_NON_PIXEL_SHADER_RESOURCE, D3D12_RESOURCE_STATE_COPY_DEST);

}

void Compute::particleUpdate()
{
	barrier(mInputParticleDataSB->getResource(), D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_NON_PIXEL_SHADER_RESOURCE);
	barrier(mOutputParticleSB->getResource(), D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_NON_PIXEL_SHADER_RESOURCE);
	barrier(mParticleStorageSB->getResource(), D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_NON_PIXEL_SHADER_RESOURCE);

	auto data1 = mParticleStorageSB->getResourceOnCPU();
	auto data2 = mOutputEmitterSB->getResourceOnCPU();
	std::vector<ParticleData> v1;
	int x = 0;
	v1.assign((ParticleData*)data1, (ParticleData*)data1 + x);

	std::vector<ParticleData> v2;
	v2.assign((ParticleData*)data2, (ParticleData*)data2 + x);

	std::copy(v2.begin(), v2.end(), std::back_inserter(v1));
	
	//エミッターアウトプットからデータを受取る
	mInputParticleDataSB->update(v1.data());

	mCMDList->Dispatch(v1.size(), 1, 1);


	barrier(mInputParticleDataSB->getResource(), D3D12_RESOURCE_STATE_NON_PIXEL_SHADER_RESOURCE, D3D12_RESOURCE_STATE_COPY_DEST);
	barrier(mOutputParticleSB->getResource(), D3D12_RESOURCE_STATE_NON_PIXEL_SHADER_RESOURCE, D3D12_RESOURCE_STATE_COPY_DEST);
	barrier(mParticleStorageSB->getResource(), D3D12_RESOURCE_STATE_NON_PIXEL_SHADER_RESOURCE, D3D12_RESOURCE_STATE_COPY_DEST);

}

void Compute::particleDraw()
{
	auto data = mOutputParticleSB->getResourceOnCPU();
	std::vector<ParticleDrawData> v;
	v.assign((ParticleDrawData*)data, (ParticleDrawData*)data + 0);
	ParticleDrawData* constMap;
	mConstBuff->Map(0, nullptr, (void**)&constMap);
	constMap = (ParticleDrawData*)data;
	mConstBuff->Unmap(0, nullptr);

	//パイプラインをセット

	//ルートシグネチャをセット

	//プリミティブ形状をセット

	//頂点バッファをセット

	//デスクリプタヒープをセット

	//定数バッファビューをセット

	//シェーダリソースビューをセット

	//描画コマンド
	mCMDList->DrawInstanced(static_cast<UINT>(std::distance(v.begin(), v.end())), 1, 0, 0);
}

void Compute::barrier(ID3D12Resource * p, D3D12_RESOURCE_STATES before, D3D12_RESOURCE_STATES after)
{
	mCMDList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(p, before, after, 0));
}

HRESULT Compute::createCMDList()
{
	HRESULT hr;

	hr = mDev->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&mCMDAllo));
	hr = mDev->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, mCMDAllo, nullptr, IID_PPV_ARGS(&mCMDList));
	return hr;
}

HRESULT Compute::createHeap()
{
	//ヒープを生成
	D3D12_DESCRIPTOR_HEAP_DESC heapDesc{};
	heapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	heapDesc.NodeMask = 0;
	heapDesc.NumDescriptors = 512;//シェーダリソースビューの数
	heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;

	return mDev->CreateDescriptorHeap(&heapDesc, IID_PPV_ARGS(&mHeap));
}

HRESULT Compute::createPiprLine()
{
	HRESULT hr;
	//エミッター更新
	mCSShader = new Shader();
	mCSShader->loadCS("ComputeEmitterShader.hlsl");
	//パーティクル更新
	mParticleCSShader = new Shader();
	mParticleCSShader->loadCS("ComputeParticleUpdateShader.hlsl");
	//パーティクル描画
	mPSShader = new Shader();
	mPSShader->loadPS("ParticlePixelShader.hlsl", "");
	mGSShader = new Shader();
	mGSShader->loadGS("ParticleGeometryShader.hlsl", "");
	mVSShader = new Shader();
	mVSShader->loadVS("ParticleVertexShader.hlsl", "");

#pragma region エミッター更新用
	//ルートシグネチャ
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
	ComPtr<ID3DBlob> error;

	D3DX12SerializeVersionedRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1, &signature, &error);
	hr = mDev->CreateRootSignature(0, signature->GetBufferPointer(), signature->GetBufferSize(), IID_PPV_ARGS(&mEmitterRoot));

	assert(SUCCEEDED(hr));

	//パイプラインステート
	D3D12_COMPUTE_PIPELINE_STATE_DESC psoDesc{};
	psoDesc.pRootSignature = mEmitterRoot;
	psoDesc.CS = CD3DX12_SHADER_BYTECODE(mCSShader->getCompiledBlob());
	psoDesc.Flags = D3D12_PIPELINE_STATE_FLAG_NONE;
	psoDesc.NodeMask = 0;

	hr = mDev->CreateComputePipelineState(&psoDesc, IID_PPV_ARGS(&mEmitterPipe));

	assert(SUCCEEDED(hr));

#pragma endregion //エミッター更新用

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
#pragma region パーティクル更新用


#pragma endregion //パーティクル更新用

#pragma region パーティクル描画用


#pragma endregion //パーティクル描画用


	return hr;
}

HRESULT Compute::createBuffer()
{
	//HRESULT hr;

	auto cpuHandle = mHeap->GetCPUDescriptorHandleForHeapStart();
	auto handle = mDev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

	//エミッターインプット
	mInputEmitterSB = new StructuredBuffer();

	cpuHandle.ptr += handle;
	//エミッターアウトプット
	mOutputEmitterSB = new RWStructuredBuffer();

	cpuHandle.ptr += handle;
	//パーティクルインプット
	mInputParticleDataSB = new StructuredBuffer();

	cpuHandle.ptr += handle;
	//パーティクル描画用
	mOutputParticleSB = new RWStructuredBuffer();

	cpuHandle.ptr += handle;
	//パーティクル保管用
	mParticleStorageSB = new RWStructuredBuffer();

	cpuHandle.ptr += handle;


	return S_OK;
}