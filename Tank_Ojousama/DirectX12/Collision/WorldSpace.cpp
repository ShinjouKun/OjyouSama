#include "WorldSpace.h"
#include "CollisonTree.h"
WorldSpace::WorldSpace()
{
	m_spLatest = nullptr;
}

void WorldSpace::ResetLink(CollisonTree* sp0FT)
{
	if (sp0FT->m_spNext != nullptr)
		ResetLink(sp0FT->m_spNext);
	sp0FT = nullptr;//�X�}�[�g�|�C���^���Z�b�g
}

bool WorldSpace::Push(CollisonTree* sp0FT)
{
	if (sp0FT == nullptr)return false;//�����ȃI�u�W�F�N�g�͓o�^���Ȃ�
	if (sp0FT->wSpace == this)return false;//��d�o�^���`�F�b�N
	if (m_spLatest == nullptr)
	{
		m_spLatest = sp0FT;//��ԂɐV�K�o�^
	}
	else
	{
		//�ŐV0FT�I�u�W�F�N�g���X�V
		sp0FT->m_spNext = m_spLatest;
		m_spLatest->m_spPre = sp0FT;
		m_spLatest = sp0FT;
	}
	sp0FT->RegistWorld(this);//��Ԃ�o�^
	return true;
}

CollisonTree* WorldSpace::GetFirstObj()
{
	return m_spLatest;
}

bool WorldSpace::OnRemove(CollisonTree *pRemoveObj)
{
	if (m_spLatest == pRemoveObj)
	{
		//���̃I�u�W�F�N�g�ɑ}���ւ�
		if (m_spLatest != nullptr)
			m_spLatest = m_spLatest->GetNextObj();
	}
	return true;
}
