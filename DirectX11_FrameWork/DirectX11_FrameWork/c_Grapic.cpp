#include "stdafx.h"
#include "c_Grapic.h"
#include "GlobalManager.h"

c_Grapic::c_Grapic()
	:m_pD3D(NULL)
{ 
} 

c_Grapic::~c_Grapic() 
{ 
}

bool c_Grapic::Initialize(int screenWidth, int screenHeight)
{ 
	bool result;

	// Create the Direct3D object.
	m_pD3D = new c_D3d;
	if (!m_pD3D)
	{
		MessageBox(0, L"m_pD3D 동적할당 오류", 0, 0);
		return false;
	}

	// Initialize the Direct3D object.
	result = m_pD3D->Initialize(screenWidth, screenHeight,
		VSYNC_ENABLED, GlobalManager::GetContorlWindow()->GetControlWindowHandle(),
		SCREEN_DEPTH, SCREEN_NEAR);
	if (!result)
	{
		MessageBox(0, L"m_pD3D 초기화 오류", 0, 0);
		return false;
	}


	return true;
} 

void c_Grapic::Shutdown() 
{ 
	SAFE_DELETE(m_pD3D);

} 

bool c_Grapic::Frame() 
{ 
	bool result;

	// 그래픽 신을 그려줌
	result = Render();
	if (!result)
	{
		return false;
	}

	return true;
} 

bool c_Grapic::Render() 
{ 
	//XMMATRIX worldMatrix, viewMatrix, projectionMatrix;
	//bool result;


	// Clear the buffers to begin the scene.
	m_pD3D->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

	// Present the rendered scene to the screen.
	m_pD3D->EndScene();

	return true;
}

