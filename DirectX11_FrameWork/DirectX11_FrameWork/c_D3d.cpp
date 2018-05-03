#include "stdafx.h"
#include "c_D3d.h"
#include "GlobalManager.h"

using namespace GameEngine;

c_D3d::c_D3d()
	:m_videoCardMemory(0),
	m_vsync_enabled(false),
	m_4xMsaaQuality(0),
	m_pDevice(NULL),
	m_pDeviceContext(NULL),
	m_pSwapChain(NULL),
	m_pRenderTargetView(NULL),
	m_pDepthStencilBuffer(NULL),
	m_pDepthStencilState(NULL),
	m_pDepthStencilView(NULL),
	m_pRasterState(NULL)
{
}

c_D3d::~c_D3d()
{
}

bool c_D3d::Initialize(const int& screenWidth, const int& screenHeight,
	const bool& vsync, const HWND& hwnd,
	const float& screenDepth, const float& screenNear)
{
	// vsync(수직동기화) 설정 저장
	m_vsync_enabled = vsync;

	// DirectX 그래픽 인터페이스 팩토리를 만듬
	IDXGIFactory* pFactory;
	HR(CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&pFactory));

	//팩토리 객체를 이용하여, 어텝터(그래픽 카드)를 벡터에 담아줌
	IDXGIAdapter* pTempAdapter;
	UINT mAdpter = 0;

	while (pFactory->EnumAdapters(mAdpter, &pTempAdapter) != DXGI_ERROR_NOT_FOUND)
	{
		m_vAdapters.push_back(pTempAdapter);
		++mAdpter;
	}
	//어댑터 갯수 넣어줌
	mAdpter = m_vAdapters.size();

	//파일 출력 
	//시작
	ofstream fon("VideoInfo.txt", ios::out);

	wostringstream outs;
	
	UINT MaxGrapicCardMemIndex = 0;
	SIZE_T MaxMemSize = 0;
	for (UINT i = 0; i < mAdpter; i++)
	{
		//어뎁터 정보를 어뎁터 정보 구조체에 넣어줌
		DXGI_ADAPTER_DESC desc;
		m_vAdapters[i]->GetDesc(&desc);
		
		//wchar -> char로 형변환
		char* pStr = Convert_WCHAR_To_CHAR(desc.Description);

		//stringstream에 넣어줌
		outs << "그래픽 카드 이름 : " << pStr << "\n"
			<< "비디오 메모리 : " << desc.DedicatedVideoMemory / 1024 / 1024 << "\n"
			<< "시스템 메모리 : " << desc.DedicatedSystemMemory / 1024 / 1024 << "\n"
			<< "Shared 시스템 메모리 : " << desc.SharedSystemMemory / 1024 / 1024 << "\n"
			<< "\n";

		//가장 큰 메모리 크기의 디바이스를 찾는다.
		if (MaxMemSize < desc.DedicatedVideoMemory)
		{
			MaxGrapicCardMemIndex = i;
			MaxMemSize = desc.DedicatedVideoMemory;
			//그 디바이스의 이름과 메모리양 저장
			m_videoCardDescription = pStr;
			m_videoCardMemory = desc.DedicatedVideoMemory / 1024 / 1024;
		}

		//동적할당 해제
		SAFE_DELETE_ARRAY(pStr);
	}

	// 출력(모니터)에 그래픽 메모리가 가장 큰 아답터를 설정해줌
	UINT mDefaultOutputCounts = 0;
	IDXGIOutput* pOutput;
	HR(m_vAdapters[MaxGrapicCardMemIndex]->EnumOutputs(mDefaultOutputCounts,
		&pOutput));
	
	//위의 어댑터의 아웃풋에 지원되는 디스플레이 모드 출력해줌
	UINT numModes = 0;
	//아웃풋의 디스플레이 모드 갯수 받아옴
	HR(pOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM,
		DXGI_ENUM_MODES_INTERLACED,
		&numModes,
		0));

	//갯수 이용하여 [디스플레이 모드 DESC] 동적 배열 생성
	DXGI_MODE_DESC* displayModeList = new DXGI_MODE_DESC[numModes];
	//디스플레이 모드 DESC에 넣어줌
	HR(pOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM,
		DXGI_ENUM_MODES_INTERLACED,
		&numModes,
		displayModeList));

	UINT numerator = 0;
	UINT denominator = 0;
	//DESC 출력해줌
	for (UINT i = 0; i < numModes; i++)
	{
		outs << L"***WIDTH = " << displayModeList[i].Width
			<< L" HEIGHT = " << displayModeList[i].Height
			<< L" REFRESH = " << displayModeList[i].RefreshRate.Numerator
			<< L"/" << displayModeList[i].RefreshRate.Denominator << L"\n";
	
		if (displayModeList[i].Width == (UINT)screenWidth)
		{
			if (displayModeList[i].Height == (UINT)screenHeight)
			{
				numerator = displayModeList[i].RefreshRate.Numerator;
				denominator = displayModeList[i].RefreshRate.Denominator;
			}
		}
	}

	//메모리 해제
	SAFE_DELETE_ARRAY(displayModeList);
	
	//파일에 string 저장
	fon << outs.str().c_str();
	//종료
	fon.close();

	//출력 어뎁터 할당 해제
	RELEASE_COM(pOutput);

	//어뎁터 할당 해제
	for (UINT i = 0; i < m_vAdapters.size(); i++)
	{
		RELEASE_COM(m_vAdapters[i]);
	}


	UINT createDeviceFlags = 0; // 디바이스 플레그 설정(Default)
