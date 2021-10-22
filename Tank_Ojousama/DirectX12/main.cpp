#include "Device/Window.h"
#include"d3d12.h"
#include"d3dx12.h"
#include"dxgi1_6.h"
#include"vector"
#include"wrl.h"

//imgui
#include"imgui/imgui.h"
#include"imgui/imgui_impl_win32.h"
#include"imgui/imgui_impl_dx12.h"
//FPS�p
#include<thread>
#include<chrono>

using namespace Microsoft::WRL;
using namespace std;
//���w���C�u����
#include"DirectXMath.h"
using namespace DirectX;
//�V�F�[�_�[�p
#include"d3dcompiler.h"
//DirectXTex
#include"DirectXTex.h"
//�C���v�b�g
#include "Device/Input.h"
//������
#include "Device/DirectXManager.h"
//�X�v���C�g�`��
#include "Render/TexLoader.h"
#include "Render/TexRenderer.h"
//�p�[�e�B�N��
#include "Render/ParticleManager.h"
//���f��
#include "Render/ModelLoader.h"//���[�h�p
#include "Render/TexRenderer.h"
//�E�B���h�E
#include "Device/Window.h"
//�J����
#include "Render/Camera.h"
//�V�[��
#include "Scene/SceneManager.h"
unique_ptr<SceneManager>mScene;//���̃N���X���������|�C���^
//�p�C�v���C��
#include "Device/PipeLine.h"

//�T�E���h
#include "Sound/SoundSystem.h"

