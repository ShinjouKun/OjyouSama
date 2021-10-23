#include"DirectXManager.h"
#include"PipeLine.h"
//#include"Vector2.h"
//#include"Vector3.h"
#include "../Math/Vector2.h"
#include "../Math/Vector3.h"

std::vector<ComPtr<ID3D12Resource>>backBuffers(2);
float clearColor[] = { 0,0,0,0 };
DirectXManager::DirectXManager()
{

}

DirectXManager::~DirectXManager()
{
	ImGui_ImplDX12_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}

void DirectXManager::Init(Window* window)
{
#pragma region  �A�_�v�^�[�ƃf�o�C�X�̐���
	//�O���t�B�b�N�{�[�h�̃A�_�v�^���
	//DXGI�t�@�N�g���[����
	result = CreateDXGIFactory1(IID_PPV_ARGS(&dxgiFactory));
	//�A�_�v�^�[�̗񋓗p
	std::vector<IDXGIAdapter*>adapters;
	//�����ɓ���̖��O�����A�_�v�^�[�I�u�W�F�N�g������
	IDXGIAdapter* tmpAdapter = nullptr;//����ComPtr����
	for (int i = 0;
		dxgiFactory->EnumAdapters(i, &tmpAdapter) != DXGI_ERROR_NOT_FOUND; i++)
	{
		adapters.push_back(tmpAdapter);//���I�z��ɒǉ�����
	}
	for (int i = 0; i < adapters.size(); i++)
	{
		DXGI_ADAPTER_DESC adesc{};
		adapters[i]->GetDesc(&adesc);//�A�_�v�^�[��񂩂��Ƃ�
		std::wstring strDesc = adesc.Description;//�A�_�v�^�[��
		//Microsoft Basic render Driver�����
		if (strDesc.find(L"Microsoft") == std::wstring::npos)
		{
			tmpAdapter = adapters[i];//�̗p
			break;
		}
	}

	//�f�o�C�X����
	//�Ή������F���̔z��
	D3D_FEATURE_LEVEL levels[] =
	{
		D3D_FEATURE_LEVEL_12_1,
		D3D_FEATURE_LEVEL_12_0,
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0,
	};
	D3D_FEATURE_LEVEL featureLevel;

	for (int i = 0; i < _countof(levels); i++)
	{
		//�̗p�����A�_�v�^�[�Ńf�o�C�X���쐬
		result = D3D12CreateDevice(tmpAdapter, levels[i], IID_PPV_ARGS(&dev));
		if (result == S_OK)
		{
			//�f�o�C�X�𐶐��o�������_�Ń��[�v�𔲂���
			featureLevel = levels[i];
			break;
		}
	}
#pragma endregion

#pragma region �R�}���h���X�g����
	//�R�}���h�A���P�[�^�̍쐬
	result = dev->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT,
		IID_PPV_ARGS(&cmdAllocator));
	//�R�}���h���X�g����
	result = dev->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT,
		cmdAllocator.Get(), nullptr, IID_PPV_ARGS(&cmdList));
	//�R�}���h�L���[����
	D3D12_COMMAND_QUEUE_DESC cmdQueueDesc{};
	dev->CreateCommandQueue(&cmdQueueDesc, IID_PPV_ARGS(&cmdQueue));