#if defined(DEBUG) || defined(_DEBUG)  
								//에러시 디버그 출력해줌
	createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	//FeatureLevel 선언(최고 수준의 Level 받기위해)
	D3D_FEATURE_LEVEL featureLevel;
	HRESULT hr = D3D11CreateDevice(
		0,                 // default adapter(기본 디스플레이 어댑터 설정)
		D3D_DRIVER_TYPE_HARDWARE,	   // 3차원 그래픽 가속 적용
		0,                 // no software device
		createDeviceFlags, // 디바이스 플레그 설정
		0, 0,              // default feature level array
		D3D11_SDK_VERSION, // 항상 D3D11_SDK_VERSION 지정
		&m_pDevice,	   // d3d장치 넣어줌 (Out 값)
		&featureLevel,	   // featureLevel 넣어줌[현 사용가능 최고 수준의 Level 나옴] (OUt 값)
		&m_pDeviceContext); // 장치 문맥(Context) 넣어줌 (Out 값)

	//md3dDevice 설정 완료됫나 검사
	if (FAILED(hr))
	{
		MessageBox(0, L"D3D11CreateDevice 생성 실패.", 0, 0);
		return false;
	}

	//FeatureLevel의 수준 검사
	if (featureLevel < D3D_FEATURE_LEVEL_11_0)
	{
		MessageBox(0, L"Direct3D Feature Level 11 지원 안함.", 0, 0);
		return false;
	}

	//알맞은 멀티셈플링 퀄리티 세팅
	HR(m_pDevice->CheckMultisampleQualityLevels(
		DXGI_FORMAT_R8G8B8A8_UNORM, 4, &m_4xMsaaQuality));
	//올바른 세팅인지 채크
	assert(m_4xMsaaQuality > 0);

	//교환 사슬 설정
	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	//교환 사슬 description을 초기화합니다.
	ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));

	//하나의 백버퍼만을 사용하도록 합니다.
	swapChainDesc.BufferCount = 1;

	//백버퍼의 너비와 높이를 설정합니다.
	swapChainDesc.BufferDesc.Width = screenWidth;
	swapChainDesc.BufferDesc.Height = screenHeight;

	//백버퍼로 일반적인 32bit의 서페이스를 지정합니다.
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;//후면 버퍼 픽셀 형식

	// 백버퍼의 새로고침 비율을 설정합니다. 
	if(m_vsync_enabled) 
	{
		swapChainDesc.BufferDesc.RefreshRate.Numerator = numerator; 
		swapChainDesc.BufferDesc.RefreshRate.Denominator = denominator; 
	} 
	else 
	{ 
		swapChainDesc.BufferDesc.RefreshRate.Numerator = 0; 
		swapChainDesc.BufferDesc.RefreshRate.Denominator = 1; 
	} 
	// 백버퍼의 용도를 설정합니다. 
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT; 
	// 렌더링이 이루어질 윈도우의 핸들을 설정합니다. 
	swapChainDesc.OutputWindow = hwnd; 
	// 멀티셈플링 유무 확인
	if (ENABLE_4X_MSAA)
	{
		swapChainDesc.SampleDesc.Count = 4;
		swapChainDesc.SampleDesc.Quality = m_4xMsaaQuality - 1;
	}
	// 멀티셈플링 안씀
	else
	{
		swapChainDesc.SampleDesc.Count = 1;
		swapChainDesc.SampleDesc.Quality = 0;
	}
	// 윈도우 모드 또는 풀스크린 모드를 설정합니다. 
	if(GlobalManager::GetContorlWindow()->GetWindowModeStyle()
		== E_WINDOW_MODE_STYLE::E_FULL_SCREEN)
	{
		swapChainDesc.Windowed = false; 
	} 
	else 
	{
		swapChainDesc.Windowed = true; 
	} 
	// 스캔라인의 정렬과 스캔라이닝을 지정되지 않음으로(unspecified) 설정합니다. 
	swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED; //디스플레이 스캔라인 모드
	swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED; //디스플레이 비례 모드
	// 출력된 이후의 백버퍼의 내용을 버리도록 합니다. 
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD; 
	// 전체화면 전환시 후면버퍼 설정에 잘부합하는 디스플레이 모드 설정됨 
	swapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	//스왑체인 생성
	HR(pFactory->CreateSwapChain(m_pDevice, &swapChainDesc, &m_pSwapChain));

	//알트 + 앤더 먹지 않게 해줌
	HR(pFactory->MakeWindowAssociation(hwnd, DXGI_MWA_NO_WINDOW_CHANGES));

	//펙토리 할당 해제
	RELEASE_COM(pFactory);

	OnResize(screenWidth, screenHeight);

	return true;
}

