#pragma once
#include <vector>
#include <string>
#include "TextEditer.h"
#include "../Render/ModelRenderer.h"

using namespace std;

enum HeadState
{
	Normal,
	Other01,
	Other02,
};

enum BodyState
{
	Light,
	Midium,
	Heavy,
};

enum BottomState
{
	Light_b,
	Midium_b,
	Heavy_b,
};

class ModelChanger
{
public:
	ModelChanger();
	~ModelChanger();

	void Init();
	void Load(shared_ptr<ModelRenderer> playerModel);
	void Save();

	void ChangeHead(HeadState headState);
	void ChangeBody(BodyState bodyState);
	void ChangeBottom(BottomState bottomState);

	HeadState GetHeadState() { return head; }
	BodyState GetBodyState() { return body; }
	BottomState GetBottomState() { return bottom; }

	string GetModelName(int num);//0`2‚¨ì—l,3íÔ(–C“ƒj,4íÔ(Ô‘Ìj

private:
	std::vector<std::string> state;
	std::vector<std::string> modelKey;
	HeadState head;
	BodyState body;
	BottomState bottom;
	TextEditor editor;
};