#pragma endregion
#pragma region �X���b�v�`�F�[���ƃ����_�[�^�[�Q�b�g

	//�f�X�N���v�^�q�[�v
	descHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	descHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;//�V�F�[�_���猩����
	descHeapDesc.NumDescriptors = 3;
	//��������
	result = dev->CreateDescriptorHeap(&descHeapDesc, IID_PPV_ARGS(&basicDescHeap));

	//�萔�o�b�t�@�쐬
	D3D12_HEAP_PROPERTIES cbheapprop{};//�q�[�v�ݒ�
	cbheapprop.Type = D3D12_HEAP_TYPE_UPLOAD;//GPU�ւ̓]���p
	D3D12_RESOURCE_DESC cbresdesc{};//���\�[�X�ݒ�
	cbresdesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	cbresdesc.Width = (sizeof(ConstBufferData) + 0xff)&~0xff;//256�o�C�g�A���C�����g
	cbresdesc.Height = 1;
	cbresdesc.DepthOrArraySize = 1;
	cbresdesc.MipLevels = 1;
	cbresdesc.SampleDesc.Count = 1;
	cbresdesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	//GPU���\�[�X�̐���

	//���
	result = dev->CreateCommittedResource(
		&cbheapprop,//�q�[�v�ݒ�
		D3D12_HEAP_FLAG_NONE,
		&cbresdesc,//���\�[�X�ݒ�
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBuff));
	//�萔�o�b�t�@�Ƀf�[�^��]������

	result = constBuff->Map(0, nullptr, (void**)&constMap);//�}�b�s���O
	constMap->color = XMFLOAT4(0, 0, 1, 0.4f);

	//�[�x�l�p�q�[�v�v���p�e�B
	//�[�x�o�b�t�@
	CD3DX12_RESOURCE_DESC depthResDesc = CD3DX12_RESOURCE_DESC::Tex2D(
		DXGI_FORMAT_D32_FLOAT,
		Window::Window_Width,
		Window::Window_Height,
		1, 0,
		1, 0,
		D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL);

	//�[�x���\�[�X�ݒ�

	result = dev->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
		D3D12_HEAP_FLAG_NONE,
		&depthResDesc,
		D3D12_RESOURCE_STATE_DEPTH_WRITE,//�[�x�l�������݂Ɏg��
		&CD3DX12_CLEAR_VALUE(DXGI_FORMAT_D32_FLOAT, 1.0f, 0),
		IID_PPV_ARGS(&depthBuffer)
	);
	//�[�x�r���[�p�f�X�N���v�^�q�[�v����
	D3D12_DESCRIPTOR_HEAP_DESC dsvHeapDesc{};
	dsvHeapDesc.NumDescriptors = 1;//�[�x�r���[�͈��
	dsvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;//�f�v�X�X�e���V���r���[

	result = dev->CreateDescriptorHeap(&dsvHeapDesc, IID_PPV_ARGS(&dsvHeap));
	//�[�x�r���[����
	D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
	dsvDesc.Format = DXGI_FORMAT_D32_FLOAT;//�[�x�l�t�H�[�}�b�g
	dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
	dev->CreateDepthStencilView(depthBuffer.Get(), &dsvDesc, dsvHeap->GetCPUDescriptorHandleForHeapStart());
	//#pragma endregion
		//�X���b�v�`�F�[���̍쐬
	ComPtr<IDXGISwapChain1>swapchain1;
	DXGI_SWAP_CHAIN_DESC1 swapChainDesc{};
	swapChainDesc.Width = Window::Window_Width;
	swapChainDesc.Height = Window::Window_Height;
	swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;//�F���̏���
	swapChainDesc.SampleDesc.Count = 1;//�}���`�T���v�����Ȃ�
	swapChainDesc.BufferUsage = DXGI_USAGE_BACK_BUFFER;//�o�b�N�o�b�t�@�p
	swapChainDesc.BufferCount = 2;//�o�b�t�@�����Q�ɐݒ�
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;//�t���b�v��͔j��
	swapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
	result = dxgiFactory->CreateSwapChainForHwnd(
		cmdQueue.Get(),
		window->GetHWND(),
		&swapChainDesc,
		nullptr,
		nullptr,
		(IDXGISwapChain1**)&swapchain1);

	swapchain1.As(&swapChain);

	//�e��ݒ�����ăf�B�X�N���v�^�q�[�v���쐬

	D3D12_DESCRIPTOR_HEAP_DESC heapDesc{};
	heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	heapDesc.NumDescriptors = 3;//6_5
	dev->CreateDescriptorHeap(&heapDesc, IID_PPV_ARGS(&rtvHeaps));
	//�\���̓���ɂ���


	for (int i = 0; i < 2; i++)
	{
		//�X���b�v�`�F�[������o�b�t�@���l��
		result = swapChain->GetBuffer(i, IID_PPV_ARGS(&backBuffers[i]));
		//�f�B�X�N���v�^�q�[�v�̃n���h�����l��
		CD3DX12_CPU_DESCRIPTOR_HANDLE handle =
			CD3DX12_CPU_DESCRIPTOR_HANDLE(rtvHeaps->GetCPUDescriptorHandleForHeapStart(),
				i, dev->GetDescriptorHandleIncrementSize(heapDesc.Type));
		//�\�������ŃA�h���X�������

		//�����_�[�^�[�Q�b�g�r���[�̐���
		dev->CreateRenderTargetView(
			backBuffers[i].Get(),
			nullptr,
			handle);
	}


