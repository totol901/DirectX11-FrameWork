#pragma once
#include "c_MemoryPool.h"

class MemoryManager
{
public:
	MemoryManager();
	~MemoryManager();

	void AddNewPool(const int blaockSize, const char* name);
	void DeletePool(const int poolID);
	void DefragPool();
	void QuaryPool();
	
private:
	int DefaultPoolID;
	c_MemoryPool* m_pFirstPool;
	c_MemoryPool* m_pLastPool;
	void* AlignedMalloc(const size_t requredBytes, const size_t alignemnet);

};

//글로벌 오버라이드 new, delte opperator
void* operator new(size_t size);
void* operator new(size_t size, const std::nothrow_t&);

void* operator new[](size_t size);
void* operator new[](size_t size, const std::nothrow_t&);

void operator delete(void* p);
void operator delete(void* p, std::nothrow_t&);
void operator delete(void* p, size_t);
void operator delete(void* p, size_t, nothrow_t&);
void operator delete[](void* p);
void operator delete[](void* p, std::nothrow_t&);
void operator delete[](void* p, size_t);
void operator delete[](void* p, size_t, nothrow_t&);

void* operator new (size_t, int PoolID);