#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")
#pragma comment(lib,"d3dcompiler.lib")
#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"dxguid.lib")
//�f�o�b�O
ComPtr<ID3D12Debug> debugController;
void Debug();//�f�o�b�O
//Main�֐�
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	//���ݎ������}�C�N���b�Ŋl��
	std::function<long long(void)>currentTimeMicro = []()
	{
		std::chrono::system_clock::duration d = std::chrono::system_clock::now().time_since_epoch();
		return std::chrono::duration_cast<std::chrono::microseconds>(d).count();
	};
	int cnt = 0;
	int fps = 60;//60fps�œ��삳����
	long long end = currentTimeMicro();//���ݎ��Ԃ̊l��(1�b = 1000000)
	long long next = end + (1000 * 1000 / fps);//���̍X�V���Ԃ��v�Z
	//�E�B���h�E�N���X
	Window* window = nullptr;
	window = new Window();
	window->CreateGameWindow();
	Debug();//�f�o�b�O�p
	//DXManager
	DirectXManager::GetInstance()->Init(window);
	//�J����
	Camera* camera = new Camera();

	PipeLine* pipeLine = new PipeLine();//�p�C�v���C������
	pipeLine->SetPipeline2D("unti2d");
	pipeLine->SetPipeline3D("unti");
	pipeLine->SetPipelineParticle("untiP");

	TexLoader::GetInstance(pipeLine)->Load("Resouse/hit.png");
	TexLoader::GetInstance(pipeLine)->Load("Resouse/sougen.png");
	TexLoader::GetInstance(pipeLine)->Load("Resouse/AIM64.png");
	TexLoader::GetInstance(pipeLine)->Load("Resouse/TankAicn.png");
	TexLoader::GetInstance(pipeLine)->Load("Resouse/TankUI.png");
	TexLoader::GetInstance(pipeLine)->Load("Resouse/Deth.png");
	TexLoader::GetInstance(pipeLine)->Load("Resouse/kisikun.png");
	TexLoader::GetInstance(pipeLine)->Load("Resouse/syutugeki.png");
	TexLoader::GetInstance(pipeLine)->Load("Resouse/garezi.png");
	TexLoader::GetInstance(pipeLine)->Load("Resouse/opusyon.png");
	TexLoader::GetInstance(pipeLine)->Load("Resouse/tatleback.png");
	TexLoader::GetInstance(pipeLine)->Load("Resouse/select.png");
	TexLoader::GetInstance(pipeLine)->Load("Resouse/setuemei.png");
	TexLoader::GetInstance(pipeLine)->Load("Resouse/pose.png");
	TexLoader::GetInstance(pipeLine)->Load("Resouse/selectback.png");
	TexLoader::GetInstance(pipeLine)->Load("Resouse/selectcursol.png");
	TexLoader::GetInstance(pipeLine)->Load("Resouse/option.png");
	TexLoader::GetInstance(pipeLine)->Load("Resouse/ritorai.png");
	TexLoader::GetInstance(pipeLine)->Load("Resouse/hime.png");
	TexLoader::GetInstance(pipeLine)->Load("Resouse/heart.png");


	TexLoader::GetInstance(pipeLine)->Load("Resouse/titleAho.png");
	TexLoader::GetInstance(pipeLine)->Load("Resouse/Space.png");

	//Player
	ModelLoader::GetInstance(pipeLine)->Load("Resouse/BoxTankBTM.obj");
	ModelLoader::GetInstance(pipeLine)->Load("Resouse/BoxTankATKA.obj");//�C��
	ModelLoader::GetInstance(pipeLine)->Load("Resouse/BoxTankATKB.obj");//�C�g

	ModelLoader::GetInstance(pipeLine)->Load("Resouse/body.obj");//�ԑ�
	ModelLoader::GetInstance(pipeLine)->Load("Resouse/head.obj");//�C��

	ModelLoader::GetInstance(pipeLine)->Load("Resouse/BoxTankBTMR.obj");
	ModelLoader::GetInstance(pipeLine)->Load("Resouse/BoxTankATKAR.obj");//�C��
	ModelLoader::GetInstance(pipeLine)->Load("Resouse/BoxTankATKBR.obj");//�C�g
	ModelLoader::GetInstance(pipeLine)->Load("Resouse/kisikunTank.obj");
	//Enemy
	ModelLoader::GetInstance(pipeLine)->Load("Resouse/EnemyHou.obj");
	ModelLoader::GetInstance(pipeLine)->Load("Resouse/EnemyDaiza.obj");

	ModelLoader::GetInstance(pipeLine)->Load("Resouse/GolemBody.obj");
	ModelLoader::GetInstance(pipeLine)->Load("Resouse/GolemArm_L.obj");//�C��
	ModelLoader::GetInstance(pipeLine)->Load("Resouse/GolemArm_R.obj");//�C�g
	//Block
	ModelLoader::GetInstance(pipeLine)->Load("Resouse/bill.obj");
	//Bullet
	ModelLoader::GetInstance(pipeLine)->Load("Resouse/Bullet.obj");
	ModelLoader::GetInstance(pipeLine)->Load("Resouse/mine.obj");
	//�X�e�[�W
	ModelLoader::GetInstance(pipeLine)->Load("Resouse/skydome.obj");
	ModelLoader::GetInstance(pipeLine)->Load("Resouse/ground.obj");
	//���낢��g����v���[��
	ModelLoader::GetInstance(pipeLine)->Load("Resouse/Plane.obj");
	//�X�v���C�g
	shared_ptr<TexRenderer>sprite = make_shared<TexRenderer>(pipeLine);
	
	//�p�[�e�B�N��
	shared_ptr<ParticleManager>paricle = make_shared<ParticleManager>(pipeLine);
	
	//���f��
	shared_ptr<ModelRenderer>model = make_shared<ModelRenderer>(pipeLine);

	
	Input* input = new Input();//�C���v�b�g�C���X�^���X����
	input->Init(window->GetHWND());//�C���v�b�g������
	input->InitGamepad(window->GetHWND());

	//�T�E���h�V�X�e���̐���
	auto& s = SoundSystem::instance();

	//�V�[��
	mScene = std::make_unique<SceneManager>(sprite, model, paricle);

	
	while (true)
	{

		if (window->ProcesssMessage()) { break; }//���b�Z�[�W����
		
		DirectXManager::GetInstance()->PostEffctBegin();
		//�L�[����
		input->Update();//input	
		input->UpdateGamepad();//�Q�[���p�b�h
		//�`��
		mScene->Update();
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)",
			1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		s.update();//�eupdate���I�������ɉ��̏���������
		DirectXManager::GetInstance()->SetDrawComnd();
		mScene->Draw();
		DirectXManager::GetInstance()->PostEffctEnd();
		DirectXManager::GetInstance()->Begin();
		
		DirectXManager::GetInstance()->PostEffctDraw();
		DirectXManager::GetInstance()->End();
#pragma region FPS����
		cnt++;//fps�p
		//�d����������������
		std::this_thread::sleep_for(std::chrono::microseconds(5));
		//�ł��邾��60fps�ɕۂ�
		end  = currentTimeMicro();
		if (end < next)
		{
			//�X�V���Ԃ܂őҋ@
			std::this_thread::sleep_for(std::chrono::microseconds(next - end));
			//���̍X�V���Ԃ��v�Z
			next += (1000 * 1000 / fps);
		}
		else
		{
			next = end + (1000 * 1000 / fps);
		}
#pragma endregion
		
	}

	window->DeleateGameWindow();//�Q�[��window�j��
	pipeLine->Clear();
	delete input;
	delete camera;
	delete pipeLine;
	return 0;
}


void Debug()
{
	
	if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController))))
	{
		debugController->EnableDebugLayer();
	}
}