#pragma endregion

#pragma region �t�F���X����
	result = dev->CreateFence(fenceVal, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&fence));
#pragma endregion
#pragma region imgui�n��
	imguiHeap = Createimgui();
	if (ImGui::CreateContext() == nullptr)
	{
		assert(0);
	}
	bool blnResult = ImGui_ImplWin32_Init(window->GetHWND());
	if (!blnResult)
	{
		assert(0);
	}
	blnResult = ImGui_ImplDX12_Init(
		Dev(),
		3,
		DXGI_FORMAT_R8G8B8A8_UNORM,//�������ݗpRTV�t�H�[�}�b�g
		Getimgui().Get(),
		Getimgui()->GetCPUDescriptorHandleForHeapStart(),
		Getimgui()->GetGPUDescriptorHandleForHeapStart()
	);
#pragma endregion
	pipeLine = make_shared<PipeLine>();//�p�C�v���C���N���X����

	//�|�X�g�G�t�F�N�g�p
	auto& bBuff = backBuffers[0];
	auto postResDesc = bBuff->GetDesc();

	D3D12_HEAP_PROPERTIES  postHeapProp = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);
	//�����_�����O���̃N���A�̐F�Ɠ���
	D3D12_CLEAR_VALUE clearValue = CD3DX12_CLEAR_VALUE(DXGI_FORMAT_R8G8B8A8_UNORM, clearColor);
	//���\�[�X�̐���
	for (auto& res : postEffctResouce)
	{
		result = dev->CreateCommittedResource(
			&postHeapProp,
			D3D12_HEAP_FLAG_NONE,
			&postResDesc,
			D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
			&clearValue,
			IID_PPV_ARGS(res.ReleaseAndGetAddressOf()));
	}
	//�쐬�σq�[�v�𗘗p�������ꖇ
	auto postHeapDesc = rtvHeaps->GetDesc();
	postHeapDesc.NumDescriptors = 2;
	result = dev->CreateDescriptorHeap(
		&postHeapDesc,
		IID_PPV_ARGS(postEffctRTVheap.ReleaseAndGetAddressOf()));
	D3D12_RENDER_TARGET_VIEW_DESC rtvDesc = {};
	rtvDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;
	rtvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	//�����_�[�^�[�Q�b�g�r���[�쐬
	auto handle = postEffctRTVheap->GetCPUDescriptorHandleForHeapStart();
	for (auto& res : postEffctResouce)
	{
		dev->CreateRenderTargetView(
			res.Get(),
			&rtvDesc,
			handle);
		handle.ptr += dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
	}
	//�V�F�[�_�[���\�[�X�r���[�쐬
	postHeapDesc.NumDescriptors = 2;
	postHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	postHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	//SRV
	result = dev->CreateDescriptorHeap(&postHeapDesc, IID_PPV_ARGS(postEffctSRVheap.ReleaseAndGetAddressOf()));
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Format = rtvDesc.Format;
	srvDesc.Texture2D.MipLevels = 1;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	handle = postEffctSRVheap->GetCPUDescriptorHandleForHeapStart();

	for (auto& res : postEffctResouce)
	{
		dev->CreateShaderResourceView(
			res.Get(),
			&srvDesc,
			handle
		);
		handle.ptr += dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	}
	PostEffctBuff();//�Ăяo��
}

