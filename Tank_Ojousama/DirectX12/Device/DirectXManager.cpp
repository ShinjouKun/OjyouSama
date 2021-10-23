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
#pragma region  アダプターとデバイスの生成
	//グラフィックボードのアダプタを列挙
	//DXGIファクトリー生成
	result = CreateDXGIFactory1(IID_PPV_ARGS(&dxgiFactory));
	//アダプターの列挙用
	std::vector<IDXGIAdapter*>adapters;
	//ここに特定の名前を持つアダプターオブジェクトが入る
	IDXGIAdapter* tmpAdapter = nullptr;//これComPtrだめ
	for (int i = 0;
		dxgiFactory->EnumAdapters(i, &tmpAdapter) != DXGI_ERROR_NOT_FOUND; i++)
	{
		adapters.push_back(tmpAdapter);//動的配列に追加する
	}
	for (int i = 0; i < adapters.size(); i++)
	{
		DXGI_ADAPTER_DESC adesc{};
		adapters[i]->GetDesc(&adesc);//アダプター情報かくとく
		std::wstring strDesc = adesc.Description;//アダプター名
		//Microsoft Basic render Driverを回避
		if (strDesc.find(L"Microsoft") == std::wstring::npos)
		{
			tmpAdapter = adapters[i];//採用
			break;
		}
	}

	//デバイス生成
	//対応レヴェルの配列
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
		//採用したアダプターでデバイスを作成
		result = D3D12CreateDevice(tmpAdapter, levels[i], IID_PPV_ARGS(&dev));
		if (result == S_OK)
		{
			//デバイスを生成出来た時点でループを抜ける
			featureLevel = levels[i];
			break;
		}
	}
#pragma endregion

#pragma region コマンドリスト生成
	//コマンドアロケータの作成
	result = dev->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT,
		IID_PPV_ARGS(&cmdAllocator));
	//コマンドリスト生成
	result = dev->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT,
		cmdAllocator.Get(), nullptr, IID_PPV_ARGS(&cmdList));
	//コマンドキュー生成
	D3D12_COMMAND_QUEUE_DESC cmdQueueDesc{};
	dev->CreateCommandQueue(&cmdQueueDesc, IID_PPV_ARGS(&cmdQueue));

