#pragma once

#include "c_D3d.h"

//글로벌 변수
static E_WINDOW_STYLE e_windowStyle = E_WINDOW;
const bool VSYNC_ENABLED = true; 
const float SCREEN_DEPTH = 1000.0f; 
const float SCREEN_NEAR = 0.1f; 

class c_Grapic
{
private:
	c_D3d* m_pD3D;
	/** 그래픽 랜더 함수 **/
	bool Render();

public:
	/** 그래픽 초기화 함수 **/
	bool Initialize(int screenWidth, int screenHeight, HWND hwnd);
	/** 그래픽 해제 함수 **/
	void Shutdown(); 
	/** 그래픽 프래임 함수 **/
	bool Frame(); 

public:
	c_Grapic();
	~c_Grapic();
};

