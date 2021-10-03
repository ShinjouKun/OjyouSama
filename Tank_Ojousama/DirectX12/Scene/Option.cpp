#include "Option.h"
#include "Garage.h"
#include"Select.h"
#include"Title.h"

Option::Option()
{
}

Option::~Option()
{
}

void Option::StartScene()
{
}

void Option::UpdateScene()
{
	if (Input::KeyDown(DIK_SPACE) || Input::pad_data.rgbButtons[2])
	{
		NextScene(std::make_shared<Select>());
	}
}

void Option::DrawScene()
{
}
