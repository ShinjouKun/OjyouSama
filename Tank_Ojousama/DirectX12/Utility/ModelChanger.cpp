#include "ModelChanger.h"

ModelChanger::ModelChanger()
{
	state.clear();
	modelKey.clear();
	state.resize(3);
	modelKey.resize(5);
}

ModelChanger::~ModelChanger()
{
}

void ModelChanger::Init()
{
	head = HeadState::Normal;
	body = BodyState::Light;
	bottom = BottomState::Light_b;
	editor = new TextEditor();
	editor->Init();
	Save();
	
}

void ModelChanger::Load(shared_ptr<ModelRenderer> playerModel)
{
	state.clear();
	editor->Read("Resouse/ModelState.txt", state);
	
	if (state[0] != "Normal")
	{
		if (state[0] != "Other01")
		{
			head = HeadState::Other02;
			playerModel->AddModel("ArmR", "Resouse/R_hands.obj", "Resouse/hands_one.png");
			playerModel->SetAncPoint("ArmR", Vector3(0.0f, -2.1f, -0.1f));
			playerModel->AddModel("OjyouSama", "Resouse/ojosama_body.obj", "Resouse/ojosama_one.png");
			playerModel->SetAncPoint("OjyouSama", Vector3(0.0f, 0.0f, -0.1f));
			playerModel->AddModel("ArmL", "Resouse/L_hands.obj", "Resouse/hands_one.png");
			playerModel->SetAncPoint("ArmL", Vector3(0.0f, -2.1f, -0.1f));
			modelKey[0] = "ArmR";
			modelKey[1] = "OjyouSama";
			modelKey[2] = "ArmL";
		}
		else
		{
			head = HeadState::Other01;
			playerModel->AddModel("ArmR2", "Resouse/R_hands.obj", "Resouse/hands_one.png");
			playerModel->SetAncPoint("ArmR2", Vector3(0.0f, -2.1f, -0.1f));
			playerModel->AddModel("OjyouSama2", "Resouse/ojosama_body.obj", "Resouse/ojosama_one.png");
			playerModel->SetAncPoint("OjyouSama2", Vector3(0.0f, 0.0f, -0.1f));
			playerModel->AddModel("ArmL2", "Resouse/L_hands.obj", "Resouse/hands_one.png");
			playerModel->SetAncPoint("ArmL2", Vector3(0.0f, -2.1f, -0.1f));
			modelKey[0] = "ArmR2";
			modelKey[1] = "OjyouSama2";
			modelKey[2] = "ArmL2";
		}
	}
	else
	{
		head = HeadState::Normal;
		playerModel->AddModel("ArmR", "Resouse/R_hands.obj", "Resouse/hands_one.png");
		playerModel->SetAncPoint("ArmR", Vector3(0.0f, -2.1f, -0.1f));
		playerModel->AddModel("OjyouSama", "Resouse/ojosama_body.obj", "Resouse/ojosama_one.png");
		playerModel->SetAncPoint("OjyouSama", Vector3(0.0f, 0.0f, -0.1f));
		playerModel->AddModel("ArmL", "Resouse/L_hands.obj", "Resouse/hands_one.png");
		playerModel->SetAncPoint("ArmL", Vector3(0.0f, -2.1f, -0.1f));
		modelKey[0] = "ArmR";
		modelKey[1] = "OjyouSama";
		modelKey[2] = "ArmL";
	}

	if (state[1] != "Light")
	{
		if (state[1] != "Midium")
		{
			SetHP(150);
			body = BodyState::Heavy;
			playerModel->AddModel("TankE", "Resouse/sensya_Type2_head.obj", "Resouse/sensya_type2_B.png");
			modelKey[3] = "TankA";
		}
		else
		{
			SetHP(100);
			body = BodyState::Midium;
			playerModel->AddModel("TankPlayerC", "Resouse/big_sensha_head.obj", "Resouse/big_sensha.png");
			modelKey[3] = "TankPlayerC";
		}
	}
	else
	{
		SetHP(70);
		body = BodyState::Light;
		playerModel->AddModel("TankA", "Resouse/houtou.obj", "Resouse/sensha_A.png");
		modelKey[3] = "TankA";
		
	}

	if (state[2] != "Light_b")
	{
		if (state[2] != "Midium_b")
		{
			SetSpeed(0.3f);
			bottom = BottomState::Heavy_b;
			playerModel->AddModel("TankF", "Resouse/sensya_Typ2_body.obj", "Resouse/sensya_type2_B.png");
			modelKey[4] = "TankB";
		}
		else
		{
			SetSpeed(0.5f);
			bottom = BottomState::Midium_b;
			playerModel->AddModel("TankPlayerD", "Resouse/big_sensha_body.obj", "Resouse/big_sensha.png");
			modelKey[4] = "TankPlayerD";
		}
	}
	else
	{
		SetSpeed(0.8f);
		bottom = BottomState::Light_b;
		playerModel->AddModel("TankB", "Resouse/sensha_body.obj", "Resouse/sensha_A.png");
		modelKey[4] = "TankB";
	}
	
	
}

void ModelChanger::Save()
{
	switch (head)
	{
	case Normal:
		state[0] = "Normal";
		break;
	case Other01:
		state[0] = "Other01";
		break;
	case Other02:
		state[0] = "Other02";
		break;
	default:
		state[0] = "Normal";
		break;
	}

	switch (body)
	{
	case Light:
		state[1] = "Light";
		break;
	case Midium:
		state[1] = "Midium";
		break;
	case Heavy:
		state[1] = "Heavy";
		break;
	default:
		state[1] = "Light";
		break;
	}

	switch (bottom)
	{
	case Light_b:
		state[2] = "Light_b";
		
		break;
	case Midium_b:
		state[2] = "Midium_b";
	
		break;
	case Heavy_b:
		state[2] = "Heavy_b";
		
		break;
	default:
		state[2] = "Light_b";
		break;
	}

	editor->Write("Resouse/ModelState.txt", state);
	state.clear();
	state.resize(3);
}


void ModelChanger::ChangeHead(HeadState headState)
{
	head = headState;
}

void ModelChanger::ChangeBody(BodyState bodyState)
{
	body = bodyState;
}

void ModelChanger::ChangeBottom(BottomState bottomState)
{
	bottom = bottomState;
}

void ModelChanger::SetHP(int value)
{
	hp = value;
}

void ModelChanger::SetSpeed(float value)
{
	speed = value;
}

string ModelChanger::GetModelName(int num)
{
	return modelKey[num];
}
