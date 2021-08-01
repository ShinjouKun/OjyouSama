#pragma once
#include"CollisonType.h"
#include"BaseObject.h"
#include"CollisonInfo.h"
class BaseCollider
{
public:
	BaseCollider()=default;
	virtual~BaseCollider()=default;
	inline void SetObject(BaseObject* obj) { this->object = obj; }
	inline BaseObject* GetObject() {return object;}
	virtual void Update() = 0;
	//形状獲得
	inline CollisonTypes GetCollType() { return collisonTypes; }
	inline void OnCollison(const CollisonInfo& info) { object->OnCollison(info); }//コールバック
protected:
	CollisonTypes collisonTypes = UNKNOWN;//最初は不明
	BaseObject* object = nullptr;
};
