#pragma once
#include "Item.h"

class Repair :public Item
{
public:
	Repair();
	~Repair();

private:

	virtual void Init() override;
	virtual void Update() override;
	virtual void Rend() override;

};