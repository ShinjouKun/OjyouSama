#include "Garage.h"
#include"Select.h"
#include"Title.h"

Garage::Garage()
{
}

Garage::~Garage()
{
}

void Garage::StartScene()
{
}

void Garage::UpdateScene()
{
	if (Input::KeyDown(DIK_SPACE) || Input::pad_data.rgbButtons[2])
	{
		NextScene(std::make_shared<Select>());
	}
}

void Garage::DrawScene()
{
}
