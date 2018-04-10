#pragma once
#include "targetver.h"
/*자주 사용되거나, 덩치가 큰 헤더 추가*/

/** 메모리 릭 감지 **/
#if defined(DEBUG) || defined(_DEBUG)
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#endif

#define WIN32_LEAN_AND_MEAN 
#include <Windows.h>
#include <DirectXMath.h>
#include <DirectXPackedVector.h>
#include <cassert>

using namespace DirectX;
using namespace DirectX::PackedVector;
using namespace std;


/** 간단 d3d error checker (출처 : 3D 게임 프로그래밍 입문, 프랭크 D.루나)**/
#if defined(DEBUG) | defined(_DEBUG)
#ifndef HR
#define HR(x)													\
	{                                                           \
		HRESULT hr = (x);                                       \
		if(FAILED(hr))                                          \
		{                                                       \
			DXTrace(__FILEW__, (DWORD)__LINE__, hr, L#x, true); \
		}                                                       \
	}
#endif

#else
	#ifndef HR
	#define HR(x) (x)
	#endif
#endif 


/** COM 객체 릴리즈 간편 매크로**/
#define RELEASE_COM(x) { if(x){ x->Release(); x = NULL; } }

/** delete 간편 매크로들**/
#define SAFE_DELETE(x) { delete x; x = NULL; }
#define SAFE_DELETE_ARRAY(x) { delete[] x; x = NULL;}

namespace Colors
{
	XMGLOBALCONST DirectX::XMVECTORF32 White = { 1.0f, 1.0f, 1.0f, 1.0f };
	XMGLOBALCONST DirectX::XMVECTORF32 Black = { 0.0f, 0.0f, 0.0f, 1.0f };
	XMGLOBALCONST DirectX::XMVECTORF32 Red = { 1.0f, 0.0f, 0.0f, 1.0f };
	XMGLOBALCONST DirectX::XMVECTORF32 Green = { 0.0f, 1.0f, 0.0f, 1.0f };
	XMGLOBALCONST DirectX::XMVECTORF32 Blue = { 0.0f, 0.0f, 1.0f, 1.0f };
	XMGLOBALCONST DirectX::XMVECTORF32 Yellow = { 1.0f, 1.0f, 0.0f, 1.0f };
	XMGLOBALCONST DirectX::XMVECTORF32 Cyan = { 0.0f, 1.0f, 1.0f, 1.0f };
	XMGLOBALCONST DirectX::XMVECTORF32 Magenta = { 1.0f, 0.0f, 1.0f, 1.0f };

	XMGLOBALCONST DirectX::XMVECTORF32 Silver = { 0.75f, 0.75f, 0.75f, 1.0f };
	XMGLOBALCONST DirectX::XMVECTORF32 LightSteelBlue = { 0.69f, 0.77f, 0.87f, 1.0f };
}