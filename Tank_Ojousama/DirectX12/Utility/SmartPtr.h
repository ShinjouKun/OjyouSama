#pragma once
#include <windows.h>
//SmartPtr.h�̃C���^�[�t�F�[�X
//sp�N���X�i�e���v���[�g�X�}�[�g�|�C���^�N���X�j
template<class T>
class sp
{
private:
	unsigned int *m_pRefCnt;//�Q�ƃJ�E���^�ւ̃|�C���^
	T** m_ppPtr;//T�^�I�u�W�F�N�g�̃_�u���|�C���^
	static T* m_NullPtr;//NULL�|�C���^�l
private:
	void AddRef() { (*m_pRefCnt)++; }//�J�E���^�[����
	void Release()//�J�E���^�[����
	{
		if (--(*m_pRefCnt) == 0)
		{
			delete *m_ppPtr;
			delete m_ppPtr;
			delete m_pRefCnt;
		}
	}
public:
	//�f�t�H���g
	explicit sp(T* src = NULL, int add = 0)
	{
		m_pRefCnt = new unsigned int;
		*m_pRefCnt = add;
		m_ppPtr = new T*;
		m_NullPtr = NULL;
		if (src)
			*m_ppPtr = src;
		else
			*m_ppPtr = m_NullPtr;//NULL�|�C���^�}��
		AddRef();//�J�E���^�[����
	}
	//�R�s�[(���^)
	sp(const sp<T> &src)
	{
		//����̃|�C���^�[��S�ăR�s�[
		m_pRefCnt = src.m_pRefCnt;
		m_ppPtr = src.m_ppPtr;
		//�����̃J�E���^�[����
		AddRef();
	}
	//�R�s�[�i�ÖٓI�A�b�v�L���X�g�H�j
	template<class T2> sp(sp<T2> &src)
	{
		m_pRefCnt = src.m_pRefCnt();
		m_ppPtr = (T**)src.GetPtrPtr();

		*m_ppPtr = src.GetPtr();

		AddRef();
	}
	//�R�s�[�iNULL����p�j
	sp(const int nullval)
	{
		m_pRefCnt = new UINT;
		*m_pRefCnt = 0;
		m_ppPtr = new T*;
		*m_ppPtr = m_NullPtr;
		AddRef();
	}
	//�f�X�g���N�^
	virtual ~sp()
	{
		Release();
	}
	//���Z�q�̃I�[�o�[���[�h
	// =������Z�q�i�����I�R�s�[�j
	sp<T>& operator = (const sp<T> &src)
	{
		if (*src.m_ppPtr == *m_ppPtr)
			return(*this);//�������g�ւ̑���͈Ӗ����Ȃ��̂ōs��Ȃ�
		Release();

		//����̃|�C���^���R�s�[
		m_pRefCnt = src.m_pRefCnt;
		m_ppPtr = src.m_ppPtr;

		AddRef();

		return (*this);
	}
	// =������Z�q�i�����I�A�b�v�L���X�g�j
	template<class T2> sp<T>& operator = (sp<T2> &src)
	{
		if (src.GetPtr() == *m_ppPtr)
			return(*this);
		Release();
		//����̃|�C���^���R�s�[
		m_pRefCnt = src.GetRefPtr();
		m_ppPtr = (T**)src.GetPtrPtr();
		//�^�`�F�b�N�R�s�[
		*m_ppPtr = src.GetPtr();

		AddRef();

		return(*this);
	}
	// =������Z�q(NULL����ɂ�郊�Z�b�g�j
	sp<T>& operator = (const int nullval)
	{
		Release();

		m_pRefCnt = new unsigned int(1);
		m_ppPtr = new T*;
		m_ppPtr = m_NullPtr;

		return(*this);
	}

	// *�Ԑډ��Z�q
	T& operator *() { return **m_ppPtr; }
	// ->�����o�I�����Z�q
	T* operator ->() { return *m_ppPtr; }
	// ==��r���Z�q
	bool operator ==(T *val)
	{
		if (*m_ppPtr == val)
			return true;
		return false;
	}
	// !=��r���Z�q
	bool operator !=(T *val)
	{
		if (*m_ppPtr != val)
			return true;
		return false;
	}
	public:
		//�����o�֐�
		// �|�C���^�̖����I�ȓo�^
		void SetPtr(T* src = NULL, int add = 0)
		{
			Release();
			m_pRefCnt = new unsigned int;
			*m_pRefCnt = add;
			m_ppPtr = new T*;
			if (src)
				*m_ppPtr = src;
			else
				*m_ppPtr = m_NullPtr;
			AddRef();
		}
		// �|�C���^�݂̑��o��
		T* GetPtr() { return *m_ppPtr; }
		T** GetPtrPtr() { return m_ppPtr; }
		// �Q�ƃJ�E���^�ւ̃|�C���^���擾
		unsigned int* GetRefPtr() { return m_pRefCnt; }
		// �_�E���L���X�g�R�s�[
		template <class T2> bool DownCast(sp<T2> &src)
		{
			T* castPtr = dynamic_cast<T*>(src.GetPtr());
			if (castPtr)//�_�E���L���X�g����
			{
				Release();
				m_ppPtr = (T**)src.GetPtrPtr();
				*m_ppPtr = castPtr;
				m_pRefCnt = src.GetRefPtr();
				AddRef();
				return true;
			}
			return false;
		}
		// �|�C���^�X���b�v�i�����j
		
			void SwapPtr(sp<T> &src)
			{
				T* pTmp = src.GetPtr();
				*src.m_ppPtr = *m_ppPtr;
				*m_ppPtr = pTmp;
			}
	
};
template<class T>
T* sp<T>::m_NullPtr = NULL;
