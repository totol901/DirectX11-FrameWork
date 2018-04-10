#pragma once

template <typename T>
class singletonBase
{
private:
	static T* m_pInstance; //�̱��� ��������

public:
	/** �̱��� Getter **/
	static T* GetSingleton(void)
	{
		if (!m_pInstance)
		{
			m_pInstance = new T;
		}

		return m_pInstance;
	};

	/** �̱��� �ı� **/
	void DestroySingleton(void)
	{
		if (m_pInstance)
		{
			delete m_pInstance;
			m_pInstance = 0;
		}
	};

protected:
	singletonBase() {};
	virtual ~singletonBase() {};
};

//�̱��� �������� �ʱ�ȭ
template <typename T>
T* singletonBase<T>::m_pInstance = 0;