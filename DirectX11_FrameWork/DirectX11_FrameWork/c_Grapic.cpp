#include "stdafx.h"
#include "c_Grapic.h"

c_Grapic::c_Grapic()
	:m_pD3D(NULL)
{ 
} 

c_Grapic::~c_Grapic() 
{ 
}

bool c_Grapic::Initialize(int screenWidth, int screenHeight, HWND hwnd)
{ 
	bool result;

	// Create the Direct3D object.
	m_pD3D = new c_D3d;
	if (!m_pD3D)
	{
		MessageBox(0, L"m_pD3D �����Ҵ� ����", 0, 0);
		return false;
	}

	// Initialize the Direct3D object.
	result = m_pD3D->Initialize(screenWidth, screenHeight,
		VSYNC_ENABLED, hwnd, e_windowStyle, SCREEN_DEPTH, SCREEN_NEAR);
	if (!result)
	{
		MessageBox(0, L"m_pD3D �ʱ�ȭ ����", 0, 0);
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

	// �׷��� ���� �׷���
	result = Render();
	if (!result)
	{
		return false;
	}

	return true;
} 

bool c_Grapic::Render() 
{ 
	XMMATRIX worldMatrix, viewMatrix, projectionMatrix;
	bool result;


	// Clear the buffers to begin the scene.
	m_pD3D->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

	// Generate the view matrix based on the camera's position.
	m_Camera->Render();

	// Get the world, view, and projection matrices from the camera and d3d objects.
	m_Camera->GetViewMatrix(viewMatrix);
	m_pD3D->GetWorldMatrix(worldMatrix);
	m_pD3D->GetProjectionMatrix(projectionMatrix);

	// Put the model vertex and index buffers on the graphics pipeline to prepare them for drawing.
	m_Model->Render(m_D3D->GetDeviceContext());

	// Render the model using the texture shader.
	result = m_TextureShader->Render(m_D3D->GetDeviceContext(), m_Model->GetIndexCount(),
		worldMatrix, viewMatrix, projectionMatrix, m_Model->GetTexture());
	if (!result)
	{
		return false;
	}

	// Present the rendered scene to the screen.
	m_D3D->EndScene();

	return true;
}

