#include"../DirectX12/Collision/CollisonTree.h"
#include"../DirectX12/Collision/WorldSpace.h"
CollisonTree::CollisonTree()
{
	wSpace = NULL;
	colObject = NULL;
	m_spNext = nullptr;
	m_spPre = nullptr;
}

CollisonTree::~CollisonTree()
{
	Remove();
}

bool CollisonTree::Remove()
{
	if (!wSpace)//���łɈ�E���Ă���ꍇ
		return false;
	
	if (!wSpace->OnRemove(this))//������o�^���Ă����ԂɎ��g��ʒm
		return false;
	//��E����
	//�O��̃I�u�W�F�N�g�����т���
	if (m_spPre != nullptr)
	{
		m_spPre->m_spNext = m_spNext;
	}
	if (m_spNext != nullptr)
	{
		m_spNext->m_spPre = m_spPre;
	}
	m_spNext = nullptr;
	m_spPre = nullptr;
	wSpace = NULL;
	return true;
}

void CollisonTree::RegistWorld(WorldSpace *wSpa)
{
	wSpace = wSpa;
}

CollisonTree* CollisonTree::GetNextObj()
{
	return m_spNext;
}
