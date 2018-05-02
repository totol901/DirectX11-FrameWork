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
void MemoryManager::AddNewPool(const int poolSize, const char* name)
{
	void* alignedPoolPointer = nullptr;
	void* originalPoolPointer = nullptr;
	void* alignedMemoryPointer = nullptr;
	c_MemoryPool* newPool = nullptr;

	//정렬된 메모리 풀 생성
	alignedPoolPointer = AlignedMalloc(poolSize * 1024, 16, OUT originalPoolPointer);
	if (alignedPoolPointer == nullptr)
	{
		//OutputDebugString(L"하학");
		return;
	}

	alignedMemoryPointer = reinterpret_cast<void*>(reinterpret_cast<size_t>(alignedPoolPointer) + sizeof(c_MemoryPool));
	//생성된 매모리 풀로 할당
	newPool = new(alignedPoolPointer) c_MemoryPool(name, originalPoolPointer, alignedMemoryPointer);
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
void * MemoryManager::AlignedMalloc(const size_t requredBytes, const size_t alignemnet, OUT void* original)
{
	/*정렬된 커스텀 동적할당*/
	//할당된 매모리 구조 (ex. 운영체제 32bit, 16바이트 정렬시 [alignemnet == 16])
	//alignedBytes(0~15bytes) + c_MemoryPool(class) + requredByetes [전체 할당된 메모리 구조]
	//alignedBytes(0~15bytes) : 16byte의 배수가 되야 함 때문에 offset을 지정하여 계산(운영체제 32bit, 64bit에 따라 alignedBytes 달라짐)
	//c_MemoryPool(class) : 메모리풀 클래스의 변수들 저장 공간

	//로직
	//1. (c_MemoryPool 크기 + 정렬을 위한 byte 크기 + 요청된 크기)를 할당 받음
	//2. offset을 이용하여 정렬된 메모리 주소를 계산 후 리턴
	//3. 정렬된 메모리 주소 - 1에 동적할당 해제를 위한 메모리 주소 저장해둠

	void* aligned = nullptr;	//정렬된 메모리의 포인터 [실제 데이터가 위치할 포인터]
	size_t offset = 0;	//c_MemoryPool 크기 + 정렬 바이트 - 1 (정렬을 위한 필요 바이트)

	offset = alignemnet - 1 + sizeof(c_MemoryPool);
	if ((original = malloc(offset + requredBytes)) == nullptr)
	{
		//TOOD: malloc 에러, 로그 파일 추가, 종료
		return nullptr;
	}

	//ex. original + offset 주소 & ~(16 - 1)[2진수로 변환하면 의미 알 수 있음] [alignemnet == 16]
	//16 미만의 수(2진수)를 0으로 변환시켜( &연산) 정렬화 해줌 [alignemnet == 16]
	//c_MemoryPool의 크기를 빼줘서 c_MemoryPool의 변수들 공간을 만듬
	//OS. c_MemoryPool은 컴파일러 padding에 의해 Word에 맞춰 자동 정렬화가 됨
	aligned = reinterpret_cast<void*>(reinterpret_cast<size_t>(original) + offset & ~(alignemnet - 1) - sizeof(c_MemoryPool));

	return aligned;
}

//------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------
//생성자를 불러오지 않은 new 연산자[가장 간단함]
void* operator new(size_t size)
{
	OutputDebugString(L"size_t하학");
	int i = 0;
	return &i;
}
//------------------------------------------------------------------------------------
void* operator new(size_t size, const std::nothrow_t&)
{
	OutputDebugString(L"sizenothrow_t하학");
	int i = 0;
	return &i;
}
//------------------------------------------------------------------------------------
void* operator new[](size_t size)
{int i = 0; OutputDebugString(L"new[]하학");
return &i;
}
//------------------------------------------------------------------------------------
void* operator new[](size_t size, const std::nothrow_t&)
{int i = 0; OutputDebugString(L"new[](size_t size, const std::nothrow_t&하학");
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
	int i = 0; OutputDebugString(L"하학");
	return &i;
}