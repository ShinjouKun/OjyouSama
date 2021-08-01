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
	//�`��l��
	inline CollisonTypes GetCollType() { return collisonTypes; }
	inline void OnCollison(const CollisonInfo& info) { object->OnCollison(info); }//�R�[���o�b�N
protected:
	CollisonTypes collisonTypes = UNKNOWN;//�ŏ��͕s��
	BaseObject* object = nullptr;
};
