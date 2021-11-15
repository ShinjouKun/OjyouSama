#pragma once
#include "TextEditer.h"

enum Head
{
	Normal,
	Other01,
	Other02,
};

enum Body
{
	Light,
	Midium,
	Heavy,
};

enum Bottom
{
	Light,
	Midium,
	Heavy,
};

class ModelChanger
{
public:
	ModelChanger();
	~ModelChanger();

	void Init();
	void Load();
	void Save();

	void ChangeHead(Head& head);
	void ChangeBody(Body& body);
	void ChangeBottom(Bottom bottom);

private:
	vector<string> state;
	Head head;
	Body body;
	Bottom bottom;
	TextEditor editor;
};
