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
//����
#include"Utility/Sequence/Sequence.h"

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
	pipeLine->SetPipelineSequence("ahokusa");

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
	TexLoader::GetInstance(pipeLine)->Load("Resouse/sentaku.png");
	TexLoader::GetInstance(pipeLine)->Load("Resouse/fade.png");
	TexLoader::GetInstance(pipeLine)->Load("Resouse/kakutoku.png");
	TexLoader::GetInstance(pipeLine)->Load("Resouse/titlerogo.png");
	TexLoader::GetInstance(pipeLine)->Load("Resouse/start.png");
	TexLoader::GetInstance(pipeLine)->Load("Resouse/cars.png");
	TexLoader::GetInstance(pipeLine)->Load("Resouse/voloption.png");
	TexLoader::GetInstance(pipeLine)->Load("Resouse/volAim.png");
	TexLoader::GetInstance(pipeLine)->Load("Resouse/volAimA.png");
	TexLoader::GetInstance(pipeLine)->Load("Resouse/Sankaku.png");
	TexLoader::GetInstance(pipeLine)->Load("Resouse/GaregeOjoSelect.png");
	TexLoader::GetInstance(pipeLine)->Load("Resouse/GaregeHeadSelect.png");
	TexLoader::GetInstance(pipeLine)->Load("Resouse/GaregeBodySelect.png");
	TexLoader::GetInstance(pipeLine)->Load("Resouse/unti.jpg");
	TexLoader::GetInstance(pipeLine)->Load("Resouse/Bom.jpg");
	TexLoader::GetInstance(pipeLine)->Load("Resouse/garege.jpg");
	TexLoader::GetInstance(pipeLine)->Load("Resouse/syata.jpg");
	TexLoader::GetInstance(pipeLine)->Load("Resouse/croshear.png");
	TexLoader::GetInstance(pipeLine)->Load("Resouse/back.png");
	TexLoader::GetInstance(pipeLine)->Load("Resouse/backselect.png");

	TexLoader::GetInstance(pipeLine)->Load("Resouse/titleAho.png");
	TexLoader::GetInstance(pipeLine)->Load("Resouse/Space.png");
	TexLoader::GetInstance(pipeLine)->Load("Resouse/kabe.png");

	//Player
	ModelLoader::GetInstance(pipeLine)->Load("Resouse/BoxTankBTM.obj");
	ModelLoader::GetInstance(pipeLine)->Load("Resouse/BoxTankATKA.obj");//�C��
	ModelLoader::GetInstance(pipeLine)->Load("Resouse/BoxTankATKB.obj");//�C�g

	ModelLoader::GetInstance(pipeLine)->Load("Resouse/body.obj");//�ԑ�
	ModelLoader::GetInstance(pipeLine)->Load("Resouse/head.obj");//�C��

	ModelLoader::GetInstance(pipeLine)->Load("Resouse/sensha_body.obj");//�ԑ�
	ModelLoader::GetInstance(pipeLine)->Load("Resouse/houtou.obj");//�C��

	//ModelLoader::GetInstance(pipeLine)->Load("Resouse/sensya_Type2_head.obj");//�C��
	//ModelLoader::GetInstance(pipeLine)->Load("Resouse/sensya_Typ2_body.obj");//�C��

	//����l
	ModelLoader::GetInstance(pipeLine)->Load("Resouse/R_hands.obj");
	ModelLoader::GetInstance(pipeLine)->Load("Resouse/ojosama_body.obj");
	ModelLoader::GetInstance(pipeLine)->Load("Resouse/L_hands.obj");

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
	//������p�̐�
	ModelLoader::GetInstance(pipeLine)->Load("Resouse/throw.obj");
	//�X�e�[�W
	ModelLoader::GetInstance(pipeLine)->Load("Resouse/skybox.obj");
	ModelLoader::GetInstance(pipeLine)->Load("Resouse/ground.obj");
	//���낢��g����v���[��
	ModelLoader::GetInstance(pipeLine)->Load("Resouse/Plane.obj");

	//��
	ModelLoader::GetInstance(pipeLine)->Load("Resouse/wood.obj");

	//��
	ModelLoader::GetInstance(pipeLine)->Load("Resouse/stone_s.obj");
	ModelLoader::GetInstance(pipeLine)->Load("Resouse/stone_m.obj");
	ModelLoader::GetInstance(pipeLine)->Load("Resouse/stone_big.obj"); 

	//��
	ModelLoader::GetInstance(pipeLine)->Load("Resouse/EnemyModel/Arrow/arrow.obj");
	//�G���tA
	ModelLoader::GetInstance(pipeLine)->Load("Resouse/EnemyModel/Elf_A/elf_body.obj");
	ModelLoader::GetInstance(pipeLine)->Load("Resouse/EnemyModel/Elf_A/elf_head.obj");
	ModelLoader::GetInstance(pipeLine)->Load("Resouse/EnemyModel/Elf_A/leg_L.obj");
	ModelLoader::GetInstance(pipeLine)->Load("Resouse/EnemyModel/Elf_A/leg_R.obj");
	//�G���tB
	ModelLoader::GetInstance(pipeLine)->Load("Resouse/EnemyModel/Elf_B/elf_body2.obj");
	ModelLoader::GetInstance(pipeLine)->Load("Resouse/EnemyModel/Elf_B/elf_head2.obj");
	ModelLoader::GetInstance(pipeLine)->Load("Resouse/EnemyModel/Elf_B/leg_L_B.obj");
	ModelLoader::GetInstance(pipeLine)->Load("Resouse/EnemyModel/Elf_B/leg_R_B.obj");
	//�G���tC
	ModelLoader::GetInstance(pipeLine)->Load("Resouse/EnemyModel/Elf_C/elf_body3.obj");
	ModelLoader::GetInstance(pipeLine)->Load("Resouse/EnemyModel/Elf_C/elf_head3.obj");
	ModelLoader::GetInstance(pipeLine)->Load("Resouse/EnemyModel/Elf_C/leg_L_C.obj");
	ModelLoader::GetInstance(pipeLine)->Load("Resouse/EnemyModel/Elf_C/leg_R_C.obj");
	//�G���tD
	ModelLoader::GetInstance(pipeLine)->Load("Resouse/EnemyModel/Elf_D/elf_body4.obj");
	ModelLoader::GetInstance(pipeLine)->Load("Resouse/EnemyModel/Elf_D/elf_head4.obj");
	ModelLoader::GetInstance(pipeLine)->Load("Resouse/EnemyModel/Elf_D/leg_L_D.obj");
	ModelLoader::GetInstance(pipeLine)->Load("Resouse/EnemyModel/Elf_D/leg_R_D.obj");
	//�؂̃{�X
	ModelLoader::GetInstance(pipeLine)->Load("Resouse/EnemyModel/TreeBoss/wood_body.obj");
	ModelLoader::GetInstance(pipeLine)->Load("Resouse/EnemyModel/TreeBoss/wood_hands_L.obj");
	ModelLoader::GetInstance(pipeLine)->Load("Resouse/EnemyModel/TreeBoss/wood_hands_R.obj");
	//�����S�̒e
	ModelLoader::GetInstance(pipeLine)->Load("Resouse/EnemyModel/AppleBullet/apple.obj");
	//�������̍U��
	ModelLoader::GetInstance(pipeLine)->Load("Resouse/EnemyModel/TreeRoot/nekko.obj");

	//�����蔻��p�̉~
	ModelLoader::GetInstance(pipeLine)->Load("Resouse/maru.obj");

	ModelLoader::GetInstance(pipeLine)->Load("Resouse/big_sensha_head.obj");//�C��
	ModelLoader::GetInstance(pipeLine)->Load("Resouse/big_sensha_body.obj");//�C��

	//�S�[����
	ModelLoader::GetInstance(pipeLine)->Load("Resouse/gorem_body.obj");
	ModelLoader::GetInstance(pipeLine)->Load("Resouse/gorem_hands_R.obj");
	ModelLoader::GetInstance(pipeLine)->Load("Resouse/gorem_hands_L.obj");
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

	//�����֘A
	auto & nums = Sequence::instance();
	nums.setDev(DirectXManager::GetInstance()->Dev());
	while (true)
	{

		if (window->ProcesssMessage()) { break; }//���b�Z�[�W����
		
		DirectXManager::GetInstance()->PostEffctBegin();
		//�L�[����
		input->Update();//input	
		input->UpdateGamepad();//�Q�[���p�b�h
		//�`��
		mScene->Update();

#ifndef DEBUG

		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)",
			1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

#endif // DEBUG

		s.update();//�eupdate���I�������ɉ��̏���������
		DirectXManager::GetInstance()->SetDrawComnd();
		mScene->Draw();
		nums.drawNumber(DirectXManager::GetInstance()->CmdList(),pipeLine);
		DirectXManager::GetInstance()->PostEffctEnd();
		DirectXManager::GetInstance()->Begin();
		
		DirectXManager::GetInstance()->PostEffctDraw();
		DirectXManager::GetInstance()->End();
#pragma region FPS����
		cnt++;//fps�p
		//�d����������������
		std::this_thread::sleep_for(std::chrono::microseconds(1));
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
	nums.finalize();
	window->DeleateGameWindow();//�Q�[��window�j��
	pipeLine->Clear();
	delete input;
	//delete camera;
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
