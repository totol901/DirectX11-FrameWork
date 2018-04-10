#pragma once

#include <fstream>
#include <sstream>


const bool ENABLE_4X_MSAA = false;

class c_D3d
{
private:
	string m_videoCardDescription;
	SIZE_T m_videoCardMemory;
	vector<IDXGIAdapter*> m_vAdapters;
	bool m_vsync_enabled;
	UINT m_4xMsaaQuality;
	
	ID3D11Device* m_pDevice;
	ID3D11DeviceContext* m_pDeviceContext;
	IDXGISwapChain* m_pSwapChain;
	
	ID3D11RenderTargetView* m_pRenderTargetView;
	ID3D11Texture2D* m_pDepthStencilBuffer;
	ID3D11DepthStencilState* m_pDepthStencilState;
	ID3D11DepthStencilView* m_pDepthStencilView;
	ID3D11RasterizerState* m_pRasterState;
	D3D11_VIEWPORT m_ScreenViewport;

	XMFLOAT4X4 m_projectionMatrix;
	XMFLOAT4X4 m_worldMatrix;
	XMFLOAT4X4 m_orthoMatrix;

public:
	bool Initialize(int screenWidth, int screenHeight,
		bool vsync, HWND hwnd, bool fullscreen,
		float screenDepth, float screenNear);
	void Shutdown();
	void BeginScene(float, float, float, float);
	void EndScene();
	ID3D11Device* GetDevice();
	ID3D11DeviceContext* GetDeviceContext();
	void GetProjectionMatrix(FXMMATRIX&);
	void GetWorldMatrix(FXMMATRIX&);
	void GetOrthoMatrix(FXMMATRIX&);
	void GetVideoCardInfo(char*, int&);
	void OnResize(int screenWidth, int screenHeight);

public:
	c_D3d();
	~c_D3d();
};

