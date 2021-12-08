#include "ModelChanger.h"

ModelChanger::ModelChanger()
{
	state.clear();
	modelKey.clear();
	state.resize(4);
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
			
			playerModel->AddModel("OjyouSama_r", "Resouse/ojousama_body_red.obj", "Resouse/ojosama_red.png");
			playerModel->SetAncPoint("OjyouSama_r", Vector3(0.0f, 0.0f, -0.1f));
			
			if (state[3] == "MachinGun")
			{
				playerModel->AddModel("ArmR_rifle_r", "Resouse/R_hands_rifle_r.obj", "Resouse/rifle_hands_red.png");
				playerModel->SetAncPoint("ArmR_rifle_r", Vector3(0.0f, -2.1f, -0.1f));
				playerModel->AddModel("ArmL_rifle_r", "Resouse/L_hands_rifle_r.obj", "Resouse/rifle_hands_red.png");
				playerModel->SetAncPoint("ArmL_rifle_r", Vector3(0.0f, -2.1f, -0.1f));
				modelKey[0] = "ArmR_rifle_r";
				modelKey[2] = "Armr_rifle_r";
			}
			else
			{
				playerModel->AddModel("ArmR_r", "Resouse/R_hands.obj", "Resouse/hands_one.png");
				playerModel->SetAncPoint("ArmR_r", Vector3(0.0f, -2.1f, -0.1f));
				playerModel->AddModel("ArmL_r", "Resouse/L_hands.obj", "Resouse/hands_one.png");
				playerModel->SetAncPoint("ArmL_r", Vector3(0.0f, -2.1f, -0.1f));
				modelKey[0] = "ArmR_r";
				modelKey[2] = "ArmL_r";
			}
			
			modelKey[1] = "OjyouSama_r";
			
		}
		else
		{
			head = HeadState::Other01;
			playerModel->AddModel("OjyouSama2", "Resouse/ojousama_body_black.obj", "Resouse/ojosama_black.png");
			playerModel->SetAncPoint("OjyouSama2", Vector3(0.0f, 0.0f, -0.1f));

			if (state[3] == "MachinGun")
			{
				playerModel->AddModel("ArmR_rifle_b", "Resouse/R_hands_rifle_b.obj", "Resouse/rifle_hands_black.png");
				playerModel->SetAncPoint("ArmR_rifle_b", Vector3(0.0f, -2.1f, -0.1f));
				playerModel->AddModel("ArmL_rifle_b", "Resouse/L_hands_rifle_b.obj", "Resouse/rifle_hands_black.png");
				playerModel->SetAncPoint("ArmL_rifle_b", Vector3(0.0f, -2.1f, -0.1f));
				modelKey[0] = "ArmR_rifle_b";
				modelKey[2] = "ArmL_rifle_b";
			}
			else
			{
				playerModel->AddModel("ArmR_b", "Resouse/R_hands.obj", "Resouse/hands_one.png");
				playerModel->SetAncPoint("ArmR_b", Vector3(0.0f, -2.1f, -0.1f));
				playerModel->AddModel("ArmL_b", "Resouse/L_hands.obj", "Resouse/hands_one.png");
				playerModel->SetAncPoint("ArmL_b", Vector3(0.0f, -2.1f, -0.1f));
				modelKey[0] = "ArmR_b";
				modelKey[2] = "ArmL_b";
			}

			modelKey[1] = "OjyouSama2";
		}
	}
	else
	{
		head = HeadState::Normal;
		playerModel->AddModel("OjyouSama", "Resouse/ojosama_body.obj", "Resouse/ojosama_one.png");
		playerModel->SetAncPoint("OjyouSama", Vector3(0.0f, 0.0f, -0.1f));

		if (state[3] == "MachinGun")
		{
			playerModel->AddModel("ArmR_rifle", "Resouse/R_hands_rifle.obj", "Resouse/rifle_hands.png");
			playerModel->SetAncPoint("ArmR_rifle", Vector3(0.0f, -2.1f, -0.1f));
			playerModel->AddModel("ArmL_rifle", "Resouse/L_hands_rifle.obj", "Resouse/rifle_hands.png");
			playerModel->SetAncPoint("ArmL_rifle", Vector3(0.0f, -2.1f, -0.1f));
			modelKey[0] = "ArmR_rifle";
			modelKey[2] = "Armr_rifle";
		}
		else
		{
			playerModel->AddModel("ArmR", "Resouse/R_hands.obj", "Resouse/hands_one.png");
			playerModel->SetAncPoint("ArmR", Vector3(0.0f, -2.1f, -0.1f));
			playerModel->AddModel("ArmL", "Resouse/L_hands.obj", "Resouse/hands_one.png");
			playerModel->SetAncPoint("ArmL", Vector3(0.0f, -2.1f, -0.1f));
			modelKey[0] = "ArmR";
			modelKey[2] = "Armr";
		}

		modelKey[1] = "OjyouSama";
	}

	if (state[1] != "Light")
	{
		if (state[1] != "Midium")
		{
			SetHP(150);
			body = BodyState::Heavy;
			playerModel->AddModel("TankE", "Resouse/sensya_Type2_head.obj", "Resouse/sensya_type2_B.png");
			modelKey[3] = "TankE";
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
			modelKey[4] = "TankF";
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

	if (state[3] != "Cannon")
	{
		if (state[3] != "MachinGun")
		{
			if (state[3] != "ShotGun")
			{
				weapons1 = Mine;
			}
			else
			{
				weapons1 = ShotGun;
			}
		}
		else
		{
			weapons1 = MachinGun;
		}
	}
	else
	{
		weapons1 = Cannon;
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

	switch (weapons1)
	{
	case Cannon:
		state[3] = "Cannon";
		break;
	case MachinGun:
		state[3] = "MachinGun";
		break;
	case ShotGun:
		state[3] = "ShotGun";
		break;
	case Mine:
		state[3] = "Mine";
		break;
	default:
		state[3] = "Cannon";
		break;
	}
	
	
	editor->Write("Resouse/ModelState.txt", state);
	state.clear();
	state.resize(4);
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

void ModelChanger::ChangeWeapons1(WeaponsState weaponState)
{
	weapons1 = weaponState;
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
