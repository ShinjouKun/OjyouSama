#pragma once
#include"CollisonType.h"
#include "../Actor/BaseObject.h"
#include"CollisonInfo.h"
#include"../Collision/CollisonManager.h"
#include"../Collision/CollisonTree.h"
//imgui
#include"../imgui/imgui.h"
#include"../imgui/imgui_impl_win32.h"
#include"../imgui/imgui_impl_dx12.h"
class BaseCollider
{
public:
	BaseCollider()=default;
	virtual~BaseCollider() { object = nullptr; }
	inline void SetObject(BaseObject* obj) { this->object = obj; }
	inline BaseObject* GetColObject() {return object;}
	virtual void Update() = 0;
	//形状獲得
	inline CollisonTypes GetCollType() { return collisonTypes; }
	inline void OnCollison(BaseCollider* col) { if(object != nullptr)object->OnCollison(col); }//コールバック
	virtual void Regist() = 0;
	
protected:
	CollisonTypes collisonTypes = UNKNOWN;//最初は不明
	BaseObject* object = nullptr;
	CollisonTree* colT;
};
