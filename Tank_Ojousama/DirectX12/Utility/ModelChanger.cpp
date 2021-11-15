#include "ModelChanger.h"

ModelChanger::ModelChanger()
{
}

ModelChanger::~ModelChanger()
{
}

void ModelChanger::Init()
{
}

void ModelChanger::Load()
{
	editor.Read("../Resource/ModelState.txt", state);
	
	
}

void ModelChanger::Save()
{
	editor.Write("../Resource/ModelState.txt", state);
}

void ModelChanger::ChangeHead(Head & head)
{
	switch (head)
	{
	case Normal:
		break;
	case Other01:
		break;
	case Other02:
		break;
	default:
		cout << "error :>> OutOfRange." << endl;
		break;
	}
}

void ModelChanger::ChangeBody(Body& body)
{
	switch (body)
	{
	case Light:
		break;
	case Midium:
		break;
	case Heavy:
		break;
	default:
		cout << "error :>> OutOfRange." << endl;
		break;
	}
}

void ModelChanger::ChangeBottom(Bottom bottom)
{
	switch (bottom)
	{
	case Light:
		break;
	case Midium:
		break;
	case Heavy:
		break;
	default:
		cout << "error :>> OutOfRange." << endl;
		break;
	}
}
