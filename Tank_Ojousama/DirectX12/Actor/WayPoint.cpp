#include "WayPoint.h"
//
////�C���X�^���X���擾(����ŏo���Ă邩�͂킩��Ȃ����ǁA���������̂ł͂Ȃ��I)
//WayPointManager * WayPointManager::GetInstance()
//{
//	WayPointManager instance;
//
//	return &instance;
//}
//
////�t�B�[���h�Ƀ|�C���g���΂�܂�
//void WayPointManager::CreateWayPoint()
//{
//	for (int i = 0; i < offsetWidth; i++)
//	{
//		for (int j = 0; j < offsetHeight; j++)
//		{
//			wayPoint.position = Vector3(i - halfWidth, 0,i - halfHeight);
//
//			//�t���O�̏�����
//			ResetFlag();
//
//			//����ł�����25���邩��?
//			pointList.push_back(wayPoint);
//		}
//	}
//}
//
////Flag�ނ͊O���烊�Z�b�g�o���Ăق����̂�public�ɂ��Ă�
//void WayPointManager::ResetFlag()
//{
//	wayPoint.useFlag = false;
//	wayPoint.arrivalFlag = false;
//	wayPoint.impossibleFlag = false;
//	wayPoint.closeFlag = false;
//}
//
////�{���Ɉ�x�������������������̂������ɏ����B
//void WayPointManager::Initialize()
//{
//	pointList.clear();
//
//	interval = 10;
//
//	offsetWidth = 50.0f;
//	offsetHeight = 50.0f;
//	halfWidth = offsetWidth / 2.0f;
//	halfHeight = offsetHeight / 2.0f;
//
//	finishCreatePoint = false;
//}