void DirectXManager::Begin()
{
	
#pragma region ���\�[�X�o���A�̕ύX���A
	//���\�[�X�o���A�̕ύX�A���A
	bbIndex = swapChain->GetCurrentBackBufferIndex();
	
	//���\�[�X�o���A�ύX
	cmdList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(backBuffers[bbIndex].Get(),
		D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET));
#pragma endregion

#pragma region ��ʃN���A�R�}���h

	D3D12_DESCRIPTOR_HEAP_DESC descHeapDesc{};
	//�����_�[�^�[�Q�b�g�r���[�p�f�B�X�N���v�^�q�[�v���擾
	D3D12_CPU_DESCRIPTOR_HANDLE rtvH =
		rtvHeaps->GetCPUDescriptorHandleForHeapStart();
	rtvH.ptr += bbIndex * dev->GetDescriptorHandleIncrementSize(descHeapDesc.Type);

	D3D12_CPU_DESCRIPTOR_HANDLE dsvH = dsvHeap->GetCPUDescriptorHandleForHeapStart();
	
	cmdList->OMSetRenderTargets(1, &rtvH, false, &dsvH);
	//�S��ʃN���ARGBA
	clearColor[0] = r;//�u���[�o�b�N
	clearColor[1] = g;//�u���[�o�b�N
	clearColor[2] = b;//�u���[�o�b�N
	clearColor[3] = a;//�u���[�o�b�N

	cmdList->ClearRenderTargetView(rtvH, clearColor, 0, nullptr);
	cmdList->ClearDepthStencilView(dsvH, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);

	//�r���[�|�[�g�A�V�U�[��`
	cmdList->RSSetViewports(1, &CD3DX12_VIEWPORT(0.0f, 0.0f, Window::Window_Width, Window::Window_Height));
	cmdList->RSSetScissorRects(1, &CD3DX12_RECT(0, 0, Window::Window_Width, Window::Window_Height));
#pragma endregion
}

void DirectXManager::End()
{
	
	//���\�[�X�o���A��߂�
	cmdList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(backBuffers[bbIndex].Get(),
		D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT));
	//���߂̃N���[�Y
	cmdList->Close();
	//�R�}���h���X�g���s
	ID3D12CommandList*cmdLists[] = { cmdList.Get() };//�R�}���h���X�g�̔z��
	cmdQueue->ExecuteCommandLists(1, cmdLists);
	//�R�}���h���X�g�̎��s�������܂�
	cmdQueue->Signal(fence.Get(), ++fenceVal);
	if (fence->GetCompletedValue() != fenceVal)
	{
		HANDLE event = CreateEvent(nullptr, false, false, nullptr);
		fence->SetEventOnCompletion(fenceVal, event);
		WaitForSingleObject(event, INFINITE);
		CloseHandle(event);
	}

	cmdAllocator->Reset();	//�L���[���N���A
	cmdList->Reset(cmdAllocator.Get(), nullptr);//�ĂуR�}���h���X�g�����߂鏀��
	//�o�b�t�@�t���b�v
	swapChain->Present(1, 0);
}

void DirectXManager::SetData3D()
{
	//�p�C�v���C���ݒ�
	cmdList->SetPipelineState(pipeLine->GetPipeLineState("unti"));
	//���[�g�V�O�l�X�`������
	cmdList->SetGraphicsRootSignature(pipeLine->GetRootSignature("unti"));
	//�v���~�e�B�u�`��
	cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	
}

void DirectXManager::SetData2D()
{
	//�p�C�v���C���ݒ�
	cmdList->SetPipelineState(pipeLine->GetPipeLineState("unti2d"));
	//���[�g�V�O�l�X�`������
	cmdList->SetGraphicsRootSignature(pipeLine->GetRootSignature("unti2d"));
	//�v���~�e�B�u�`��
	cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
}

