#include "stdafx.h"
#include "c_Grapic.h"

c_Grapic::c_Grapic()
{ 
} 

c_Grapic::~c_Grapic() 
{ 
}

bool c_Grapic::Initialize(int screenWidth, int screenHeight, HWND hwnd)
{ 


	//FeatureLevel 선언(최고 수준의 Level 받기위해)
	//D3D_FEATURE_LEVEL featureLevel;
	//HRESULT hr = D3D11CreateDevice(
	//	0,                 // default adapter(기본 디스플레이 어댑터 설정)
	//	md3dDriverType,	   // 3차원 그래픽 가속 적용
	//	0,                 // no software device
	//	createDeviceFlags, // 디바이스 플레그 설정
	//	0, 0,              // default feature level array
	//	D3D11_SDK_VERSION, // 항상 D3D11_SDK_VERSION 지정
	//	&md3dDevice,	   // d3d장치 넣어줌 (Out 값)
	//	&featureLevel,	   // featureLevel 넣어줌[현 사용가능 최고 수준의 Level 나옴] (OUt 값)
	//	&md3dImmediateContext); // 장치 문맥(Context) 넣어줌 (Out 값)


	

	return true; 
} 

void c_Grapic::Shutdown() 
{ 
	return; 
} 

bool c_Grapic::Frame() 
{ 
	return true; 
} 

bool c_Grapic::Render() 
{ 
	return true; 
}

