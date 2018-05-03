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

//글로벌 오버라이드 new, delte opperator
//stl과 new 오버라이드는 양립할수 없다.
//stl함수 안에 보통의 <new>를 사용하는데 오버라이딩 하면
//오류가 터져나오기 때문이다. (일일이 다 고쳐주거나, stl을 사용하지 않거나 둘중 하나!)

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