//#pragma once
//#include "BaseObject.h"             //�I�u�W�F�N�g�̂ЂȌ^
//#include "ObjectManager.h"          //�I�u�W�F�N�g�Ǘ���
//#include "../Render/ModelRenderer.h"
//
//class WayPoint : public BaseObject
//{
//public:
//	/// <summary>
//	/// �R���X�g���N�g
//	/// </summary>
//	/// <param name="position">�����ʒu</param>
//	/// <param name="objManager">�I�u�W�F�N�g�Ǘ���</param>
//	/// <param name="modelRender">���f���`��</param>
//	/// <param name="num">���ʔԍ�</param>
//	WayPoint(
//		const Vector3 & pos,
//		ObjectManager * objManager,
//		shared_ptr<ModelRenderer> modelRender,
//		int num
//	);
//
//	//�f�t�H���g�R���X�g���N�^
//	WayPoint();
//
//	/// <summary>
//	/// �f�X�g���N�^
//	/// </summary>
//	~WayPoint();
//
//	void FlagInit();
//
//	bool GetUseFlag() { return useFlag; }
//	bool GetImpossibleFlag() { return impossibleFlag; }
//	bool GetArrivalFlag() { return arrivalFlag; }
//	bool GetCloseFlag() { return closeFlag; }
//
//	void SetUseFlag(bool value) { useFlag = value; }
//	void SetCloseFlag(bool value) { closeFlag = value; }
//	void SetImpossibleFlag(bool value) { impossibleFlag = value; }
//	void SetDeathFlag(bool value) { death = value; }
//
//private:
//	virtual void Init() override;
//	virtual void Update() override;
//	virtual void Rend() override;
//	virtual void OnCollison(BaseCollider* col) override;
//	virtual void ImGuiDebug() override;
//
//private:
//
//	ObjectManager* objectManager;
//	shared_ptr<ModelRenderer> modelRenderer;
//
//	int keyNumber = 0;
//
//	bool useFlag;
//	bool arrivalFlag;
//	bool impossibleFlag;
//	bool closeFlag;
//
//	string key;	   //
//	string name;   //
//	string keyName;//
//};