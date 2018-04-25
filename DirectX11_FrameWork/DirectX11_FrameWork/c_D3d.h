#pragma once
#include <fstream>
#include <sstream>


const bool ENABLE_4X_MSAA = false;	//��Ƽ ���ø� ����

namespace GameEngine
{
	class c_D3d
	{
	private:
		string m_videoCardDescription;					//���� ī�� �̸�
		SIZE_T m_videoCardMemory;						//���� ī�� �޸𸮷�
		vector<IDXGIAdapter*> m_vAdapters;				//���(����ī��) ����
		bool m_vsync_enabled;							//���� ���� ����
		UINT m_4xMsaaQuality;							//��Ƽ ���ø� ����Ƽ
	
		ID3D11Device* m_pDevice;						//D3D����̽�
		ID3D11DeviceContext* m_pDeviceContext;			//D3D����̽� ����
		IDXGISwapChain* m_pSwapChain;					//���� ü��
	
		ID3D11RenderTargetView* m_pRenderTargetView;	//����Ÿ�� ��
		ID3D11Texture2D* m_pDepthStencilBuffer;			//����-���ٽ� ����
		ID3D11DepthStencilState* m_pDepthStencilState;	//����-���ٽ� ����
		ID3D11DepthStencilView* m_pDepthStencilView;	//����-���ٽ� ��
		ID3D11RasterizerState* m_pRasterState;			//�����Ͷ����� ����
		D3D11_VIEWPORT m_ScreenViewport;				//����Ʈ
	
		XMFLOAT4X4 m_projectionMatrix;					//�������� MAT
		XMFLOAT4X4 m_worldMatrix;						//���� MAT
		XMFLOAT4X4 m_orthoMatrix;						//��Ÿ MAT
	
	public:
		/** D3D11�ʱ�ȭ �Լ� **/
		bool Initialize(const int& screenWidth, const int& screenHeight,
			const bool& vsync, const HWND& hwnd,
			const float& screenDepth, const float& screenNear);
		/** D3D ���� �Լ� **/
		void Shutdown();
		/** �� �׷��ֱ� ���� �Լ� **/
		void BeginScene(const float& red, const float& green,
			const float& blue, const float& alpha);
		/** �� �׷��ֱ� �� �Լ� **/
		void EndScene();
		/** ������ ũ�⿡ ���� ������ ũ�� ��ȯ �Լ� **/
		void OnResize(int screenWidth, int screenHeight);
	
		ID3D11Device* GetDevice() const { return m_pDevice; }
		ID3D11DeviceContext* GetDeviceContext() const { return m_pDeviceContext; }
		const XMFLOAT4X4& GetProjectionMatrix() { return m_worldMatrix; }
		const XMFLOAT4X4& GetWorldMatrix() { return m_worldMatrix; }
		const XMFLOAT4X4& GetOrthoMatrix() { return m_orthoMatrix; }
		void GetVideoCardInfo(char* cardName, SIZE_T& memory);
	
	public:
		c_D3d();
		~c_D3d();
	};
};
