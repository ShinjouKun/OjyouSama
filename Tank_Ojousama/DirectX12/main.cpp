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
	ModelLoader::GetInstance(pipeLine)->Load("Resouse/BoxTankATKA.obj");//砲塔
	ModelLoader::GetInstance(pipeLine)->Load("Resouse/BoxTankATKB.obj");//砲身

	ModelLoader::GetInstance(pipeLine)->Load("Resouse/body.obj");//車体
	ModelLoader::GetInstance(pipeLine)->Load("Resouse/head.obj");//砲塔

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
	//ステージ
	ModelLoader::GetInstance(pipeLine)->Load("Resouse/skydome.obj");
	ModelLoader::GetInstance(pipeLine)->Load("Resouse/ground.obj");
	//いろいろ使えるプレーン
	ModelLoader::GetInstance(pipeLine)->Load("Resouse/Plane.obj");
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

	
	while (true)
	{

		if (window->ProcesssMessage()) { break; }//メッセージ処理
		
		DirectXManager::GetInstance()->PostEffctBegin();
		//キー入力
		input->Update();//input	
		input->UpdateGamepad();//ゲームパッド
		//描画
		mScene->Update();
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)",
			1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		s.update();//各updateが終わった後に音の処理を入れる
		DirectXManager::GetInstance()->SetDrawComnd();
		mScene->Draw();
		DirectXManager::GetInstance()->PostEffctEnd();
		DirectXManager::GetInstance()->Begin();
		
		DirectXManager::GetInstance()->PostEffctDraw();
		DirectXManager::GetInstance()->End();
#pragma region FPS処理
		cnt++;//fps用
		//重い処理があった時
		std::this_thread::sleep_for(std::chrono::microseconds(5));
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

	window->DeleateGameWindow();//ゲームwindow破棄
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
