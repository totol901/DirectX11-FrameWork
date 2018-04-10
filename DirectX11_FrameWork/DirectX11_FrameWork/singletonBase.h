#pragma once

template <typename T>
class singletonBase
{
private:
	static T* m_pInstance; //½Ì±ÛÅÏ Àü¿ªº¯¼ö

public:
	/** ½Ì±ÛÅæ Getter **/
	static T* GetSingleton(void)
	{
		if (!m_pInstance)
		{
			m_pInstance = new T;
		}

		return m_pInstance;
	};

	/** ½Ì±ÛÅæ ÆÄ±« **/
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

//½Ì±ÛÅæ Àü¿ªº¯¼ö ÃÊ±âÈ­
template <typename T>
T* singletonBase<T>::m_pInstance = 0;