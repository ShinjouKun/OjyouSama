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

	void Draw();//�`�揈��

	std::vector<BaseObject*>getUseList();

	//�u1=P�L�[�ŗ��Ƃ��v�u2=�����ŗ��Ƃ��v�u3=���Ԃŗ��Ƃ��v
	void DropBreadCrumb(int status);

	BaseObject& GetPlayer()const;

private:
	std::vector<BaseObject*>objectList;//�g������
	std::vector<BaseObject*>addObjectList;//�ǉ��������̂̈ꎞ�ۑ��p
	int IdCount;

	//�p��������������p�}�b�v
	std::unordered_map<int, Vector3> measureMap;
	//�p�������̎��ʔԍ�
	int breadNumber = 0;
	//�p�������𗎂Ƃ��Ԋu
	int intervalCount = 0;
	int intervalTime = 1;
};
