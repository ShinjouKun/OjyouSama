#pragma once
#include "BaseObject.h"             //�I�u�W�F�N�g�̂ЂȌ^
#include "ObjectManager.h"          //�I�u�W�F�N�g�Ǘ���
#include "BreadCrumb.h"             //�p������
#include "../Render/ModelRenderer.h"//���f���\��t��
#include "../Render/TexRenderer.h"	//�|���S���̕`��

class WayPoint : public BaseObject
{
public:
	/// <summary>
	/// �R���X�g���N�g
	/// </summary>
	/// <param name="position">�����ʒu</param>
	/// <param name="objManager">�I�u�W�F�N�g�Ǘ���</param>
	/// <param name="modelRender">���f���`��</param>
	/// <param name="num">���ʔԍ�</param>
	WayPoint(
		Vector3 position,
		ObjectManager * objManager,
		shared_ptr<ModelRenderer> modelRender,
		int num
	);

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	~WayPoint();
};