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

#include <string> 
#include <dxcapi.h>

class Shader
{
public:
	Shader();
	~Shader();
	//�s�N�Z���V�F�[�_
	void loadPS(std::string fileName, std::string entryFuncName);
	//���_�V�F�[�_
	void loadVS(std::string fileName, std::string entryFuncName);
	//�W�I���g���V�F�[�_
	void loadGS(std::string fileName, std::string entryFuncName);
	//�R���s���[�g�V�F�[�_
	void loadCS(std::string fileName);

	ID3DBlob* getCompiledBlob()const
	{
		return mBlob;
	}

	IDxcBlob* getCompiledDxcBlob()const
	{
		return mDxcBlob;
	}

	bool isInited() const
	{
		return mIsinited;
	}

private:
	void load(std::string filePath, std::string type, std::string shaderVer);

private:
	ID3DBlob* mBlob;
	ID3DBlob* mErrorBlob;
	IDxcBlob* mDxcBlob;
	bool mIsinited;

};