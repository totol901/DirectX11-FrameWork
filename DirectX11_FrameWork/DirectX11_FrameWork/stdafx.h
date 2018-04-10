#pragma once
#include "targetver.h"
/*���� ���ǰų�, ��ġ�� ū ��� �߰�*/

/** �޸� �� ���� **/
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


/** ���� d3d error checker (��ó : 3D ���� ���α׷��� �Թ�, ����ũ D.�糪)**/
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


/** COM ��ü ������ ���� ��ũ��**/
#define RELEASE_COM(x) { if(x){ x->Release(); x = NULL; } }

/** delete ���� ��ũ�ε�**/
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