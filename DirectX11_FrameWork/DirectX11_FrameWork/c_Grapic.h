#pragma once

#include "c_D3d.h"

//�۷ι� ����
static E_WINDOW_STYLE e_windowStyle = E_WINDOW;
const bool VSYNC_ENABLED = true; 
const float SCREEN_DEPTH = 1000.0f; 
const float SCREEN_NEAR = 0.1f; 

class c_Grapic
{
private:
	c_D3d* m_pD3D;
	/** �׷��� ���� �Լ� **/
	bool Render();

public:
	/** �׷��� �ʱ�ȭ �Լ� **/
	bool Initialize(int screenWidth, int screenHeight, HWND hwnd);
	/** �׷��� ���� �Լ� **/
	void Shutdown(); 
	/** �׷��� ������ �Լ� **/
	bool Frame(); 

public:
	c_Grapic();
	~c_Grapic();
};