#pragma endregion
#pragma region スワップチェーンとレンダーターゲット

	//デスクリプタヒープ
	descHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	descHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;//シェーダから見える
	descHeapDesc.NumDescriptors = 3;
	//生成する
	result = dev->CreateDescriptorHeap(&descHeapDesc, IID_PPV_ARGS(&basicDescHeap));

	//定数バッファ作成
	D3D12_HEAP_PROPERTIES cbheapprop{};//ヒープ設定
	cbheapprop.Type = D3D12_HEAP_TYPE_UPLOAD;//GPUへの転送用
	D3D12_RESOURCE_DESC cbresdesc{};//リソース設定
	cbresdesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	cbresdesc.Width = (sizeof(ConstBufferData) + 0xff)&~0xff;//256バイトアライメント
	cbresdesc.Height = 1;
	cbresdesc.DepthOrArraySize = 1;
	cbresdesc.MipLevels = 1;
	cbresdesc.SampleDesc.Count = 1;
	cbresdesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	//GPUリソースの生成

	//一個目
	result = dev->CreateCommittedResource(
		&cbheapprop,//ヒープ設定
		D3D12_HEAP_FLAG_NONE,
		&cbresdesc,//リソース設定
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBuff));
	//定数バッファにデータを転送する

	result = constBuff->Map(0, nullptr, (void**)&constMap);//マッピング
	constMap->color = XMFLOAT4(0, 0, 1, 0.4f);

	//深度値用ヒーププロパティ
	//深度バッファ
	CD3DX12_RESOURCE_DESC depthResDesc = CD3DX12_RESOURCE_DESC::Tex2D(
		DXGI_FORMAT_D32_FLOAT,
		Window::Window_Width,
		Window::Window_Height,
		1, 0,
		1, 0,
		D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL);

	//深度リソース設定

	result = dev->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
		D3D12_HEAP_FLAG_NONE,
		&depthResDesc,
		D3D12_RESOURCE_STATE_DEPTH_WRITE,//深度値書き込みに使う
		&CD3DX12_CLEAR_VALUE(DXGI_FORMAT_D32_FLOAT, 1.0f, 0),
		IID_PPV_ARGS(&depthBuffer)
	);
	//深度ビュー用デスクリプタヒープ生成
	D3D12_DESCRIPTOR_HEAP_DESC dsvHeapDesc{};
	dsvHeapDesc.NumDescriptors = 1;//深度ビューは一つ
	dsvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;//デプスステンシルビュー

	result = dev->CreateDescriptorHeap(&dsvHeapDesc, IID_PPV_ARGS(&dsvHeap));
	//深度ビュー生成
	D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
	dsvDesc.Format = DXGI_FORMAT_D32_FLOAT;//深度値フォーマット
	dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
	dev->CreateDepthStencilView(depthBuffer.Get(), &dsvDesc, dsvHeap->GetCPUDescriptorHandleForHeapStart());
	//#pragma endregion
		//スワップチェーンの作成
	ComPtr<IDXGISwapChain1>swapchain1;
	DXGI_SWAP_CHAIN_DESC1 swapChainDesc{};
	swapChainDesc.Width = Window::Window_Width;
	swapChainDesc.Height = Window::Window_Height;
	swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;//色情報の書式
	swapChainDesc.SampleDesc.Count = 1;//マルチサンプルしない
	swapChainDesc.BufferUsage = DXGI_USAGE_BACK_BUFFER;//バックバッファ用
	swapChainDesc.BufferCount = 2;//バッファ数を２に設定
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;//フリップ後は破棄
	swapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
	result = dxgiFactory->CreateSwapChainForHwnd(
		cmdQueue.Get(),
		window->GetHWND(),
		&swapChainDesc,
		nullptr,
		nullptr,
		(IDXGISwapChain1**)&swapchain1);

	swapchain1.As(&swapChain);

	//各種設定をしてディスクリプタヒープを作成

	D3D12_DESCRIPTOR_HEAP_DESC heapDesc{};
	heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	heapDesc.NumDescriptors = 3;//6_5
	dev->CreateDescriptorHeap(&heapDesc, IID_PPV_ARGS(&rtvHeaps));
	//表裏の二つ分について


	for (int i = 0; i < 2; i++)
	{
		//スワップチェーンからバッファを獲得
		result = swapChain->GetBuffer(i, IID_PPV_ARGS(&backBuffers[i]));
		//ディスクリプタヒープのハンドルを獲得
		CD3DX12_CPU_DESCRIPTOR_HANDLE handle =
			CD3DX12_CPU_DESCRIPTOR_HANDLE(rtvHeaps->GetCPUDescriptorHandleForHeapStart(),
				i, dev->GetDescriptorHandleIncrementSize(heapDesc.Type));
		//表か裏かでアドレスがずれる

		//レンダーターゲットビューの生成
		dev->CreateRenderTargetView(
			backBuffers[i].Get(),
			nullptr,
			handle);
	}


#pragma endregion

#pragma region フェンス生成
	result = dev->CreateFence(fenceVal, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&fence));
#pragma endregion
#pragma region imgui系統
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
		DXGI_FORMAT_R8G8B8A8_UNORM,//書き込み用RTVフォーマット
		Getimgui().Get(),
		Getimgui()->GetCPUDescriptorHandleForHeapStart(),
		Getimgui()->GetGPUDescriptorHandleForHeapStart()
	);
