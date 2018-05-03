#include "stdafx.h"
#include "MemoryManager.h"

using namespace GameEngine;

//------------------------------------------------------------------------------------
MemoryManager::MemoryManager()
	:m_pFirstPool(nullptr),
	m_pLastPool(nullptr)
{
}

//------------------------------------------------------------------------------------
MemoryManager::~MemoryManager()
{
}

//------------------------------------------------------------------------------------
unsigned int MemoryManager::AddNewPool(const int poolSize, const char* name)
{
	void* alignedPoolPointer = nullptr;
	void* originalPoolPointer = nullptr;
	void* alignedMemoryPointer = nullptr;
	c_MemoryPool* newPool = nullptr;

	//���ĵ� �޸� Ǯ ����
	alignedPoolPointer = AlignedMalloc(poolSize * 1024, 16, OUT &originalPoolPointer);
	if (alignedPoolPointer == nullptr)
	{
		//OutputDebugString(L"����");
		return 0;
	}

	alignedMemoryPointer = reinterpret_cast<void*>(reinterpret_cast<size_t>(alignedPoolPointer) + sizeof(c_MemoryPool));
	//������ �Ÿ� Ǯ�� �Ҵ�
	newPool = new(alignedPoolPointer) c_MemoryPool(name, originalPoolPointer, alignedMemoryPointer);

	if (m_pFirstPool == nullptr)
	{
		m_pFirstPool = newPool;
	}

	return newPool->GetPoolID();
}
//------------------------------------------------------------------------------------
void MemoryManager::DeletePool(const int poolID)
{
	c_MemoryPool* dyingPool;
	void* memoryStart;

	dyingPool = m_pFirstPool;
	while (dyingPool != nullptr)
	{
		if (dyingPool->GetPoolID() == poolID)
		{
			memoryStart = dyingPool->GetOriginalMemoryStart();
			dyingPool->~c_MemoryPool();
			free(memoryStart);
			return;
		}
		dyingPool = dyingPool->GetNextPool();
	}
}
//------------------------------------------------------------------------------------
void MemoryManager::DefragPool()
{
}
//------------------------------------------------------------------------------------
void MemoryManager::QuaryPool()
{
}
//------------------------------------------------------------------------------------
void * MemoryManager::AlignedMalloc(const size_t requredBytes, const size_t alignemnet, OUT void** original)
{
	/*���ĵ� Ŀ���� �����Ҵ�*/
	//�Ҵ�� �Ÿ� ���� (ex. �ü�� 32bit, 16����Ʈ ���Ľ� [alignemnet == 16])
	//alignedBytes(0~15bytes) + c_MemoryPool(class) + requredByetes [��ü �Ҵ�� �޸� ����]
	//alignedBytes(0~15bytes) : 16byte�� ����� �Ǿ� �� ������ offset�� �����Ͽ� ���(�ü�� 32bit, 64bit�� ���� alignedBytes �޶���)
	//c_MemoryPool(class) : �޸�Ǯ Ŭ������ ������ ���� ����

	//����
	//1. (c_MemoryPool ũ�� + ������ ���� byte ũ�� + ��û�� ũ��)�� �Ҵ� ����
	//2. offset�� �̿��Ͽ� ���ĵ� �޸� �ּҸ� ��� �� ����
	//3. ���ĵ� �޸� �ּ� - 1�� �����Ҵ� ������ ���� �޸� �ּ� �����ص�

	void* aligned = nullptr;	//���ĵ� �޸��� ������ [���� �����Ͱ� ��ġ�� ������]
	size_t offset = 0;	//c_MemoryPool ũ�� + ���� ����Ʈ - 1 (������ ���� �ʿ� ����Ʈ)

	offset = alignemnet - 1 + sizeof(c_MemoryPool);
	if ((*original = malloc(offset + requredBytes)) == nullptr)
	{
		//TOOD: malloc ����, �α� ���� �߰�, ����
		return nullptr;
	}

	//ex. original + offset �ּ� & ~(16 - 1)[2������ ��ȯ�ϸ� �ǹ� �� �� ����] [alignemnet == 16]
	//16 �̸��� ��(2����)�� 0���� ��ȯ����( &����) ����ȭ ���� [alignemnet == 16]
	//c_MemoryPool�� ũ�⸦ ���༭ c_MemoryPool�� ������ ������ ����
	//OS. c_MemoryPool�� �����Ϸ� padding�� ���� Word�� ���� �ڵ� ����ȭ�� ��
	aligned = reinterpret_cast<void*>(reinterpret_cast<size_t>(*original) + offset & ~(alignemnet - 1) - sizeof(c_MemoryPool));

	return aligned;
}

//------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------
//�����ڸ� �ҷ����� ���� new ������[���� ������]
void* operator new(size_t size)
{
	OutputDebugString(L"size_t����");
	int i = 0;
	return &i;
}
//------------------------------------------------------------------------------------
void* operator new(size_t size, const std::nothrow_t&)
{
	OutputDebugString(L"sizenothrow_t����");
	int i = 0;
	return &i;
}
//------------------------------------------------------------------------------------
void* operator new[](size_t size)
{int i = 0; OutputDebugString(L"new[]����");
return &i;
}
//------------------------------------------------------------------------------------
void* operator new[](size_t size, const std::nothrow_t&)
{int i = 0; OutputDebugString(L"new[](size_t size, const std::nothrow_t&����");
return &i;
}
//------------------------------------------------------------------------------------
void operator delete(void* p)
{
}
//------------------------------------------------------------------------------------
void operator delete(void* p, std::nothrow_t&)
{
}
//------------------------------------------------------------------------------------
void operator delete(void* p, size_t)
{
}
//------------------------------------------------------------------------------------
void operator delete(void* p, size_t, nothrow_t&)
{
}
//------------------------------------------------------------------------------------
void operator delete[](void* p)
{
}
//------------------------------------------------------------------------------------
void operator delete[](void* p, std::nothrow_t&)
{
}
//------------------------------------------------------------------------------------
void operator delete[](void* p, size_t)
{
}
//------------------------------------------------------------------------------------
void operator delete[](void* p, size_t, nothrow_t&)
{
}
//------------------------------------------------------------------------------------
void* operator new (size_t, int PoolID)
{
	int i = 0; OutputDebugString(L"����");
	return &i;
}