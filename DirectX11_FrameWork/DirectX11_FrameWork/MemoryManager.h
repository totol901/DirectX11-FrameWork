#pragma once

class MemoryManager
{
public:
	MemoryManager();
	~MemoryManager();
};

//글로벌 오버라이드 new, delte opperator
void* operator new(size_t size);
void* operator new(size_t size, const std::nothrow_t&);

void* operator new[](size_t size);
void* operator new[](size_t size, const std::nothrow_t&);

void operator delete(void* p);
void operator delete(void* p, std::nothrow_t&);

void operator delete[](void* p);
void operator delete[](void* p, std::nothrow_t&);


