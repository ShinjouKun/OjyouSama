#pragma once
#include "../Math/Vector3.h"
#include "ObjectManager.h"
#include <unordered_map>
#include "BaseObject.h"

class TestBreadCrumb;

class BreadCrumbCreater
{
public:
	BreadCrumbCreater(
		ObjectManager* objM
	);

	~BreadCrumbCreater();

	void DropBreadCrumb();

	/*ƒŠƒXƒg‚ðŽæ“¾*/
	std::vector<std::shared_ptr<TestBreadCrumb>> GetBreadList() const;

private:

	void Initialize();

	ObjectManager* objManager;
	Vector3 playerPosition;


	std::vector<std::shared_ptr<TestBreadCrumb>> mBreadList;

	std::unordered_map<int, Vector3> measureMap;
	int breadNumber;
	int intervalCount;
	int intervalTime;
};