#include "stdafx.h"
#include "c_MemoryPool.h"

//------------------------------------------------------------------------------------
c_MemoryPool::c_MemoryPool()
{
}
//------------------------------------------------------------------------------------
c_MemoryPool::c_MemoryPool(const char * name, void* original, void* aligned)
	:OriginalMemoryPointer(original),
	AlignedMemoryPointer(aligned)
{
}
//------------------------------------------------------------------------------------
c_MemoryPool::~c_MemoryPool()
{
}
//------------------------------------------------------------------------------------
unsigned int c_MemoryPool::GetPoolID()
{
	return 0;
}
//------------------------------------------------------------------------------------
void * c_MemoryPool::GetOriginalMemoryStart()
{
	return OriginalMemoryPointer;
}
//------------------------------------------------------------------------------------
c_MemoryPool * c_MemoryPool::GetNextPool()
{
	return nullptr;
}
