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

	//정렬된 메모리 풀 생성
	poolPointer = AlignedMalloc(blaockSize * 1024, 16);
	if (poolPointer == nullptr)
	{
		return;
	}

	//생성된 매모리 풀로 할당
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
	/*정렬된 커스텀 동적할당*/
	//할당된 매모리 구조 (ex. 운영체제 32bit, 16바이트 정렬시 [alignemnet == 16])
	//address(4bytes) + alignedBytes(0~15bytes) + requredByetes [전체 할당된 메모리 구조]
	//alignedBytes(0~15bytes) : 16byte의 배수가 되야 함 때문에 offset을 지정하여 계산(운영체제 32bit, 64bit에 따라 alignedBytes 달라짐)
	//address(4bytes) : alignemnet의 크기가 address크기 보다 작을 경우, alignedBytes(0~15bytes)에 들어갈 데이터 공간 부족에 대비한 할당

	//로직
	//1. (포인터 크기 + 정렬을 위한 byte 크기 + 요청된 크기)를 할당 받음
	//2. offset을 이용하여 정렬된 메모리 주소를 계산 후 리턴
	//3. 정렬된 메모리 주소 - 1에 동적할당 해제를 위한 메모리 주소 저장해둠

	void* original;	//동적할당 된 메모리의 기본(base) 포인터 [동적할당 해제를 위한 포인터]
	void** aligned;	//정렬된 메모리의 포인터 [실제 데이터가 위치할 포인터]
	size_t offset;	//포인터 크기 + 정렬 바이트 - 1 (정렬을 위한 필요 바이트)

	offset = sizeof(void*) + alignemnet - 1;
	if ((original = malloc(offset + requredBytes)) == nullptr)
	{
		//TOOD: malloc 에러, 로그 파일 추가, 종료
		return nullptr;
	}

	//ex. original + offset 주소 & ~(16 - 1)[2진수로 변환하면 의미 알 수 있음] [alignemnet == 16]
	//16 미만의 수(2진수)를 0으로 변환시켜( &연산) 정렬화 해줌 [alignemnet == 16]
	aligned = reinterpret_cast<void**>(reinterpret_cast<size_t>(original) + offset & ~(alignemnet - 1));
	//동적 할당 해제를 위한 malloc 주소 저장
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