void DirectXManager::SetDataParticle()
{
	//�p�C�v���C���ݒ�
	cmdList->SetPipelineState(pipeLine->GetPipeLineState("untiP"));
	//���[�g�V�O�l�X�`������
	cmdList->SetGraphicsRootSignature(pipeLine->GetRootSignature("untiP"));
	//�v���~�e�B�u�`��
	cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_POINTLIST);
}

DirectXManager * DirectXManager::GetInstance()
{
	static DirectXManager instance;
	return &instance;
}

ID3D12Device * DirectXManager::Dev() const
{
	return dev.Get();
}

ID3D12GraphicsCommandList * DirectXManager::CmdList() const
{
	return cmdList.Get();
}

long long DirectXManager::currentTime()
{
	std::chrono::system_clock::duration d = std::chrono::system_clock::now().time_since_epoch();
	return std::chrono::duration_cast<std::chrono::milliseconds>(d).count();
}

void DirectXManager::UpdateFPS()
{
	long long end = currentTime();
	double fpsResult = (double)(1000) / (end - time)*cnt;
	time = end;
	cnt = 0;
}

void DirectXManager::Update()
{
	cnt++;
	if (limit <= cnt)
	{
		UpdateFPS();//�K��t���[�����ɂȂ�����X�V
	}
}