void c_D3d::Shutdown()
{
	if (m_pSwapChain)
	{
		m_pSwapChain->SetFullscreenState(false, NULL);
	}
	
	RELEASE_COM(m_pRasterState);
	RELEASE_COM(m_pDepthStencilView);
	RELEASE_COM(m_pDepthStencilState);
	RELEASE_COM(m_pDepthStencilBuffer);
	RELEASE_COM(m_pRenderTargetView);
	RELEASE_COM(m_pDeviceContext);
	RELEASE_COM(m_pDevice);
	RELEASE_COM(m_pSwapChain);
}

void c_D3d::BeginScene(const float& red, const float& green,
	const float& blue, const float& alpha)
{
	XMVECTORF32 color = {red, green, blue, alpha};

	// 벡버퍼 color로 지워줌
	m_pDeviceContext->ClearRenderTargetView(m_pRenderTargetView, color);
	// 깊이버퍼도 지워줌 
	m_pDeviceContext->ClearDepthStencilView(m_pDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
}

void c_D3d::EndScene()
{ 
	// 벡버퍼에 올린 그림 화면으로 보냄 
	if (m_vsync_enabled)
	{
		// 수직동기로 그려줌
		HR(m_pSwapChain->Present(1, 0));
	}
	else
	{
		// 가능한 빨리 그려줌
		HR(m_pSwapChain->Present(0, 0));
	}
}

void c_D3d::GetVideoCardInfo(char * cardName, SIZE_T & memory)
{
	cardName = const_cast<char*>(m_videoCardDescription.c_str());
	memory = m_videoCardMemory;
}

void c_D3d::OnResize(int screenWidth, int screenHeight)
{
	//이 함수에 쓰일 변수들 채크
	assert(m_pDeviceContext);
	assert(m_pDevice);
	assert(m_pSwapChain);

	//사용한 COM객체들 릴리즈
	RELEASE_COM(m_pRenderTargetView);
	RELEASE_COM(m_pDepthStencilView);
	RELEASE_COM(m_pDepthStencilBuffer);

	//버퍼 크기 재설정
	HR(m_pSwapChain->ResizeBuffers(1, screenWidth, screenHeight, DXGI_FORMAT_R8G8B8A8_UNORM, 0));
	ID3D11Texture2D* backBuffer;
	HR(m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&backBuffer)));
	//랜더 타겟 재설정
	HR(m_pDevice->CreateRenderTargetView(backBuffer, 0, &m_pRenderTargetView));
	RELEASE_COM(backBuffer);

	//깊이 버퍼 설정
	D3D11_TEXTURE2D_DESC depthBufferDesc;
	ZeroMemory(&depthBufferDesc, sizeof(D3D11_TEXTURE2D_DESC));

	depthBufferDesc.Width = screenWidth;
	depthBufferDesc.Height = screenHeight;
	depthBufferDesc.MipLevels = 1;
	depthBufferDesc.ArraySize = 1;
	depthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;

	// 멀티셈플링 유무
	if (ENABLE_4X_MSAA)
	{
		depthBufferDesc.SampleDesc.Count = 4;
		depthBufferDesc.SampleDesc.Quality = m_4xMsaaQuality - 1;
	}
	// 멀티셈플링 안함
	else
	{
		depthBufferDesc.SampleDesc.Count = 1;
		depthBufferDesc.SampleDesc.Quality = 0;
	}

	depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthBufferDesc.CPUAccessFlags = 0;
	depthBufferDesc.MiscFlags = 0;

	//깊이 버퍼 텍스처 생성
	HR(m_pDevice->CreateTexture2D(&depthBufferDesc, 0, &m_pDepthStencilBuffer));
	
	// 스텐실 상태의 description을 초기화합니다.
	D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
	ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));
	// 스텐실 상태의 description을 작성합니다.
	depthStencilDesc.DepthEnable = true;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;
	depthStencilDesc.StencilEnable = true;
	depthStencilDesc.StencilReadMask = 0xFF;
	depthStencilDesc.StencilWriteMask = 0xFF;
	// Stencil operations if pixel is front-facing.
	depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	// Stencil operations if pixel is back-facing.
	depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// 깊이-스텐실 상태를 생성
	HR(m_pDevice->CreateDepthStencilState(&depthStencilDesc, &m_pDepthStencilState));

	// 깊이-스텐실 상태를 설정
	m_pDeviceContext->OMSetDepthStencilState(m_pDepthStencilState, 1);

	// 깊이-스텐실 뷰의 description을 초기화
	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
	ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));

	// 깊이-스텐실 뷰의 description을 작성합니다.
	depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0;

	// 깊이-스텐실 뷰를 생성
	HR(m_pDevice->CreateDepthStencilView(m_pDepthStencilBuffer, &depthStencilViewDesc, &m_pDepthStencilView));

	//랜더타갯과 깊이 버퍼 바인드 시켜줌
	m_pDeviceContext->OMSetRenderTargets(1, &m_pRenderTargetView, m_pDepthStencilView);

	//레스터라이저 Desc 선언
	D3D11_RASTERIZER_DESC rasterDesc;
	ZeroMemory(&rasterDesc, sizeof(D3D11_RASTERIZER_DESC));
	//초기화
	rasterDesc.AntialiasedLineEnable = false;
	rasterDesc.CullMode = D3D11_CULL_BACK;
	rasterDesc.DepthBias = 0;
	rasterDesc.DepthBiasClamp = 0.0f;
	rasterDesc.DepthClipEnable = true;
	rasterDesc.FillMode = D3D11_FILL_SOLID;
	rasterDesc.FrontCounterClockwise = false;
	rasterDesc.MultisampleEnable = false;
	rasterDesc.ScissorEnable = false;
	rasterDesc.SlopeScaledDepthBias = 0.0f;

	// 작성한 description으로부터 래스터화기 상태를 생성
	HR(m_pDevice->CreateRasterizerState(&rasterDesc, &m_pRasterState));

	// 래스터화기 상태를 설정
	m_pDeviceContext->RSSetState(m_pRasterState);

	//뷰포트 재설정
	m_ScreenViewport.TopLeftX = 0.0f;
	m_ScreenViewport.TopLeftY = 0.0f;
	m_ScreenViewport.Width = static_cast<float>(screenWidth);
	m_ScreenViewport.Height = static_cast<float>(screenHeight);
	m_ScreenViewport.MinDepth = 0.0f;
	m_ScreenViewport.MaxDepth = 1.0f;

	m_pDeviceContext->RSSetViewports(1, &m_ScreenViewport);
}
