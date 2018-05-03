#pragma once
#include "c_MemoryPool.h"

namespace GameEngine
{
	class MemoryManager
	{
	public:
		MemoryManager();
		~MemoryManager();

		unsigned int AddNewPool(const int PoolSize, const char* name);
		void DeletePool(const int poolID);
		void DefragPool();
		void QuaryPool();

	private:
		int DefaultPoolID;
		c_MemoryPool* m_pFirstPool;
		c_MemoryPool* m_pLastPool;
		void* AlignedMalloc(const size_t requredBytes, const size_t alignemnet, OUT void** original);

	};
};

//�۷ι� �������̵� new, delte opperator
//stl�� new �������̵�� �縳�Ҽ� ����.
//stl�Լ� �ȿ� ������ <new>�� ����ϴµ� �������̵� �ϸ�
//������ ���������� �����̴�. (������ �� �����ְų�, stl�� ������� �ʰų� ���� �ϳ�!)

//void* operator new(size_t size);
//void* operator new(size_t size, const std::nothrow_t&);
//
//void* operator new[](size_t size);
//void* operator new[](size_t size, const std::nothrow_t&);
//
//void operator delete(void* p);
//void operator delete(void* p, std::nothrow_t&);
//void operator delete(void* p, size_t);
//void operator delete(void* p, size_t, std::nothrow_t&);
//void operator delete[](void* p);
//void operator delete[](void* p, std::nothrow_t&);
//void operator delete[](void* p, size_t);
//void operator delete[](void* p, size_t, std::nothrow_t&);
//
//void* operator new (size_t, int PoolID);