#include "stdafx.h"
#include "MemoryManager.h"

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
void MemoryManager::AddNewPool(const int blaockSize, const char* name)
{
	void* poolPointer;
	c_MemoryPool* newBlcok;

	//���ĵ� �޸� Ǯ ����
	poolPointer = AlignedMalloc(blaockSize * 1024, 16);
	if (poolPointer == nullptr)
	{
		return;
	}

	//������ �Ÿ� Ǯ�� �Ҵ�
	newBlcok = new(poolPointer) c_MemoryPool(name);
}
//------------------------------------------------------------------------------------
void MemoryManager::DeletePool(const int poolID)
{
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
void * MemoryManager::AlignedMalloc(const size_t requredBytes, const size_t alignemnet)
{
	/*���ĵ� Ŀ���� �����Ҵ�*/
	//�Ҵ�� �Ÿ� ���� (ex. �ü�� 32bit, 16����Ʈ ���Ľ� [alignemnet == 16])
	//address(4bytes) + alignedBytes(0~15bytes) + requredByetes [��ü �Ҵ�� �޸� ����]
	//alignedBytes(0~15bytes) : 16byte�� ����� �Ǿ� �� ������ offset�� �����Ͽ� ���(�ü�� 32bit, 64bit�� ���� alignedBytes �޶���)
	//address(4bytes) : alignemnet�� ũ�Ⱑ addressũ�� ���� ���� ���, alignedBytes(0~15bytes)�� �� ������ ���� ������ ����� �Ҵ�

	//����
	//1. (������ ũ�� + ������ ���� byte ũ�� + ��û�� ũ��)�� �Ҵ� ����
	//2. offset�� �̿��Ͽ� ���ĵ� �޸� �ּҸ� ��� �� ����
	//3. ���ĵ� �޸� �ּ� - 1�� �����Ҵ� ������ ���� �޸� �ּ� �����ص�

	void* original;	//�����Ҵ� �� �޸��� �⺻(base) ������ [�����Ҵ� ������ ���� ������]
	void** aligned;	//���ĵ� �޸��� ������ [���� �����Ͱ� ��ġ�� ������]
	size_t offset;	//������ ũ�� + ���� ����Ʈ - 1 (������ ���� �ʿ� ����Ʈ)

	offset = sizeof(void*) + alignemnet - 1;
	if ((original = malloc(offset + requredBytes)) == nullptr)
	{
		//TOOD: malloc ����, �α� ���� �߰�, ����
		return nullptr;
	}

	//ex. original + offset �ּ� & ~(16 - 1)[2������ ��ȯ�ϸ� �ǹ� �� �� ����] [alignemnet == 16]
	//16 �̸��� ��(2����)�� 0���� ��ȯ����( &����) ����ȭ ���� [alignemnet == 16]
	aligned = reinterpret_cast<void**>(reinterpret_cast<size_t>(original) + offset & ~(alignemnet - 1));
	//���� �Ҵ� ������ ���� malloc �ּ� ����
	aligned[-1] = original;

	return aligned;
}

//------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------
void* operator new(size_t size)
{
	int i = 0;
	return &i;
}
//------------------------------------------------------------------------------------
void* operator new(size_t size, const std::nothrow_t&)
{
	int i = 0;
	return &i;
}
//------------------------------------------------------------------------------------
void* operator new[](size_t size)
{int i = 0;
return &i;
}
//------------------------------------------------------------------------------------
void* operator new[](size_t size, const std::nothrow_t&)
{int i = 0;
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
	int i = 0;
	return &i;
}