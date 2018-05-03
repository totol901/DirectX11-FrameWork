#include "stdafx.h"
#include "c_MemoryPool.h"
#include "GlobalManager.h"

using namespace GameEngine;

//------------------------------------------------------------------------------------
c_MemoryPool::c_MemoryPool()
{
}
//------------------------------------------------------------------------------------
c_MemoryPool::c_MemoryPool(const char * name, void* original, void* aligned)
	:OriginalMemoryPointer(original),
	AlignedMemoryPointer(aligned)
{
	NameHash = GlobalManager::GetHash()->Hash(name, sizeof(name));
}
//------------------------------------------------------------------------------------
c_MemoryPool::~c_MemoryPool()
{
}
//------------------------------------------------------------------------------------
unsigned int c_MemoryPool::GetPoolID()
{
	return NameHash;
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
