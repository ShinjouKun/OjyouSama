#include "ModelChanger.h"

ModelChanger::ModelChanger()
{
}

ModelChanger::~ModelChanger()
{
}

void ModelChanger::Init()
{
	head = HeadState::Normal;
	body = BodyState::Light;
	bottom = BottomState::Light_b;
	
}

void ModelChanger::Load(shared_ptr<ModelRenderer> playerModel)
{
	editor.Read("Resource/ModelState.txt", state);
	
	if (state[0] != "Normal")
	{
		if (state[0] != "Other01")
		{
			head = HeadState::Other02;
		}
		else
		{
			head = HeadState::Other01;
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
			body = BodyState::Heavy;
		}
		else
		{
			body = BodyState::Midium;
		}
	}
	else
	{
		body = BodyState::Light;
		playerModel->AddModel("TankA", "Resouse/houtou.obj", "Resouse/sensha_A.png");
		modelKey[3] = "TankA";
		
	}

	if (state[2] != "Light_b")
	{
		if (state[1] != "Midium_b")
		{
			bottom = BottomState::Heavy_b;
		}
		else
		{
			bottom = BottomState::Midium_b;
		}
	}
	else
	{
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
		cout << "error :>> OutOfRange." << endl;
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
		cout << "error :>> OutOfRange." << endl;
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
		cout << "error :>> OutOfRange." << endl;
		break;
	}

	editor.Write("Resource/ModelState.txt", state);
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

string ModelChanger::GetModelName(int num)
{
	return modelKey[num];
}