#pragma endregion
	pipeLine = make_shared<PipeLine>();//パイプラインクラス生成

	//ポストエフェクト用
	auto& bBuff = backBuffers[0];
	auto postResDesc = bBuff->GetDesc();

	D3D12_HEAP_PROPERTIES  postHeapProp = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);
	//レンダリング時のクリアの色と同じ
	D3D12_CLEAR_VALUE clearValue = CD3DX12_CLEAR_VALUE(DXGI_FORMAT_R8G8B8A8_UNORM, clearColor);
	//リソースの生成
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
	//作成済ヒープを利用しもう一枚
	auto postHeapDesc = rtvHeaps->GetDesc();
	postHeapDesc.NumDescriptors = 2;
	result = dev->CreateDescriptorHeap(
		&postHeapDesc,
		IID_PPV_ARGS(postEffctRTVheap.ReleaseAndGetAddressOf()));
	D3D12_RENDER_TARGET_VIEW_DESC rtvDesc = {};
	rtvDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;
	rtvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	//レンダーターゲットビュー作成
	auto handle = postEffctRTVheap->GetCPUDescriptorHandleForHeapStart();
	for (auto& res : postEffctResouce)
	{
		dev->CreateRenderTargetView(
			res.Get(),
			&rtvDesc,
			handle);
		handle.ptr += dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
	}
	//シェーダーリソースビュー作成
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
	PostEffctBuff();//呼び出し
}

void DirectXManager::Begin()
{
	
#pragma region リソースバリアの変更復帰
	//リソースバリアの変更、復帰
	bbIndex = swapChain->GetCurrentBackBufferIndex();
	
	//リソースバリア変更
	cmdList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(backBuffers[bbIndex].Get(),
		D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET));
#pragma endregion

#pragma region 画面クリアコマンド

	D3D12_DESCRIPTOR_HEAP_DESC descHeapDesc{};
	//レンダーターゲットビュー用ディスクリプタヒープを取得
	D3D12_CPU_DESCRIPTOR_HANDLE rtvH =
		rtvHeaps->GetCPUDescriptorHandleForHeapStart();
	rtvH.ptr += bbIndex * dev->GetDescriptorHandleIncrementSize(descHeapDesc.Type);

	D3D12_CPU_DESCRIPTOR_HANDLE dsvH = dsvHeap->GetCPUDescriptorHandleForHeapStart();
	
	cmdList->OMSetRenderTargets(1, &rtvH, false, &dsvH);
	//全画面クリアRGBA
	clearColor[0] = r;//ブルーバック
	clearColor[1] = g;//ブルーバック
	clearColor[2] = b;//ブルーバック
	clearColor[3] = a;//ブルーバック

	cmdList->ClearRenderTargetView(rtvH, clearColor, 0, nullptr);
	cmdList->ClearDepthStencilView(dsvH, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);

	//ビューポート、シザー矩形
	cmdList->RSSetViewports(1, &CD3DX12_VIEWPORT(0.0f, 0.0f, Window::Window_Width, Window::Window_Height));
	cmdList->RSSetScissorRects(1, &CD3DX12_RECT(0, 0, Window::Window_Width, Window::Window_Height));
#pragma endregion
}

void DirectXManager::End()
{
	
	//リソースバリアを戻す
	cmdList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(backBuffers[bbIndex].Get(),
		D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT));
	//命令のクローズ
	cmdList->Close();
	//コマンドリスト実行
	ID3D12CommandList*cmdLists[] = { cmdList.Get() };//コマンドリストの配列
	cmdQueue->ExecuteCommandLists(1, cmdLists);
	//コマンドリストの実行完了をまつ
	cmdQueue->Signal(fence.Get(), ++fenceVal);
	if (fence->GetCompletedValue() != fenceVal)
	{
		HANDLE event = CreateEvent(nullptr, false, false, nullptr);
		fence->SetEventOnCompletion(fenceVal, event);
		WaitForSingleObject(event, INFINITE);
		CloseHandle(event);
	}

	cmdAllocator->Reset();	//キューをクリア
	cmdList->Reset(cmdAllocator.Get(), nullptr);//再びコマンドリストをためる準備
	//バッファフリップ
	swapChain->Present(1, 0);
}