void DirectXManager::PostEffctBuff()
{
	ComPtr<ID3DBlob>vsBlob;//���_�V�F�[�_�I�u�W�F�N�g
	ComPtr<ID3DBlob>psBlob;//PS�V�F�[�_�I�u�W�F�N�g
	ComPtr<ID3DBlob>errorBlob;//eraaaaaaaa�V�F�[�_�I�u�W�F�N�g
	D3D12_INPUT_ELEMENT_DESC layout[] =
	{
		 {
			"POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT,0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0
		 },
		 {
			 "TEXCOORD",0,DXGI_FORMAT_R32G32_FLOAT,0,
			  D3D12_APPEND_ALIGNED_ELEMENT,
			  D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0
		 },
	};
	D3D12_GRAPHICS_PIPELINE_STATE_DESC gpsDesc = {};
	//VS
	result = D3DCompileFromFile(
		L"Shader/PostEffectVertexShader.hlsl",//�V�F�[�_�[�t�@�C����
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,//�C���N���[�h�\�ɂ���
		"VSmain", "vs_5_0",//�G���g���[�|�C���g��,�V�F�[�_�[���f���w��
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,//�f�o�b�O�p�w��
		0,
		&vsBlob, &errorBlob
	);
	//errorBlob����G���[���󂯎��string�ɂ���
	if (FAILED(result))
	{
		std::string errstr;
		errstr.resize(errorBlob->GetBufferSize());
		std::copy_n((char*)errorBlob->GetBufferPointer(),
			errorBlob->GetBufferSize(),
			errstr.begin());
		errstr += "\n";
		//�G���[���e���o�̓E�B���h�E�ɕ\��
		OutputDebugStringA(errstr.c_str());
		exit(1);
	}
	//PS
	result = D3DCompileFromFile(
		L"Shader/PostEffectPixelShader.hlsl",//�V�F�[�_�[�t�@�C����
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,//�C���N���[�h�\�ɂ���
		"PSmain", "ps_5_0",//�G���g���[�|�C���g��,�V�F�[�_�[���f���w��
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,//�f�o�b�O�p�w��
		0,
		&psBlob, &errorBlob
	);

	//errorBlob����G���[���󂯎��string�ɂ���
	if (FAILED(result))
	{
		std::string errstr;
		errstr.resize(errorBlob->GetBufferSize());
		std::copy_n((char*)errorBlob->GetBufferPointer(),
			errorBlob->GetBufferSize(),
			errstr.begin());
		errstr += "\n";
		//�G���[���e���o�̓E�B���h�E�ɕ\��
		OutputDebugStringA(errstr.c_str());
		exit(1);
	}
	gpsDesc.VS = CD3DX12_SHADER_BYTECODE(vsBlob.Get());
	gpsDesc.PS = CD3DX12_SHADER_BYTECODE(psBlob.Get());
	gpsDesc.DepthStencilState.DepthEnable = false;
	gpsDesc.DepthStencilState.StencilEnable = false;

	gpsDesc.InputLayout.NumElements = _countof(layout);
	gpsDesc.InputLayout.pInputElementDescs = layout;

	gpsDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
	gpsDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	gpsDesc.NumRenderTargets = 1;
	gpsDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
	gpsDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	gpsDesc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;
	gpsDesc.SampleDesc.Count = 1;
	gpsDesc.SampleDesc.Quality = 0;
	gpsDesc.Flags = D3D12_PIPELINE_STATE_FLAG_NONE;

	D3D12_DESCRIPTOR_RANGE range[2] = {};
	range[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_CBV;
	range[0].BaseShaderRegister = 0;
	range[0].NumDescriptors = 1;

	range[1].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	range[1].BaseShaderRegister = 0;
	range[1].NumDescriptors = 1;

	D3D12_ROOT_PARAMETER rp[2] = {};
	rp[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	rp[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
	rp[0].DescriptorTable.pDescriptorRanges = &range[0];
	rp[0].DescriptorTable.NumDescriptorRanges = 1;

	rp[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	rp[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
	rp[1].DescriptorTable.pDescriptorRanges = &range[1];
	rp[1].DescriptorTable.NumDescriptorRanges = 1;

	D3D12_ROOT_SIGNATURE_DESC rsDesc = {};
	rsDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

	D3D12_STATIC_SAMPLER_DESC sampDesc = CD3DX12_STATIC_SAMPLER_DESC(0);

	rsDesc.pParameters = rp;
	rsDesc.NumParameters = 2;
	rsDesc.NumStaticSamplers = 1;
	rsDesc.pStaticSamplers = &sampDesc;

	ComPtr<ID3DBlob>rsBlob;

	result = D3D12SerializeRootSignature(
		&rsDesc,
		D3D_ROOT_SIGNATURE_VERSION_1_0,
		rsBlob.ReleaseAndGetAddressOf(),
		errorBlob.ReleaseAndGetAddressOf()
	);

	assert(SUCCEEDED(result));

	result = dev->CreateRootSignature(
		0,
		rsBlob->GetBufferPointer(),
		rsBlob->GetBufferSize(),
		IID_PPV_ARGS(postEffctRootSignature.ReleaseAndGetAddressOf()));

	assert(SUCCEEDED(result));

	gpsDesc.pRootSignature = postEffctRootSignature.Get();

	result = dev->CreateGraphicsPipelineState(&gpsDesc, IID_PPV_ARGS(postEffctPipeLine.ReleaseAndGetAddressOf()));

	assert(SUCCEEDED(result));

	struct Vertexes
	{
		Vector3 pos;
		Vector2 uv;
	};
	Vertexes pv[4] =
	{
	 {
		{-1,-1,0},{0,1}
	 },
	 {
		{-1,1,0},{0,0}
	 },
	 {
		{1,-1,0},{1,1}
	 },
	 {
		{1,1,0},{1,0}
	 }
	};

	result = dev->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(sizeof(pv)),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(postEffctVB.ReleaseAndGetAddressOf()));

	assert(SUCCEEDED(result));

	Vertexes* mappedPost = nullptr;
	postEffctVB->Map(0, nullptr, (void**)&mappedPost);
	memcpy(mappedPost, pv, sizeof(pv));
	postEffctVB->Unmap(0, nullptr);

	postEffctVBV.BufferLocation = postEffctVB->GetGPUVirtualAddress();
	postEffctVBV.SizeInBytes = sizeof(pv);
	postEffctVBV.StrideInBytes = sizeof(Vertexes);

	D3D12_HEAP_PROPERTIES heapProp = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);

}

void DirectXManager::SetDrawComnd()
{
	//�X�v���C�g�`��R�}���h
	ID3D12DescriptorHeap* ppHeaps[] = { pipeLine->GetDescripterHeap() };
	cmdList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);
}

void DirectXManager::PostEffctBegin()
{

	////�r���[�|�[�g�A�V�U�[��`
	cmdList->RSSetViewports(1, &CD3DX12_VIEWPORT(0.0f, 0.0f, Window::Window_Width, Window::Window_Height));
	cmdList->RSSetScissorRects(1, &CD3DX12_RECT(0, 0, Window::Window_Width, Window::Window_Height));

	//���\�[�X�o���A�ύX
	cmdList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(postEffctResouce[0].Get(),
		D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE, D3D12_RESOURCE_STATE_RENDER_TARGET));
	////���\�[�X�o���A�ύX
	cmdList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(postEffctResouce[1].Get(),
		D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE, D3D12_RESOURCE_STATE_RENDER_TARGET));
	

	CD3DX12_CPU_DESCRIPTOR_HANDLE handles[2];
	D3D12_CPU_DESCRIPTOR_HANDLE baseH = postEffctRTVheap->GetCPUDescriptorHandleForHeapStart();
	auto incSize = dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

	uint32_t offset = 0;
	for (auto& handles : handles)
	{
		handles.InitOffsetted(baseH, offset);
		offset += incSize;
	}

	cmdList->OMSetRenderTargets(_countof(handles), handles, false, &dsvHeap->GetCPUDescriptorHandleForHeapStart());

	for (int i = 0, size = _countof(handles); i < size; i++)
	{
		cmdList->ClearRenderTargetView(handles[i], clearColor, 0, nullptr);//����������
	}

	////imgui�֘A
	//ImGui_ImplDX12_NewFrame();
	//ImGui_ImplWin32_NewFrame();
	//ImGui::NewFrame();
	//ImGui::Begin("Debug Window 1");
	//ImGui::SetWindowSize(ImVec2(400, 500), ImGuiCond_::ImGuiCond_FirstUseEver);
}

