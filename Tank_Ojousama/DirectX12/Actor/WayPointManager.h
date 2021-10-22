#pragma once
#include <vector>
#include "ObjectManager.h"
#include "../Render/ModelRenderer.h"

class TestWayPoint;

class WayPointManager
{
public:

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="pos">���S�ʒu</param>
	/// <param name="objM">�I�u�W�F�N�g�}�l�[�W���[</param>
	/// <param name="mRender">���f���`��</param>
	WayPointManager(
		const Vector3 & pos,
		ObjectManager* objM,
		std::shared_ptr<ModelRenderer> mRender,
		const bool breadDraw
	);

	~WayPointManager();

	/*WayPoint���쐬*/
	void CreateWayPoint();

	/*���X�g�̖����ɗv�f��ǉ�*/
	void AddTest(std::shared_ptr<TestWayPoint> point);

	/*���X�g�̖����̗v�f���폜*/
	void Remove();

	//�S�Ẵ|�C���g�̃t���O��������
	void ResetFlag();

	/*PointList���擾*/
	std::vector<std::shared_ptr<TestWayPoint>> GetTestPointList()const;

	bool GetFinishFlag()const;

private:
	ObjectManager * objectManager;
	std::shared_ptr<ModelRenderer> modelRender;

	std::shared_ptr<TestWayPoint> testWay;//�e�|�C���g
	std::vector<std::shared_ptr<TestWayPoint>> testPointList;//�|�C���g�̃��X�g

	Vector3 position;

	int pointNumber = 0;

	int interval = 20;//�z�u�Ԋu
	float offsetWidth = 100.0f;
	float offsetHeight = 100.0f;
	float halfWidth;
	float halfHeight;

	bool finishCreatePoint;//�����������������H
	bool mBreadDraw;
};