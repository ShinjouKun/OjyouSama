#include"OctTreeManager.h"
#include"../Collision/WorldSpace.h"
#include"../Collision/CollisonTree.h"
#include"../Collision/WorldSpace.h"
OctTreeManager::OctTreeManager()
{
	m_uiDim = 0;
	ppWsAry = NULL;
	m_W.x = m_W.y = m_W.z = 1.0f;
	m_RgnMin.x = m_RgnMin.y = m_RgnMin.z = 0.0f;
	m_RgnMax.x = m_RgnMax.y = m_RgnMax.z = 1.0f;
	m_Unit.x = m_Unit.y = m_Unit.z = 1.0f;
	m_dwWorldNum = 0;
	m_uiLevel = 0;
}

OctTreeManager::~OctTreeManager()
{

	DWORD i;
	for (i = 0; i < m_dwWorldNum; i++)
	{
		if (ppWsAry[i] != NULL)
			delete ppWsAry[i];
	}
	delete[] ppWsAry;

}

bool OctTreeManager::Init(unsigned int Level, const Vector3 & Min, const Vector3 & Max)
{
	// �ݒ�ō����x���ȏ�̋�Ԃ͍��Ȃ�
	if (Level >= TREEMANAGER_MAXLEVEL)
		return false;
	// �e���x���ł̋�Ԑ����Z�o
	int i;
	m_iPow[0] = 1;
	for (i = 1; i < TREEMANAGER_MAXLEVEL + 1; i++)
		m_iPow[i] = m_iPow[i - 1] * 8;
	// Level���x���i0��_�j�̔z��쐬
	m_dwWorldNum = (m_iPow[Level + 1] - 1) / 7;
	ppWsAry = new WorldSpace*[m_dwWorldNum];
	ZeroMemory(ppWsAry, sizeof(WorldSpace*)*m_dwWorldNum);

	//�̈�̓o�^
	m_RgnMin = Min;
	m_RgnMax = Max;
	m_W = m_RgnMax - m_RgnMin;
	m_Unit = m_W / ((float)(1 << Level));

	m_uiLevel = Level;

	return true;
}

bool OctTreeManager::Regist(Vector3 * Min, Vector3 * Max, CollisonTree* spOFT)
{
	// �I�u�W�F�N�g�̋��E�͈͂���o�^���[�g���ԍ����Z�o
	DWORD Elem = GetMortonNumber(Min, Max);
	if (Elem < m_dwWorldNum)
	{
		//��Ԃ��Ȃ��ꍇ�͐V�������
		if (!ppWsAry[Elem])
			CreateNewWorld(Elem);
		return ppWsAry[Elem]->Push(spOFT);
	}
	return false;//�o�^���s
}

DWORD OctTreeManager::GetAllCollisonList(vector<BaseCollider*>& ColVect)
{
	//������
	ColVect.clear();
	//���[�g��Ԃ̑��݂��m�F
	if (ppWsAry[0] == NULL)
		return 0;//�����Ȃ�
	//���[�g��Ԃ̏���
	list<BaseCollider*>ColStac;
	GetCollisonList(0, ColVect, ColStac);

	return (DWORD)ColVect.size();
}

bool OctTreeManager::GetCollisonList(DWORD Elem, vector<BaseCollider*>& ColVect, list<BaseCollider*>& ColStac)
{
	typename list<BaseCollider*>::iterator it;
	//��ԓ��̃I�u�W�F�N�g���m�̏Փ˃��X�g�쐬
	CollisonTree* sp0FT1 = ppWsAry[Elem]->GetFirstObj();
	while (sp0FT1 != nullptr)
	{
		CollisonTree* sp0FT2 = sp0FT1->m_spNext;
		while (sp0FT2 != NULL)
		{
			//�Փ˃��X�g�쐬
			ColVect.push_back(sp0FT1->colObject);
			ColVect.push_back(sp0FT2->colObject);
			sp0FT2 = sp0FT2->m_spNext;
		}
		//�Փ˃X�^�b�N�Ƃ̏Փ˃��X�g�쐬
		for (it = ColStac.begin(); it != ColStac.end(); it++)
		{
			ColVect.push_back(sp0FT1->colObject);
			ColVect.push_back(*it);
		}
		sp0FT1 = sp0FT1->m_spNext;
	}
	bool ChildFlag = false;
	//�q��ԂɈړ�
	DWORD objNum = 0;
	DWORD i, NextElem;
	for (i = 0; i < 8; i++)
	{
		NextElem = Elem * 8 + 1 + i;
		if (NextElem < m_dwWorldNum && ppWsAry[Elem * 8 + 1 + i])
		{
			if (!ChildFlag)
			{
				//�o�^�I�u�W�F�N�g���X�^�b�N�ɒǉ�
				sp0FT1 = ppWsAry[Elem]->GetFirstObj();
				while (sp0FT1)
				{
					ColStac.push_back(sp0FT1->colObject);
					objNum++;
					sp0FT1 = sp0FT1->m_spNext;
				}
			}
			ChildFlag = true;
			GetCollisonList(Elem * 8 + 1 + i, ColVect, ColStac);//�q��Ԃ�
		}
	}
	//�X�^�b�N����I�u�W�F�N�g���O��
	if (ChildFlag)
	{
		for (i = 0; i < objNum; i++)
		{
			ColStac.pop_back();
	    }
	}
	return true;
}

bool OctTreeManager::CreateNewWorld(DWORD Elem)
{
	//�����̗v�f�ԍ�
	while (!ppWsAry[Elem])
	{
		ppWsAry[Elem] = new WorldSpace;

		//�e��ԂɃW�����v
		Elem = (Elem - 1) >> 3;
		if (Elem >= m_dwWorldNum)break;
	}
	return true;
}

DWORD OctTreeManager::GetMortonNumber(Vector3 * Min, Vector3 * Max)
{
	// �ŏ����x���ɂ�����e���ʒu���Z�o
	DWORD LT = GetPointElem(*Min);
	DWORD RB = GetPointElem(*Max);
	// ��Ԕԍ��������Z���čŏ�ʋ�؂肩�珊�����x�����Z�o
	DWORD Def = RB ^ LT;
	unsigned int HiLevel = 1;
	unsigned int i;
	for (i = 0; i < m_uiLevel; i++)
	{
		DWORD Check = (Def >> (i * 3)) & 0x7;
		if (Check != 0)
			HiLevel = i + 1;
	}
	DWORD SpaceNum = RB >> (HiLevel * 3);
	DWORD AddNum = (m_iPow[m_uiLevel - HiLevel] - 1) / 7;
	SpaceNum += AddNum;

	if (SpaceNum > m_dwWorldNum)
		return 0xffffffff;

	return SpaceNum;
}

DWORD OctTreeManager::BitSeparateFor3D(BYTE n)
{
	DWORD s = n;
	s = (s | s << 8) & 0x0000f00f;
	s = (s | s << 4) & 0x000c30c3;
	s = (s | s << 2) & 0x00249249;
	return s;
}

DWORD OctTreeManager::Get3DMortonNumber(BYTE x, BYTE y, BYTE z)
{
	return BitSeparateFor3D(x) | BitSeparateFor3D(y) << 1 | BitSeparateFor3D(z) << 2;
}

DWORD OctTreeManager::GetPointElem(const Vector3 & p)
{
	return Get3DMortonNumber(
		(BYTE)((p.x - m_RgnMin.x) / m_Unit.x),
		(BYTE)((p.y - m_RgnMin.y) / m_Unit.y),
		(BYTE)((p.z - m_RgnMin.z) / m_Unit.z)
	);
}
