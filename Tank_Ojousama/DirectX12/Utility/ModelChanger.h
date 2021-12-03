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

	void SetHP(int value);//体力
	int GetHP() { return hp; }
	void SetSpeed(float value);//速度
	float GetSpeed() { return speed; }

	string GetModelName(int num);//0～2お嬢様,3戦車(砲塔）,4戦車(車体）

	int hp;
	float speed;
private:
	
	std::vector<std::string> state{ 3 };
	std::vector<std::string> modelKey{ 5 };
	HeadState head;
	BodyState body;
	BottomState bottom;
	TextEditor* editor;
};
