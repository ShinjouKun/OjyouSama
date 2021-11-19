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
//FPS用
#include<thread>
#include<chrono>
//数字
#include"Utility/Sequence/Sequence.h"

using namespace Microsoft::WRL;
using namespace std;
//数学ライブラリ
#include"DirectXMath.h"
using namespace DirectX;
//シェーダー用
#include"d3dcompiler.h"
//DirectXTex
#include"DirectXTex.h"
//インプット
#include "Device/Input.h"
//初期化
#include "Device/DirectXManager.h"
//スプライト描画
#include "Render/TexLoader.h"
#include "Render/TexRenderer.h"
//パーティクル
#include "Render/ParticleManager.h"
//モデル
#include "Render/ModelLoader.h"//ロード用
#include "Render/TexRenderer.h"
//ウィンドウ
#include "Device/Window.h"
//カメラ
#include "Render/Camera.h"
//シーン
#include "Scene/SceneManager.h"
unique_ptr<SceneManager>mScene;//このクラスだけが持つポインタ
//パイプライン
#include "Device/PipeLine.h"

//サウンド
#include "Sound/SoundSystem.h"

#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")
#pragma comment(lib,"d3dcompiler.lib")
#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"dxguid.lib")
//デバッグ
ComPtr<ID3D12Debug> debugController;
void Debug();//デバッグ
//Main関数
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	//現在時刻をマイクロ秒で獲得
	std::function<long long(void)>currentTimeMicro = []()
	{
		std::chrono::system_clock::duration d = std::chrono::system_clock::now().time_since_epoch();
		return std::chrono::duration_cast<std::chrono::microseconds>(d).count();
	};
	int cnt = 0;
	int fps = 60;//60fpsで動作させる
	long long end = currentTimeMicro();//現在時間の獲得(1秒 = 1000000)
	long long next = end + (1000 * 1000 / fps);//次の更新時間を計算
	//ウィンドウクラス
	Window* window = nullptr;
	window = new Window();
	window->CreateGameWindow();
	Debug();//デバッグ用
	//DXManager
	DirectXManager::GetInstance()->Init(window);
	//カメラ
	Camera* camera = new Camera();

	PipeLine* pipeLine = new PipeLine();//パイプライン生成
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
	ModelLoader::GetInstance(pipeLine)->Load("Resouse/BoxTankATKA.obj");//砲塔
	ModelLoader::GetInstance(pipeLine)->Load("Resouse/BoxTankATKB.obj");//砲身

	ModelLoader::GetInstance(pipeLine)->Load("Resouse/body.obj");//車体
	ModelLoader::GetInstance(pipeLine)->Load("Resouse/head.obj");//砲塔

	ModelLoader::GetInstance(pipeLine)->Load("Resouse/sensha_body.obj");//車体
	ModelLoader::GetInstance(pipeLine)->Load("Resouse/houtou.obj");//砲塔

	//ModelLoader::GetInstance(pipeLine)->Load("Resouse/sensya_Type2_head.obj");//砲塔
	//ModelLoader::GetInstance(pipeLine)->Load("Resouse/sensya_Typ2_body.obj");//砲塔

	//お嬢様
	ModelLoader::GetInstance(pipeLine)->Load("Resouse/R_hands.obj");
	ModelLoader::GetInstance(pipeLine)->Load("Resouse/ojosama_body.obj");
	ModelLoader::GetInstance(pipeLine)->Load("Resouse/L_hands.obj");

	ModelLoader::GetInstance(pipeLine)->Load("Resouse/BoxTankBTMR.obj");
	ModelLoader::GetInstance(pipeLine)->Load("Resouse/BoxTankATKAR.obj");//砲塔
	ModelLoader::GetInstance(pipeLine)->Load("Resouse/BoxTankATKBR.obj");//砲身
	ModelLoader::GetInstance(pipeLine)->Load("Resouse/kisikunTank.obj");
	//Enemy
	ModelLoader::GetInstance(pipeLine)->Load("Resouse/EnemyHou.obj");
	ModelLoader::GetInstance(pipeLine)->Load("Resouse/EnemyDaiza.obj");

	ModelLoader::GetInstance(pipeLine)->Load("Resouse/GolemBody.obj");
	ModelLoader::GetInstance(pipeLine)->Load("Resouse/GolemArm_L.obj");//砲塔
	ModelLoader::GetInstance(pipeLine)->Load("Resouse/GolemArm_R.obj");//砲身
	//Block
	ModelLoader::GetInstance(pipeLine)->Load("Resouse/bill.obj");
	//Bullet
	ModelLoader::GetInstance(pipeLine)->Load("Resouse/Bullet.obj");
	ModelLoader::GetInstance(pipeLine)->Load("Resouse/mine.obj");
	//投げる用の石
	ModelLoader::GetInstance(pipeLine)->Load("Resouse/throw.obj");
	//ステージ
	ModelLoader::GetInstance(pipeLine)->Load("Resouse/skybox.obj");
	ModelLoader::GetInstance(pipeLine)->Load("Resouse/ground.obj");
	//いろいろ使えるプレーン
	ModelLoader::GetInstance(pipeLine)->Load("Resouse/Plane.obj");

	//木
	ModelLoader::GetInstance(pipeLine)->Load("Resouse/wood.obj");

	//岩
	ModelLoader::GetInstance(pipeLine)->Load("Resouse/stone_s.obj");
	ModelLoader::GetInstance(pipeLine)->Load("Resouse/stone_m.obj");
	ModelLoader::GetInstance(pipeLine)->Load("Resouse/stone_big.obj"); 

	//矢
	ModelLoader::GetInstance(pipeLine)->Load("Resouse/EnemyModel/Arrow/arrow.obj");
	//エルフA
	ModelLoader::GetInstance(pipeLine)->Load("Resouse/EnemyModel/Elf_A/elf_body.obj");
	ModelLoader::GetInstance(pipeLine)->Load("Resouse/EnemyModel/Elf_A/elf_head.obj");
	ModelLoader::GetInstance(pipeLine)->Load("Resouse/EnemyModel/Elf_A/leg_L.obj");
	ModelLoader::GetInstance(pipeLine)->Load("Resouse/EnemyModel/Elf_A/leg_R.obj");
	//エルフB
	ModelLoader::GetInstance(pipeLine)->Load("Resouse/EnemyModel/Elf_B/elf_body2.obj");
	ModelLoader::GetInstance(pipeLine)->Load("Resouse/EnemyModel/Elf_B/elf_head2.obj");
	ModelLoader::GetInstance(pipeLine)->Load("Resouse/EnemyModel/Elf_B/leg_L_B.obj");
	ModelLoader::GetInstance(pipeLine)->Load("Resouse/EnemyModel/Elf_B/leg_R_B.obj");
	//エルフC
	ModelLoader::GetInstance(pipeLine)->Load("Resouse/EnemyModel/Elf_C/elf_body3.obj");
	ModelLoader::GetInstance(pipeLine)->Load("Resouse/EnemyModel/Elf_C/elf_head3.obj");
	ModelLoader::GetInstance(pipeLine)->Load("Resouse/EnemyModel/Elf_C/leg_L_C.obj");
	ModelLoader::GetInstance(pipeLine)->Load("Resouse/EnemyModel/Elf_C/leg_R_C.obj");
	//エルフD
	ModelLoader::GetInstance(pipeLine)->Load("Resouse/EnemyModel/Elf_D/elf_body4.obj");
	ModelLoader::GetInstance(pipeLine)->Load("Resouse/EnemyModel/Elf_D/elf_head4.obj");
	ModelLoader::GetInstance(pipeLine)->Load("Resouse/EnemyModel/Elf_D/leg_L_D.obj");
	ModelLoader::GetInstance(pipeLine)->Load("Resouse/EnemyModel/Elf_D/leg_R_D.obj");
	//木のボス
	ModelLoader::GetInstance(pipeLine)->Load("Resouse/EnemyModel/TreeBoss/wood_body.obj");
	ModelLoader::GetInstance(pipeLine)->Load("Resouse/EnemyModel/TreeBoss/wood_hands_L.obj");
	ModelLoader::GetInstance(pipeLine)->Load("Resouse/EnemyModel/TreeBoss/wood_hands_R.obj");
	//リンゴの弾
	ModelLoader::GetInstance(pipeLine)->Load("Resouse/EnemyModel/AppleBullet/apple.obj");
	//根っこの攻撃
	ModelLoader::GetInstance(pipeLine)->Load("Resouse/EnemyModel/TreeRoot/nekko.obj");

	//当たり判定用の円
	ModelLoader::GetInstance(pipeLine)->Load("Resouse/maru.obj");

	ModelLoader::GetInstance(pipeLine)->Load("Resouse/big_sensha_head.obj");//砲塔
	ModelLoader::GetInstance(pipeLine)->Load("Resouse/big_sensha_body.obj");//砲塔

	//ゴーレム
	ModelLoader::GetInstance(pipeLine)->Load("Resouse/gorem_body.obj");
	ModelLoader::GetInstance(pipeLine)->Load("Resouse/gorem_hands_R.obj");
	ModelLoader::GetInstance(pipeLine)->Load("Resouse/gorem_hands_L.obj");
	//スプライト
	shared_ptr<TexRenderer>sprite = make_shared<TexRenderer>(pipeLine);
	
	//パーティクル
	shared_ptr<ParticleManager>paricle = make_shared<ParticleManager>(pipeLine);
	
	//モデル
	shared_ptr<ModelRenderer>model = make_shared<ModelRenderer>(pipeLine);

	
	Input* input = new Input();//インプットインスタンス生成
	input->Init(window->GetHWND());//インプット初期化
	input->InitGamepad(window->GetHWND());

	//サウンドシステムの生成
	auto& s = SoundSystem::instance();

	//シーン
	mScene = std::make_unique<SceneManager>(sprite, model, paricle);

	//数字関連
	auto & nums = Sequence::instance();
	nums.setDev(DirectXManager::GetInstance()->Dev());
	while (true)
	{

		if (window->ProcesssMessage()) { break; }//メッセージ処理
		
		DirectXManager::GetInstance()->PostEffctBegin();
		//キー入力
		input->Update();//input	
		input->UpdateGamepad();//ゲームパッド
		//描画
		mScene->Update();

#ifndef DEBUG

		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)",
			1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

#endif // DEBUG

		s.update();//各updateが終わった後に音の処理を入れる
		DirectXManager::GetInstance()->SetDrawComnd();
		mScene->Draw();
		nums.drawNumber(DirectXManager::GetInstance()->CmdList(),pipeLine);
		DirectXManager::GetInstance()->PostEffctEnd();
		DirectXManager::GetInstance()->Begin();
		
		DirectXManager::GetInstance()->PostEffctDraw();
		DirectXManager::GetInstance()->End();
#pragma region FPS処理
		cnt++;//fps用
		//重い処理があった時
		std::this_thread::sleep_for(std::chrono::microseconds(1));
		//できるだけ60fpsに保つ
		end  = currentTimeMicro();
		if (end < next)
		{
			//更新時間まで待機
			std::this_thread::sleep_for(std::chrono::microseconds(next - end));
			//次の更新時間を計算
			next += (1000 * 1000 / fps);
		}
		else
		{
			next = end + (1000 * 1000 / fps);
		}
#pragma endregion
		
	}
	nums.finalize();
	window->DeleateGameWindow();//ゲームwindow破棄
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
