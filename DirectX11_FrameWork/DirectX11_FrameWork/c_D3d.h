#pragma once
#include <fstream>
#include <sstream>


const bool ENABLE_4X_MSAA = false;	//멀티 셈플링 유무

namespace GameEngine
{
	class c_D3d
	{
	private:
		string m_videoCardDescription;					//비디오 카드 이름
		SIZE_T m_videoCardMemory;						//비디오 카드 메모리량
		vector<IDXGIAdapter*> m_vAdapters;				//어뎁터(비디오카드) 벡터
		bool m_vsync_enabled;							//수직 동기 유무
		UINT m_4xMsaaQuality;							//멀티 셈플링 퀄리티
	
		ID3D11Device* m_pDevice;						//D3D디바이스
		ID3D11DeviceContext* m_pDeviceContext;			//D3D디바이스 문맥
		IDXGISwapChain* m_pSwapChain;					//스왑 체인
	
		ID3D11RenderTargetView* m_pRenderTargetView;	//렌더타겟 뷰
		ID3D11Texture2D* m_pDepthStencilBuffer;			//깊이-스텐실 버퍼
		ID3D11DepthStencilState* m_pDepthStencilState;	//깊이-스텐실 상태
		ID3D11DepthStencilView* m_pDepthStencilView;	//깊이-스텐실 뷰
		ID3D11RasterizerState* m_pRasterState;			//레스터라이즈 상태
		D3D11_VIEWPORT m_ScreenViewport;				//뷰포트
	
		XMFLOAT4X4 m_projectionMatrix;					//프로젝션 MAT
		XMFLOAT4X4 m_worldMatrix;						//월드 MAT
		XMFLOAT4X4 m_orthoMatrix;						//기타 MAT
	
	public:
		/** D3D11초기화 함수 **/
		bool Initialize(const int& screenWidth, const int& screenHeight,
			const bool& vsync, const HWND& hwnd,
			const float& screenDepth, const float& screenNear);
		/** D3D 해제 함수 **/
		void Shutdown();
		/** 씬 그려주기 시작 함수 **/
		void BeginScene(const float& red, const float& green,
			const float& blue, const float& alpha);
		/** 씬 그려주기 끝 함수 **/
		void EndScene();
		/** 윈도우 크기에 따른 벡버퍼 크기 변환 함수 **/
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
