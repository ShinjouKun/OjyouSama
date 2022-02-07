#include "ModelChanger.h"
#include"../Scene/BaseScene.h"
ModelChanger::ModelChanger()
{
	state.clear();
	modelKey.clear();
	state.resize(4);
	modelKey.resize(5);
	//buys.clear();
	buys.resize(12);
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
	buymoney = 0;
	balance = BaseScene::mMoney;
	stopBuy = false;
	gotoBuy = false;
	Save();
	SetHP(100);
	SetUpDamage(0);
	SetSpeed(0.5);
}

void ModelChanger::Load(shared_ptr<ModelRenderer> playerModel)
{
	state.clear();
	
	editor->Read("Resouse/ModelState.txt", state);
	

	if (state[0] != "Normal")
	{
		if (state[0] != "Other01")
		{
			SetHP(120);
			head = HeadState::Other02;
			
			playerModel->AddModel("OjyouSama_r", "Resouse/ojousama_body_red.obj", "Resouse/ojosama_red.png");
			playerModel->SetAncPoint("OjyouSama_r", Vector3(0.0f, 0.0f, -0.1f));
			
			if (state[3] == "MachinGun")
			{
				playerModel->AddModel("ArmR_rifle_r", "Resouse/R_hands_rifle_r.obj", "Resouse/rifle_hands_red.png");
				playerModel->SetAncPoint("ArmR_rifle_r", Vector3(0.6f, -2.1f, -0.5f));
				playerModel->AddModel("ArmL_rifle_r", "Resouse/L_hands_rifle_r.obj", "Resouse/rifle_hands_red.png");
				playerModel->SetAncPoint("ArmL_rifle_r", Vector3(-0.6f, -2.1f, -0.5f));
				modelKey[0] = "ArmR_rifle_r";
				modelKey[2] = "ArmL_rifle_r";
			}
			else if (state[3] == "ShotGun")
			{
				playerModel->AddModel("ArmR_shotgun_r", "Resouse/sg_ojosama_R_r.obj", "Resouse/sg_ojosama_R.png");
				playerModel->SetAncPoint("ArmR_shotgun_r", Vector3(0.6f, -2.1f, -0.5f));
				playerModel->AddModel("ArmL_shotgun_r", "Resouse/sg_ojosama_L_r.obj", "Resouse/sg_ojosama_R.png");
				playerModel->SetAncPoint("ArmL_shotgun_r", Vector3(-0.6f, -2.1f, -0.5f));
				modelKey[0] = "ArmR_shotgun_r";
				modelKey[2] = "ArmL_shotgun_r";
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
			SetHP(150);
			head = HeadState::Other01;
			playerModel->AddModel("OjyouSama2", "Resouse/ojousama_body_black.obj", "Resouse/ojosama_black.png");
			playerModel->SetAncPoint("OjyouSama2", Vector3(0.0f, 0.0f, -0.1f));

			if (state[3] == "MachinGun")
			{
				playerModel->AddModel("ArmR_rifle_b", "Resouse/R_hands_rifle_b.obj", "Resouse/rifle_hands_black.png");
				playerModel->SetAncPoint("ArmR_rifle_b", Vector3(0.6f, -2.1f, -0.5f));
				playerModel->AddModel("ArmL_rifle_b", "Resouse/L_hands_rifle_b.obj", "Resouse/rifle_hands_black.png");
				playerModel->SetAncPoint("ArmL_rifle_b", Vector3(-0.6f, -2.1f, -0.5f));
				modelKey[0] = "ArmR_rifle_b";
				modelKey[2] = "ArmL_rifle_b";
			}
			else if (state[3] == "ShotGun")
			{
				playerModel->AddModel("ArmR_shotgun_b", "Resouse/sg_ojosama_R_b.obj", "Resouse/sg_ojosama_B.png");
				playerModel->SetAncPoint("ArmR_shotgun_b", Vector3(0.6f, -2.1f, -0.5f));
				playerModel->AddModel("ArmL_shotgun_b", "Resouse/sg_ojosama_L_b.obj", "Resouse/sg_ojosama_B.png");
				playerModel->SetAncPoint("ArmL_shotgun_b", Vector3(-0.6f, -2.1f, -0.5f));
				modelKey[0] = "ArmR_shotgun_b";
				modelKey[2] = "ArmL_shotgun_b";
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
		SetHP(100);
		head = HeadState::Normal;
		playerModel->AddModel("OjyouSama", "Resouse/ojosama_body.obj", "Resouse/ojosama_one.png");
		playerModel->SetAncPoint("OjyouSama", Vector3(0.0f, 0.0f, -0.1f));

		if (state[3] == "MachinGun")
		{
			playerModel->AddModel("ArmR_rifle", "Resouse/R_hands_rifle.obj", "Resouse/rifle_hands.png");
			playerModel->SetAncPoint("ArmR_rifle", Vector3(0.6f, -2.1f, -0.5f));
			playerModel->AddModel("ArmL_rifle", "Resouse/L_hands_rifle.obj", "Resouse/rifle_hands.png");
			playerModel->SetAncPoint("ArmL_rifle", Vector3(-0.6f, -2.1f, -0.5f));
			modelKey[0] = "ArmR_rifle";
			modelKey[2] = "ArmL_rifle";
		}
		else if (state[3] == "ShotGun")
		{
			playerModel->AddModel("ArmR_shotgun", "Resouse/sg_ojosama_R.obj", "Resouse/sg_ojosama.png");
			playerModel->SetAncPoint("ArmR_shotgun", Vector3(0.6f, -2.1f, -0.5f));
			playerModel->AddModel("ArmL_shotgun", "Resouse/sg_ojosama_L.obj", "Resouse/sg_ojosama.png");
			playerModel->SetAncPoint("ArmL_shotgun", Vector3(-0.6f, -2.1f, -0.5f));
			modelKey[0] = "ArmR_shotgun";
			modelKey[2] = "ArmL_shotgun";
		}
		else
		{
			playerModel->AddModel("ArmR", "Resouse/R_hands.obj", "Resouse/hands_one.png");
			playerModel->SetAncPoint("ArmR", Vector3(0.0f, -2.1f, -0.1f));
			playerModel->AddModel("ArmL", "Resouse/L_hands.obj", "Resouse/hands_one.png");
			playerModel->SetAncPoint("ArmL", Vector3(0.0f, -2.1f, -0.1f));
			modelKey[0] = "ArmR";
			modelKey[2] = "ArmL";
		}

		modelKey[1] = "OjyouSama";
	}

	if (state[1] != "Light")
	{
		if (state[1] != "Midium")
		{
			SetUpDamage(10);
			body = BodyState::Heavy;
			playerModel->AddModel("TankE", "Resouse/big_sensha_head.obj", "Resouse/big_sensha.png");
			modelKey[3] = "TankE";
		}
		else
		{
			SetUpDamage(5);
			body = BodyState::Midium;
			playerModel->AddModel("TankD", "Resouse/sensya_Type2_head.obj", "Resouse/sensya_type2.png");
			modelKey[3] = "TankD";
		}
	}
	else
	{
		SetUpDamage(0);
		body = BodyState::Light;
		playerModel->AddModel("TankA", "Resouse/houtou.obj", "Resouse/sensha_A.png");
		modelKey[3] = "TankA";
		
	}

	if (state[2] != "Light_b")
	{
		if (state[2] != "Midium_b")
		{
			SetSpeed(1.0f);
			bottom = BottomState::Heavy_b;
			playerModel->AddModel("TankF", "Resouse/big_sensha_body.obj", "Resouse/big_sensha.png");
			modelKey[4] = "TankF";
		}
		else
		{
			SetSpeed(0.8f);
			bottom = BottomState::Midium_b;
			playerModel->AddModel("TankC", "Resouse/sensya_Typ2_body.obj", "Resouse/sensya_type2.png");
			modelKey[4] = "TankC";
		}
	}
	else
	{
		SetSpeed(0.5f);
		bottom = BottomState::Light_b;
		playerModel->AddModel("TankB", "Resouse/sensha_body.obj", "Resouse/sensha_A.png");
		modelKey[4] = "TankB";
	}

	if (state[3] != "MachinGun")
	{
		if (state[3] != "ShotGun")
		{
			SetWeaponNum(3);
			weapons1 = Mine;
		}
		else
		{
			SetWeaponNum(2);
			weapons1 = ShotGun;
		}
	}
	else
	{
		SetWeaponNum(1);
		weapons1 = MachinGun;
	}
}

void ModelChanger::Save()
{
	buys.clear();
	editor->Read("Resouse/BuysState.txt", buys);
	switch (head)
	{
	case Normal:
		state[0] = "Normal";
		buys[0] = 1;
		break;
	case Other01:
		if (buys[2] == "nonBuy"&&BaseScene::mMoney >= 0)
		{
			state[0] = "Normal";
		}
		else
		{
			state[0] = "Other01";
		}
		break;
	case Other02:
		if (buys[1] == "nonBuy" && BaseScene::mMoney >= 0)
		{
			state[0] = "Normal";
		}
		else
		{
			state[0] = "Other02";
		}
		break;
	default:
		state[0] = "Normal";
		break;
	}

	switch (body)
	{
	case Light:
		state[1] = "Light";
		buys[3] = 1;
		break;
	case Midium:
		if (buys[4] == "nonBuy" && BaseScene::mMoney >= 0)
		{
			state[1] = "Light";
		}
		else
		{
			state[1] = "Midium";
		}
		break;
	case Heavy:
		if (buys[5] == "nonBuy"&& BaseScene::mMoney >= 0)
		{
			state[1] = "Light";
		}
		else
		{
			state[1] = "Heavy";
		}
		break;
	default:
		state[1] = "Light";
		break;
	}

	switch (bottom)
	{
	case Light_b:
		buys[6] = 1;
		state[2] = "Light_b";
		
		break;
	case Midium_b:
		if (buys[7] == "nonBuy"&& BaseScene::mMoney >= 0)
		{
			state[2] = "Light_b";
		}
		else
		{
			state[2] = "Midium_b";
		}
		break;
	case Heavy_b:
		if (buys[8] == "nonBuy"&& BaseScene::mMoney >= 0)
		{
			state[2] = "Light_b";
		}
		else
		{
			state[2] = "Heavy_b";
		}
		break;
	default:
		state[2] = "Light_b";
		break;
	}

	switch (weapons1)
	{
		state[3] = "Cannon";
		break;
	case MachinGun:
		if (buys[9] == "nonBuy" && BaseScene::mMoney >= 0)
		{
			buys[9] = 1;
			buymoney += 0;
		}
		state[3] = "MachinGun";
		break;
	case ShotGun:
		if (buys[10] == "nonBuy" && BaseScene::mMoney >= 0)
		{
			state[3] = "Cannon";
		}
		else
		{
			state[3] = "ShotGun";
		}
		break;
	case Mine:
		if (buys[11] == "nonBuy" && BaseScene::mMoney >= 0)
		{
			state[3] = "Cannon";
		}
		else
		{
			state[3] = "Mine";
		}
		break;
	default:
		state[3] = "Cannon";
		break;
	}
	

	
	editor->Write("Resouse/ModelState.txt", state);
	state.clear();
	state.resize(4);
	
}

void ModelChanger::Buys()
{
	buys.clear();
	buymoney = 0;
	editor->Read("Resouse/BuysState.txt", buys);
	switch (head)
	{
	case Normal:
		state[0] = "Normal";
		buys[0] = 1;
		break;
	case Other01:
		state[0] = "Other01";

		if (buys[2] == "nonBuy"&&BaseScene::mMoney >= 0)
		{
			buys[2] = 1;
			buymoney += 4000000;
		}
		break;
	case Other02:
		if (buys[1] == "nonBuy" && BaseScene::mMoney >= 0)
		{
			buys[1] = 1;
			buymoney += 2000000;
		}
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
		buys[3] = 1;
		break;
	case Midium:
		if (buys[4] == "nonBuy" && BaseScene::mMoney >= 0)
		{
			buys[4] = 1;
			buymoney += 2000000;
		}
		state[1] = "Midium";
		break;
	case Heavy:
		if (buys[5] == "nonBuy"&& BaseScene::mMoney >= 0)
		{
			buys[5] = 1;
			buymoney += 4000000;
		}
		state[1] = "Heavy";
		break;
	default:
		state[1] = "Light";
		break;
	}

	switch (bottom)
	{
	case Light_b:
		buys[6] = 1;
		state[2] = "Light_b";

		break;
	case Midium_b:
		if (buys[7] == "nonBuy"&& BaseScene::mMoney >= 0)
		{
			buys[7] = 1;
			buymoney += 2000000;
		}
		state[2] = "Midium_b";

		break;
	case Heavy_b:
		if (buys[8] == "nonBuy"&& BaseScene::mMoney >= 0)
		{
			buys[8] = 1;
			buymoney += 4000000;
		}
		state[2] = "Heavy_b";

		break;
	default:
		state[2] = "Light_b";
		break;
	}

	switch (weapons1)
	{
	case MachinGun:
		if (buys[9] == "nonBuy" && BaseScene::mMoney >= 0)
		{
			buys[9] = 1;
			buymoney += 0;
		}
		state[3] = "MachinGun";
		break;
	case ShotGun:
		if (buys[11] == "nonBuy" && BaseScene::mMoney >= 0)
		{
			buys[11] = 1;
			buymoney += 500000;
		}
		state[3] = "ShotGun";
		break;
	case Mine:
		if (buys[10] == "nonBuy" && BaseScene::mMoney >= 0)
		{
			buys[10] = 1;
			buymoney += 200000;
		}
		state[3] = "Mine";
		break;
	default:
		state[3] = "Cannon";
		break;
	}

	if (balance - buymoney <= 0)
	{
		stopBuy = true;
	}
	
	if (gotoBuy)
	{
		stopBuy = false;
	}

	if (!stopBuy)
	{
		BaseScene::mMoney -= buymoney;
		balance = BaseScene::mMoney;
		editor->Write("Resouse/BuysState.txt", buys);
		buys.clear();
		buys.resize(12);
	}

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

void ModelChanger::SetUpDamage(int up)
{
	upDamage = up;
}

void ModelChanger::SetSpeed(float value)
{
	speed = value;
}

void ModelChanger::SetWeaponNum(int num)
{
	Weapon = num;
}

string ModelChanger::GetModelName(int num)
{
	return modelKey[num];
}

void ModelChanger::SetBuysNum(string buy)
{
	for (int i = 0;i == buys.size()-1;i++)
	{
		buys[i] = buy;
	}
	editor->Write("Resouse/BuysState.txt", buys);
}
string ModelChanger::GetBuysNum(int buy)
{
	return buys[buy];
}

void ModelChanger::SetGotoBuy(bool value)
{
	stopBuy = false;
	gotoBuy = value;
}


void ModelChanger::LoadBuys()
{
	buys.clear();
	editor->Read("Resouse/BuysState.txt", buys);

}
