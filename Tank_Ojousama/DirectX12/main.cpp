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
#include"../DirectX12/Utility/ModelChanger.h"
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
//�p�[�e�B�N��
#include "ParticleSystem/ParticleSystem.h"

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
	//���f��

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
	TexLoader::GetInstance(pipeLine)->Load("Resouse/GarageWeponSelect.png");
	TexLoader::GetInstance(pipeLine)->Load("Resouse/GarageSelect.png");
	TexLoader::GetInstance(pipeLine)->Load("Resouse/Bom.jpg");
	TexLoader::GetInstance(pipeLine)->Load("Resouse/garege.jpg");
	TexLoader::GetInstance(pipeLine)->Load("Resouse/syata.jpg");
	TexLoader::GetInstance(pipeLine)->Load("Resouse/smoke.jpg");
	TexLoader::GetInstance(pipeLine)->Load("Resouse/croshear.png");
	TexLoader::GetInstance(pipeLine)->Load("Resouse/back.png");
	TexLoader::GetInstance(pipeLine)->Load("Resouse/backselect.png");
	TexLoader::GetInstance(pipeLine)->Load("Resouse/controller.png");
	TexLoader::GetInstance(pipeLine)->Load("Resouse/operation.png");
	TexLoader::GetInstance(pipeLine)->Load("Resouse/operationback.png");
	TexLoader::GetInstance(pipeLine)->Load("Resouse/wepon.png");
	TexLoader::GetInstance(pipeLine)->Load("Resouse/hpUI.png");
	TexLoader::GetInstance(pipeLine)->Load("Resouse/title_rogo.png");
	TexLoader::GetInstance(pipeLine)->Load("Resouse/setumeibos.png");
	TexLoader::GetInstance(pipeLine)->Load("Resouse/setumeiDe.png");
	TexLoader::GetInstance(pipeLine)->Load("Resouse/setumeiRo.png");
	TexLoader::GetInstance(pipeLine)->Load("Resouse/gameover.png");
	TexLoader::GetInstance(pipeLine)->Load("Resouse/testend.png");
	TexLoader::GetInstance(pipeLine)->Load("Resouse/map_base.png");
	TexLoader::GetInstance(pipeLine)->Load("Resouse/map_1_clear.png");
	TexLoader::GetInstance(pipeLine)->Load("Resouse/map_2_clear.png");
	TexLoader::GetInstance(pipeLine)->Load("Resouse/map_3_clear.png");
	TexLoader::GetInstance(pipeLine)->Load("Resouse/map_4_clear.png");
	TexLoader::GetInstance(pipeLine)->Load("Resouse/messege1.png");
	TexLoader::GetInstance(pipeLine)->Load("Resouse/messege2.png");
	TexLoader::GetInstance(pipeLine)->Load("Resouse/messege3.png");
	TexLoader::GetInstance(pipeLine)->Load("Resouse/back_sky.png");

	TexLoader::GetInstance(pipeLine)->Load("Resouse/titleAho.png");
	TexLoader::GetInstance(pipeLine)->Load("Resouse/Space.png");
	TexLoader::GetInstance(pipeLine)->Load("Resouse/kabe.png");

	TexLoader::GetInstance(pipeLine)->Load("Resouse/item_box.png");
	TexLoader::GetInstance(pipeLine)->Load("Resouse/item1.png");
	TexLoader::GetInstance(pipeLine)->Load("Resouse/item2.png");
	TexLoader::GetInstance(pipeLine)->Load("Resouse/wave1.png");
	TexLoader::GetInstance(pipeLine)->Load("Resouse/wave2.png");
	TexLoader::GetInstance(pipeLine)->Load("Resouse/wave3.png");
	TexLoader::GetInstance(pipeLine)->Load("Resouse/redflo.png");
	TexLoader::GetInstance(pipeLine)->Load("Resouse/blueflo.png");
	TexLoader::GetInstance(pipeLine)->Load("Resouse/greenflo.png");
	TexLoader::GetInstance(pipeLine)->Load("Resouse/pinkflo.png");
	TexLoader::GetInstance(pipeLine)->Load("Resouse/hpgage.png");
	TexLoader::GetInstance(pipeLine)->Load("Resouse/Title.png");

	TexLoader::GetInstance(pipeLine)->Load("Resouse/blood.png");
	TexLoader::GetInstance(pipeLine)->Load("Resouse/blood2.png");
	TexLoader::GetInstance(pipeLine)->Load("Resouse/blood3.png");
	TexLoader::GetInstance(pipeLine)->Load("Resouse/castle_aicon.png");
	TexLoader::GetInstance(pipeLine)->Load("Resouse/Film.png");

	TexLoader::GetInstance(pipeLine)->Load("Resouse/heal.png");
	TexLoader::GetInstance(pipeLine)->Load("Resouse/ShieldEffect.png");

	TexLoader::GetInstance(pipeLine)->Load("Resouse/endhing.png");
	TexLoader::GetInstance(pipeLine)->Load("Resouse/flower.png");
	TexLoader::GetInstance(pipeLine)->Load("Resouse/flower2.png");
	TexLoader::GetInstance(pipeLine)->Load("Resouse/flower3.png");
	TexLoader::GetInstance(pipeLine)->Load("Resouse/theend.png");
	TexLoader::GetInstance(pipeLine)->Load("Resouse/opening.png");
	TexLoader::GetInstance(pipeLine)->Load("Resouse/gameplaysetumei.png");
	TexLoader::GetInstance(pipeLine)->Load("Resouse/robberysetumei.png");
	TexLoader::GetInstance(pipeLine)->Load("Resouse/difencesetumei.png");
	TexLoader::GetInstance(pipeLine)->Load("Resouse/bossscenesetumei.png");

	TexLoader::GetInstance(pipeLine)->Load("Resouse/RepairIcon.png");
	TexLoader::GetInstance(pipeLine)->Load("Resouse/SmokeIcon.png");
	TexLoader::GetInstance(pipeLine)->Load("Resouse/ShieldIcon.png");
	TexLoader::GetInstance(pipeLine)->Load("Resouse/X.png");

	//����
	ModelLoader::GetInstance(pipeLine)->Load("Resouse/boxs.obj");
	ModelLoader::GetInstance(pipeLine)->Load("Resouse/boxs_huta.obj");
	//�h�q���_
	ModelLoader::GetInstance(pipeLine)->Load("Resouse/castle.obj");
	//Player
	ModelLoader::GetInstance(pipeLine)->Load("Resouse/BoxTankBTM.obj");
	ModelLoader::GetInstance(pipeLine)->Load("Resouse/BoxTankATKA.obj");//�C��
	ModelLoader::GetInstance(pipeLine)->Load("Resouse/BoxTankATKB.obj");//�C�g

	ModelLoader::GetInstance(pipeLine)->Load("Resouse/body.obj");//�ԑ�
	ModelLoader::GetInstance(pipeLine)->Load("Resouse/head.obj");//�C��

	ModelLoader::GetInstance(pipeLine)->Load("Resouse/sensha_body.obj");//�ԑ�
	ModelLoader::GetInstance(pipeLine)->Load("Resouse/houtou.obj");//�C��

	ModelLoader::GetInstance(pipeLine)->Load("Resouse/sensya_Type2_head.obj");//�C��
	ModelLoader::GetInstance(pipeLine)->Load("Resouse/sensya_Typ2_body.obj");//�C��

	//����l
	ModelLoader::GetInstance(pipeLine)->Load("Resouse/R_hands.obj");
	ModelLoader::GetInstance(pipeLine)->Load("Resouse/R_hands_rifle.obj");
	ModelLoader::GetInstance(pipeLine)->Load("Resouse/sg_ojosama_R.obj");
	ModelLoader::GetInstance(pipeLine)->Load("Resouse/ojosama_body.obj");
	ModelLoader::GetInstance(pipeLine)->Load("Resouse/L_hands.obj");
	ModelLoader::GetInstance(pipeLine)->Load("Resouse/L_hands_rifle.obj");
	ModelLoader::GetInstance(pipeLine)->Load("Resouse/sg_ojosama_L.obj");

	ModelLoader::GetInstance(pipeLine)->Load("Resouse/L_hands_rifle_b.obj");
	ModelLoader::GetInstance(pipeLine)->Load("Resouse/sg_ojosama_L_b.obj");
	ModelLoader::GetInstance(pipeLine)->Load("Resouse/ojousama_body_black.obj");
	ModelLoader::GetInstance(pipeLine)->Load("Resouse/R_hands_rifle_b.obj");
	ModelLoader::GetInstance(pipeLine)->Load("Resouse/sg_ojosama_R_b.obj");

	ModelLoader::GetInstance(pipeLine)->Load("Resouse/L_hands_rifle_r.obj");
	ModelLoader::GetInstance(pipeLine)->Load("Resouse/sg_ojosama_L_r.obj");
	ModelLoader::GetInstance(pipeLine)->Load("Resouse/ojousama_body_red.obj");
	ModelLoader::GetInstance(pipeLine)->Load("Resouse/R_hands_rifle_r.obj");
	ModelLoader::GetInstance(pipeLine)->Load("Resouse/sg_ojosama_R_r.obj");

	ModelLoader::GetInstance(pipeLine)->Load("Resouse/R_hands_rifle.obj");
	ModelLoader::GetInstance(pipeLine)->Load("Resouse/L_hands_rifle.obj");

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
	ModelLoader::GetInstance(pipeLine)->Load("Resouse/bullet.obj");
	ModelLoader::GetInstance(pipeLine)->Load("Resouse/mine.obj");
	//������p�̐�
	ModelLoader::GetInstance(pipeLine)->Load("Resouse/throw.obj");
	//�X�e�[�W
	ModelLoader::GetInstance(pipeLine)->Load("Resouse/skybox.obj");
	ModelLoader::GetInstance(pipeLine)->Load("Resouse/ground.obj");
	//���낢��g����v���[��
	ModelLoader::GetInstance(pipeLine)->Load("Resouse/Plane.obj");
	ModelLoader::GetInstance(pipeLine)->Load("Resouse/Plane1.obj");

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
	//�ߐ�
	ModelLoader::GetInstance(pipeLine)->Load("Resouse/yari_elf_body.obj");
	ModelLoader::GetInstance(pipeLine)->Load("Resouse/yari_elf_head.obj");
	ModelLoader::GetInstance(pipeLine)->Load("Resouse/yari_elf_L.obj");
	ModelLoader::GetInstance(pipeLine)->Load("Resouse/yari_elf_R.obj");
	ModelLoader::GetInstance(pipeLine)->Load("Resouse/yari_elf_te.obj");
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

	//�񕜃A�C�e��
	ModelLoader::GetInstance(pipeLine)->Load("Resouse/item.obj");
	//�V�[���h
	ModelLoader::GetInstance(pipeLine)->Load("Resouse/shield.obj");
	//�X���[�N
	ModelLoader::GetInstance(pipeLine)->Load("Resouse/smokeModel.obj");

	//���̓G
	ModelLoader::GetInstance(pipeLine)->Load("Resouse/EnemyModel/Eagle/eagle_leg.obj");
	ModelLoader::GetInstance(pipeLine)->Load("Resouse/EnemyModel/Eagle/eagle_body.obj");

	//���G���t
	ModelLoader::GetInstance(pipeLine)->Load("Resouse/EnemyModel/SpearElf/yari_elf_head.obj");
	ModelLoader::GetInstance(pipeLine)->Load("Resouse/EnemyModel/SpearElf/yari_elf_body.obj");
	ModelLoader::GetInstance(pipeLine)->Load("Resouse/EnemyModel/SpearElf/yari_te.obj");
	ModelLoader::GetInstance(pipeLine)->Load("Resouse/EnemyModel/SpearElf/yari_elf_R.obj");
	ModelLoader::GetInstance(pipeLine)->Load("Resouse/EnemyModel/SpearElf/yari_elf_L.obj");

	//�h�q���_
	ModelLoader::GetInstance(pipeLine)->Load("Resouse/castle.obj");


	//�{�X�p�̃V�[���h
	ModelLoader::GetInstance(pipeLine)->Load("Resouse/EnemyModel/BossBarrier/boss_barrier.obj");
	ModelLoader::GetInstance(pipeLine)->Load("Resouse/flower.obj");
	ModelLoader::GetInstance(pipeLine)->Load("Resouse/flower1.obj");
	ModelLoader::GetInstance(pipeLine)->Load("Resouse/flower2.obj");
	ModelLoader::GetInstance(pipeLine)->Load("Resouse/flower3.obj");


	//�X�v���C�g
	shared_ptr<TexRenderer>sprite = make_shared<TexRenderer>(pipeLine);

	//�p�[�e�B�N��
	shared_ptr<ParticleManager>paricle = make_shared<ParticleManager>(pipeLine);
	auto& particleSystem = ParticleSystem::instance();

	//���f��
	shared_ptr<ModelRenderer>model = make_shared<ModelRenderer>(pipeLine);


	Input* input = new Input();//�C���v�b�g�C���X�^���X����
	input->Init(window->GetHWND());//�C���v�b�g������

	//�T�E���h�V�X�e���̐���
	auto& s = SoundSystem::instance();

	//�V�[��
	mScene = std::make_unique<SceneManager>(sprite, model, paricle);
	std::vector<string> buys(9,"nonBuy");
	TextEditor* text = new TextEditor();
	text->Write("Resouse/BuysState.txt", buys);

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
		particleSystem.update();

#ifdef _DEBUG

		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)",
			1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

#endif  _DEBUG

		s.update();//�eupdate���I�������ɉ��̏���������
		DirectXManager::GetInstance()->SetDrawComnd();
		mScene->Draw();
		particleSystem.draw();
		nums.drawNumber(DirectXManager::GetInstance()->CmdList(), pipeLine);
		DirectXManager::GetInstance()->PostEffctEnd();
		DirectXManager::GetInstance()->Begin();

		DirectXManager::GetInstance()->PostEffctDraw();
		DirectXManager::GetInstance()->End();
#pragma region FPS����
		cnt++;//fps�p
		//�d����������������
		std::this_thread::sleep_for(std::chrono::microseconds(1));
		//�ł��邾��60fps�ɕۂ�
		end = currentTimeMicro();
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
	particleSystem.finalize();
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
