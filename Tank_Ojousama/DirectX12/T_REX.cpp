#include "T_REX.h"

T_REX::T_REX(Vector3 pos, Vector3 ang, ObjectManager * obj, shared_ptr<ModelRenderer> m, shared_ptr<ParticleManager> p)
	:REXModel(m),REXParticle(p)
{
	position = pos;
	angle = ang;
	objM = obj;
}

T_REX::~T_REX()
{
}

void T_REX::Fire()
{
}

void T_REX::Init()
{
	death = false;
	speed = 1.5f;
	angle.y = 90.0f;
	angle.x = 0.0f;
	angleL = -100.0f;
	angleR = 20.0f;
	KutiAngle = -25.0f;
	objType = ObjectType::ENEMY;
	REXModel->AddModel("REXA", "Resouse/TLEX_A.obj", "Resouse/TLEX_A.png");
	REXModel->SetAncPoint("REXA",Vector3(-2.0f,-2.0f,-4.0f));
	REXModel->AddModel("REXB", "Resouse/TLEX_B.obj", "Resouse/TLEX_B.png");
	REXModel->SetAncPoint("REXB", Vector3(-2.0f, -3.8f, -4.4f));
	REXModel->AddModel("REXR", "Resouse/TLEX_asiR.obj", "Resouse/TLEX_asiR.png");
	REXModel->SetAncPoint("REXR", Vector3(-2.0f, -3.8f, -4.0f));
	REXModel->AddModel("REXL", "Resouse/TLEX_asiL.obj", "Resouse/TLEX_asiL.png");
	REXModel->SetAncPoint("REXL", Vector3(-2.0f,-3.8f, -4.0f));

	REXParticleBox = make_shared<ParticleEmitterBox>(REXParticle);
	REXParticleBox->LoadAndSet("F", "Resouse/Bom.jpg");
	velocity = Vector3(0, 0, 0);
}

void T_REX::Update()
{
	velocity = RotateY(-angle.y + 90.0f)*speed;
	position -= velocity;
	angle.y += 1.0f;
}

void T_REX::Rend()
{
	angleL -= 20.0f;
	angleR -= 20.0f;
	DirectXManager::GetInstance()->SetData3D();
	REXModel->Draw("REXA", position, Vector3(angle.x, angle.y, angle.z), Vector3(15, 15, 15));
	REXModel->Draw("REXB", Vector3(position.x, position.y + 37.0f, position.z), Vector3(KutiAngle, angle.y, angle.z), Vector3(15, 15, 15));
	REXModel->Draw("REXL", Vector3(position.x,position.y+26.0f,position.z), Vector3(angleL, angle.y, angle.z), Vector3(15, 15, 15));
	REXModel->Draw("REXR", Vector3(position.x, position.y+26.0f, position.z), Vector3(angleR, angle.y, angle.z), Vector3(15, 15, 15));
	
	REXParticleBox->EmitterUpdateFireGas("F", Vector3(position.x, position.y + 40.0f, position.z), Vector3(angle.x, angle.y, angle.z));
}


void T_REX::OnCollison(const CollisonInfo & info)
{
}

void T_REX::ImGuiDebug()
{
}
