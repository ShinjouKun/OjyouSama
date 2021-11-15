#pragma once
#include "TextEditer.h"

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
	void Load();
	void Save();

	void ChangeHead(HeadState headState);
	void ChangeBody(BodyState bodyState);
	void ChangeBottom(BottomState bottomState);

	HeadState GetHeadState() { return head; }
	BodyState GetBodyState() { return body; }
	BottomState GetBottomState() { return bottom; }

private:
	vector<string> state;
	HeadState head;
	BodyState body;
	BottomState bottom;
	TextEditor editor;
};
