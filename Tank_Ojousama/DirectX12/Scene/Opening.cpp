#include "Opening.h"
#include "Select.h"

Opening::Opening()
	:mSound(nullptr)
{
}

Opening::~Opening()
{
	delete objM;//d—v
}

void Opening::StartScene()
{
	objM = new ObjectManager();
	objM->Claer();
	camerapos = Vector3(0, 5, -10);
	setcamerapos = Vector3(0, 5, 0);
	pos = Vector3(5, -150, -100);
	fade2 = 0;
	fadeF = false;
	BaseScene::mSprite->AddTexture("Fade2", "Resouse/fade.png");
	BaseScene::mModel->AddModel("Open", "Resouse/Plane1.obj", "Resouse/opening.png");
	BaseScene::mModel->AddModel("Sora2", "Resouse/skybox.obj", "Resouse/skybox_A.png");
}

void Opening::UpdateScene()
{
	ImGui::Begin("pose");
	ImGui::DragFloat("selectflag", &pos.x,0,100);
	ImGui::DragFloat("pos", &pos.y , -50 , 1);
	ImGui::DragFloat("ctflag", &pos.z ,-10 ,1);
	ImGui::End();
	camera->SetEye(camerapos);
	camera->SetTarget(setcamerapos);
	pos.y += 0.05f;
	pos.z += 0.05f;
	if (pos.y >= -75)
	{
		fadeF = true;
		if (fadeF)
		{
			fade2 += 0.01;
			if (fade2 >= 1)
			{
				NextScene(std::make_shared<Select>());
			}
		}
	}
	if (Input::getKeyDown(KeyCode::Enter) || Input::getJoyDown(JoyCode::MenuButton))
	{
		NextScene(std::make_shared<Select>());
	}
}

void Opening::DrawScene()
{
	DirectXManager::GetInstance()->SetData3D();
	BaseScene::mModel->Draw("Open",pos, Vector3(47, 0 ,0), Vector3(100, 200, 1));	
	DirectXManager::GetInstance()->SetData2D();
	BaseScene::mSprite->Draw("Fade2", Vector3(0, 0, 0), 0.0f, Vector2(1, 1), Vector4(1, 1, 1, fade2));
	//BaseScene::mModel->Draw("Sora2", Vector3(0, 2.0f, 0), Vector3(0, 0, 0), Vector3(7, 7, 7));
}
