#pragma once
#include<vector>
#include<memory>
#include"BaseObject.h"
#include "../Device/DirectXManager.h"
#include "../Collision/Collision.h"

class ObjectManager
{
public:
	ObjectManager();
	~ObjectManager();
	void Claer();//�e���X�g�̏�����
	void Add(BaseObject* addObj);//���X�g�ւ̒ǉ�
	void Update();//���X�V
	void AddListUpdate();//�ǉ��������X�g�̍X�V
	void ObjectManagerUpdate();//���X�g���̃I�u�W�F�N�g�̍X�V
	void RemoveListUpdate();//���񂾂��̂Ȃǂ̍X�V����
	int GetReloadTime() { return ReloadTime; }
	void SetReloadTime(int time) { ReloadTime = time; }
	void Draw();//�`�揈��
	std::vector<BaseObject*>getUseList();

	BaseObject & GetPlayer() const;
	BaseObject & GetGolem() const;
	BaseObject & GetEnemy() const;
private:
	std::vector<BaseObject*>objectList;//�g������
	std::vector<BaseObject*>addObjectList;//�ǉ��������̂̈ꎞ�ۑ��p
	
	int IdCount;
	int ReloadTime;
};
