#include "Shader.h"
#include <cassert>

namespace
{
	const char* vsShaderVer = "vs_5_0";
	const char* psShaderVer = "ps_5_0";
	const char* gsShaderVer = "gs_5_0";
	const char* csShaderVer = "cs_5_1";
}

Shader::Shader():
	mBlob(nullptr),
	mErrorBlob(nullptr),
	mDxcBlob(nullptr),
	mIsinited(false)
{
}

Shader::~Shader()
{
	if (mBlob)
	{
		mBlob->Release();
		mBlob = nullptr;
	}
	if (mErrorBlob)
	{
		mErrorBlob->Release();
		mErrorBlob = nullptr;
	}
	if (mDxcBlob)
	{
		mDxcBlob->Release();
		mDxcBlob = nullptr;
	}
}

void Shader::loadPS(std::string fileName, std::string entryFuncName)
{
	std::string filePath = "Shader/Compute/" + fileName;
	load(filePath, "PS", psShaderVer);
}

void Shader::loadVS(std::string fileName, std::string entryFuncName)
{
	std::string filePath = "Shader/Compute/" + fileName;
	load(filePath, "VS", vsShaderVer);
}

void Shader::loadGS(std::string fileName, std::string entryFuncName)
{
	std::string filePath = "Shader/Compute/" + fileName;
	load(filePath, "GS", gsShaderVer);
}

void Shader::loadCS(std::string fileName)
{
	std::string filePath = "Shader/Compute/" + fileName;
	load(filePath, "CS", csShaderVer);
}

void Shader::load(std::string filePath, std::string type, std::string shaderVer)
{
	wchar_t wfxFilePath[256] = { L"" };
	mbstowcs(wfxFilePath, filePath.c_str(), 256);

	//シェーダコンパイル
	auto hr = D3DCompileFromFile(
		wfxFilePath,
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,
		type.c_str(), shaderVer.c_str(),
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
		0,
		&mBlob, &mErrorBlob);

	if (FAILED(hr))
	{
		//errorBlobから内容をstring型にコピー
		std::string errstr;
		errstr.resize(mErrorBlob->GetBufferSize());
		std::copy_n((char*)mErrorBlob->GetBufferPointer(), mErrorBlob->GetBufferSize(), errstr.begin());
		errstr += "\n";

		//エラー内容を出力ウィンドウに表示
		OutputDebugStringA(errstr.c_str());
		assert(0);
	}
}