void DirectXManager::PostEffctEnd()
{
	//ImGui::End();
	//ImGui::Render();
	//ID3D12DescriptorHeap* ppHeaps[] = { imguiHeap.Get() };
	//cmdList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);
	//ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), cmdList.Get());
	//���\�[�X�o���A��߂�
	cmdList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(postEffctResouce[0].Get(),
		D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE));
	////���\�[�X�o���A��߂�
	cmdList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(postEffctResouce[1].Get(),
		D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE));
}

void DirectXManager::PostEffctDraw()
{
	cmdList->SetGraphicsRootSignature(postEffctRootSignature.Get());
	cmdList->SetPipelineState(postEffctPipeLine.Get());

	cmdList->SetDescriptorHeaps(1, postEffctSRVheap.GetAddressOf());
	auto handle = postEffctSRVheap->GetGPUDescriptorHandleForHeapStart();
	cmdList->SetGraphicsRootDescriptorTable(1, handle);

	cmdList->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	cmdList->IASetVertexBuffers(0, 1, &postEffctVBV);
	cmdList->DrawInstanced(4, 1, 0, 0);
}

ComPtr<ID3D12DescriptorHeap> DirectXManager::Createimgui()
{
	ComPtr<ID3D12DescriptorHeap> ret;
	D3D12_DESCRIPTOR_HEAP_DESC desc = {};
	desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	desc.NodeMask = 0;
	desc.NumDescriptors = 1;
	desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	dev->CreateDescriptorHeap(&desc, IID_PPV_ARGS(ret.ReleaseAndGetAddressOf()));
	return ret;
}

ComPtr<ID3D12DescriptorHeap> DirectXManager::Getimgui()
{
	return imguiHeap;
}