void DirectXManager::SetData3D()
{
	//パイプライン設定
	cmdList->SetPipelineState(pipeLine->GetPipeLineState("unti"));
	//ルートシグネスチャ生成
	cmdList->SetGraphicsRootSignature(pipeLine->GetRootSignature("unti"));
	//プリミティブ形状
	cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	
}

void DirectXManager::SetData2D()
{
	//パイプライン設定
	cmdList->SetPipelineState(pipeLine->GetPipeLineState("unti2d"));
	//ルートシグネスチャ生成
	cmdList->SetGraphicsRootSignature(pipeLine->GetRootSignature("unti2d"));
	//プリミティブ形状
	cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
}

void DirectXManager::SetDataParticle()
{
	//パイプライン設定
	cmdList->SetPipelineState(pipeLine->GetPipeLineState("untiP"));
	//ルートシグネスチャ生成
	cmdList->SetGraphicsRootSignature(pipeLine->GetRootSignature("untiP"));
	//プリミティブ形状
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
		UpdateFPS();//規定フレーム数になったら更新
	}
}

void DirectXManager::PostEffctBuff()
{
	ComPtr<ID3DBlob>vsBlob;//頂点シェーダオブジェクト
	ComPtr<ID3DBlob>psBlob;//PSシェーダオブジェクト
	ComPtr<ID3DBlob>errorBlob;//eraaaaaaaaシェーダオブジェクト
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
		L"Shader/PostEffectVertexShader.hlsl",//シェーダーファイル名
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,//インクルード可能にする
		"VSmain", "vs_5_0",//エントリーポイント名,シェーダーモデル指定
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,//デバッグ用指定
		0,
		&vsBlob, &errorBlob
	);
	//errorBlobからエラーを受け取りstringにする
	if (FAILED(result))
	{
		std::string errstr;
		errstr.resize(errorBlob->GetBufferSize());
		std::copy_n((char*)errorBlob->GetBufferPointer(),
			errorBlob->GetBufferSize(),
			errstr.begin());
		errstr += "\n";
		//エラー内容を出力ウィンドウに表示
		OutputDebugStringA(errstr.c_str());
		exit(1);
	}
	//PS
	result = D3DCompileFromFile(
		L"Shader/PostEffectPixelShader.hlsl",//シェーダーファイル名
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,//インクルード可能にする
		"PSmain", "ps_5_0",//エントリーポイント名,シェーダーモデル指定
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,//デバッグ用指定
		0,
		&psBlob, &errorBlob
	);

	//errorBlobからエラーを受け取りstringにする
	if (FAILED(result))
	{
		std::string errstr;
		errstr.resize(errorBlob->GetBufferSize());
		std::copy_n((char*)errorBlob->GetBufferPointer(),
			errorBlob->GetBufferSize(),
			errstr.begin());
		errstr += "\n";
		//エラー内容を出力ウィンドウに表示
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
	//スプライト描画コマンド
	ID3D12DescriptorHeap* ppHeaps[] = { pipeLine->GetDescripterHeap() };
	cmdList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);
}

void DirectXManager::PostEffctBegin()
{

	////ビューポート、シザー矩形
	cmdList->RSSetViewports(1, &CD3DX12_VIEWPORT(0.0f, 0.0f, Window::Window_Width, Window::Window_Height));
	cmdList->RSSetScissorRects(1, &CD3DX12_RECT(0, 0, Window::Window_Width, Window::Window_Height));

	//リソースバリア変更
	cmdList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(postEffctResouce[0].Get(),
		D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE, D3D12_RESOURCE_STATE_RENDER_TARGET));
	////リソースバリア変更
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
		cmdList->ClearRenderTargetView(handles[i], clearColor, 0, nullptr);//ここくさい
	}

	////imgui関連
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
	//リソースバリアを戻す
	cmdList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(postEffctResouce[0].Get(),
		D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE));
	////リソースバリアを戻す
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

