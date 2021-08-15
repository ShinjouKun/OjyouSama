#pragma once
#include <windows.h>
//SmartPtr.hのインターフェース
//spクラス（テンプレートスマートポインタクラス）
template<class T>
class sp
{
private:
	unsigned int *m_pRefCnt;//参照カウンタへのポインタ
	T** m_ppPtr;//T型オブジェクトのダブルポインタ
	static T* m_NullPtr;//NULLポインタ値
private:
	void AddRef() { (*m_pRefCnt)++; }//カウンター増加
	void Release()//カウンター減少
	{
		if (--(*m_pRefCnt) == 0)
		{
			delete *m_ppPtr;
			delete m_ppPtr;
			delete m_pRefCnt;
		}
	}
public:
	//デフォルト
	explicit sp(T* src = NULL, int add = 0)
	{
		m_pRefCnt = new unsigned int;
		*m_pRefCnt = add;
		m_ppPtr = new T*;
		m_NullPtr = NULL;
		if (src)
			*m_ppPtr = src;
		else
			*m_ppPtr = m_NullPtr;//NULLポインタ挿入
		AddRef();//カウンター増加
	}
	//コピー(同型)
	sp(const sp<T> &src)
	{
		//相手のポインターを全てコピー
		m_pRefCnt = src.m_pRefCnt;
		m_ppPtr = src.m_ppPtr;
		//自分のカウンター増加
		AddRef();
	}
	//コピー（暗黙的アップキャスト？）
	template<class T2> sp(sp<T2> &src)
	{
		m_pRefCnt = src.m_pRefCnt();
		m_ppPtr = (T**)src.GetPtrPtr();

		*m_ppPtr = src.GetPtr();

		AddRef();
	}
	//コピー（NULL代入用）
	sp(const int nullval)
	{
		m_pRefCnt = new UINT;
		*m_pRefCnt = 0;
		m_ppPtr = new T*;
		*m_ppPtr = m_NullPtr;
		AddRef();
	}
	//デストラクタ
	virtual ~sp()
	{
		Release();
	}
	//演算子のオーバーロード
	// =代入演算子（明示的コピー）
	sp<T>& operator = (const sp<T> &src)
	{
		if (*src.m_ppPtr == *m_ppPtr)
			return(*this);//自分自身への代入は意味がないので行わない
		Release();

		//相手のポインタをコピー
		m_pRefCnt = src.m_pRefCnt;
		m_ppPtr = src.m_ppPtr;

		AddRef();

		return (*this);
	}
	// =代入演算子（明示的アップキャスト）
	template<class T2> sp<T>& operator = (sp<T2> &src)
	{
		if (src.GetPtr() == *m_ppPtr)
			return(*this);
		Release();
		//相手のポインタをコピー
		m_pRefCnt = src.GetRefPtr();
		m_ppPtr = (T**)src.GetPtrPtr();
		//型チェックコピー
		*m_ppPtr = src.GetPtr();

		AddRef();

		return(*this);
	}
	// =代入演算子(NULL代入によるリセット）
	sp<T>& operator = (const int nullval)
	{
		Release();

		m_pRefCnt = new unsigned int(1);
		m_ppPtr = new T*;
		m_ppPtr = m_NullPtr;

		return(*this);
	}

	// *間接演算子
	T& operator *() { return **m_ppPtr; }
	// ->メンバ選択演算子
	T* operator ->() { return *m_ppPtr; }
	// ==比較演算子
	bool operator ==(T *val)
	{
		if (*m_ppPtr == val)
			return true;
		return false;
	}
	// !=比較演算子
	bool operator !=(T *val)
	{
		if (*m_ppPtr != val)
			return true;
		return false;
	}
	public:
		//メンバ関数
		// ポインタの明示的な登録
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
		// ポインタの貸し出し
		T* GetPtr() { return *m_ppPtr; }
		T** GetPtrPtr() { return m_ppPtr; }
		// 参照カウンタへのポインタを取得
		unsigned int* GetRefPtr() { return m_pRefCnt; }
		// ダウンキャストコピー
		template <class T2> bool DownCast(sp<T2> &src)
		{
			T* castPtr = dynamic_cast<T*>(src.GetPtr());
			if (castPtr)//ダウンキャスト成功
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
		// ポインタスワップ（交換）
		
			void SwapPtr(sp<T> &src)
			{
				T* pTmp = src.GetPtr();
				*src.m_ppPtr = *m_ppPtr;
				*m_ppPtr = pTmp;
			}
	
};
template<class T>
T* sp<T>::m_NullPtr = NULL;
