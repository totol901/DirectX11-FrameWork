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


	//FeatureLevel ����(�ְ� ������ Level �ޱ�����)
	//D3D_FEATURE_LEVEL featureLevel;
	//HRESULT hr = D3D11CreateDevice(
	//	0,                 // default adapter(�⺻ ���÷��� ����� ����)
	//	md3dDriverType,	   // 3���� �׷��� ���� ����
	//	0,                 // no software device
	//	createDeviceFlags, // ����̽� �÷��� ����
	//	0, 0,              // default feature level array
	//	D3D11_SDK_VERSION, // �׻� D3D11_SDK_VERSION ����
	//	&md3dDevice,	   // d3d��ġ �־��� (Out ��)
	//	&featureLevel,	   // featureLevel �־���[�� ��밡�� �ְ� ������ Level ����] (OUt ��)
	//	&md3dImmediateContext); // ��ġ ����(Context) �־��� (Out ��)


	

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

