//***************************************************************************************
// d3dApp.cpp by Frank Luna (C) 2011 All Rights Reserved.
//***************************************************************************************

#include "d3dApp.h"
#include <WindowsX.h>
#include <sstream>

namespace
{
	// This is just used to forward Windows messages from a global window
	// procedure to our member function window procedure because we cannot
	// assign a member function to WNDCLASS::lpfnWndProc.
	// 가상 프로시저 함수를 위해서 만듬
	D3DApp* gd3dApp = 0;
}

LRESULT CALLBACK MainWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	// Forward hwnd on because we can get messages (e.g., WM_CREATE)
	// before CreateWindow returns, and thus before mhMainWnd is valid.
	// 위의 namespace로 부터 가져옴
	return gd3dApp->MsgProc(hwnd, msg, wParam, lParam);
}

D3DApp::D3DApp(HINSTANCE hInstance)
:	mhAppInst(hInstance),					 //winmain으로 부터 받은 hInstance로 초기화
	mMainWndCaption(L"D3D11 Application"),	 //윈도우 이름
	md3dDriverType(D3D_DRIVER_TYPE_HARDWARE),//3차원 그래픽 가속 적용
	mClientWidth(800),						 //가로 크기
	mClientHeight(600),						 //세로 크기
	mEnable4xMsaa(false),					 //안티얼라이즈 false
	mhMainWnd(0),							 //윈도우 헨들
	mAppPaused(false),						 //앱 정지용 bool
	mMinimized(false),						 //최대화 bool
	mMaximized(false),						 //최소화 bool
	mResizing(false),						 //크기 재설정 bool
	m4xMsaaQuality(0),						 //안티얼라이즈 퀄리티
 
	md3dDevice(0),							//DirectX11 COM디바이스
	md3dImmediateContext(0),				//DirectX11 COM컨택스트
	mSwapChain(0),							//DirectX11 COM스왑 체인
	mDepthStencilBuffer(0),					//DirectX11 COM스텐실 깊이 버퍼
	mRenderTargetView(0),					//DirectX11 COM랜더타겟 뷰
	mDepthStencilView(0),					//DirectX11 COM스텐실 뷰

	mAdpter(0),								//어뎁터(그래픽) 개수
	mDefaultOutputCounts(0)					//기본 어뎁터의 Output 개수
{
	//스크린 뷰포트 0으로 세팅
	ZeroMemory(&mScreenViewport, sizeof(D3D11_VIEWPORT));

	// Get a pointer to the application object so we can forward 
	// Windows messages to the object's window procedure through
	// the global window procedure.
	// App 포인터 넣어줌
	gd3dApp = this;
}

D3DApp::~D3DApp()
{
	//COM객체들 릴리즈 해줌
	ReleaseCOM(mRenderTargetView);
	ReleaseCOM(mDepthStencilView);
	ReleaseCOM(mSwapChain);
	ReleaseCOM(mDepthStencilBuffer);

	// Restore all default settings.
	// 컨택스트 default 설정 해줌
	if( md3dImmediateContext )
		md3dImmediateContext->ClearState();

	//COM객체들 릴리즈 해줌
	ReleaseCOM(md3dImmediateContext);
	ReleaseCOM(md3dDevice);
}

HINSTANCE D3DApp::AppInst()const
{
	//앱(윈도우) hInstance 리턴
	return mhAppInst;
}

HWND D3DApp::MainWnd()const
{
	//앱(윈도우) 헨들 리턴
	return mhMainWnd;
}

float D3DApp::AspectRatio()const
{
	//종횡비 계산 리턴
	return static_cast<float>(mClientWidth) / mClientHeight;
}

int D3DApp::Run()
{
	//윈도우 메시지 선언 및 초기화
	MSG msg = {0};
 
	//타이머 리셋
	mTimer.Reset();

	while(msg.message != WM_QUIT)
	{
		// If there are Window messages then process them.
		if(PeekMessage( &msg, 0, 0, 0, PM_REMOVE ))
		{
            TranslateMessage( &msg );
            DispatchMessage( &msg );
		}
		// Otherwise, do animation/game stuff.
		else
        {	
			mTimer.Tick();

			if( !mAppPaused )
			{
				CalculateFrameStats();
				UpdateScene(mTimer.DeltaTime());	
				DrawScene();
			}
			else
			{
				Sleep(100);
			}
        }
    }

	return (int)msg.wParam;
}

bool D3DApp::Init()
{
	//윈도우 초기화
	if(!InitMainWindow())
		return false;

	//다이렉트11 초기화
	if(!InitDirect3D())
		return false;

	return true;
}
 
void D3DApp::OnResize()
{
	//이 함수에 쓰일 변수들 채크
	assert(md3dImmediateContext);
	assert(md3dDevice);
	assert(mSwapChain);

	// Release the old views, as they hold references to the buffers we
	// will be destroying.  Also release the old depth/stencil buffer.

	//사용한 COM객체들 릴리즈
	ReleaseCOM(mRenderTargetView);
	ReleaseCOM(mDepthStencilView);
	ReleaseCOM(mDepthStencilBuffer);


	// Resize the swap chain and recreate the render target view.

	//버퍼 크기 재설정
	HR(mSwapChain->ResizeBuffers(1, mClientWidth, mClientHeight, DXGI_FORMAT_R8G8B8A8_UNORM, 0));
	ID3D11Texture2D* backBuffer;
	HR(mSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&backBuffer)));
	//랜더 타겟 재설정
	HR(md3dDevice->CreateRenderTargetView(backBuffer, 0, &mRenderTargetView));
	ReleaseCOM(backBuffer);

	// Create the depth/stencil buffer and view.

	//깊이 버퍼 설정
	D3D11_TEXTURE2D_DESC depthStencilDesc;
	
	depthStencilDesc.Width     = mClientWidth;
	depthStencilDesc.Height    = mClientHeight;
	depthStencilDesc.MipLevels = 1;
	depthStencilDesc.ArraySize = 1;
	depthStencilDesc.Format    = DXGI_FORMAT_D24_UNORM_S8_UINT;

	// Use 4X MSAA? --must match swap chain MSAA values.
	if( mEnable4xMsaa )
	{
		depthStencilDesc.SampleDesc.Count   = 4;
		depthStencilDesc.SampleDesc.Quality = m4xMsaaQuality-1;
	}
	// No MSAA
	else
	{
		depthStencilDesc.SampleDesc.Count   = 1;
		depthStencilDesc.SampleDesc.Quality = 0;
	}

	depthStencilDesc.Usage          = D3D11_USAGE_DEFAULT;
	depthStencilDesc.BindFlags      = D3D11_BIND_DEPTH_STENCIL;
	depthStencilDesc.CPUAccessFlags = 0; 
	depthStencilDesc.MiscFlags      = 0;

	HR(md3dDevice->CreateTexture2D(&depthStencilDesc, 0, &mDepthStencilBuffer));
	HR(md3dDevice->CreateDepthStencilView(mDepthStencilBuffer, 0, &mDepthStencilView));


	// Bind the render target view and depth/stencil view to the pipeline.
	//랜더타갯과 깊이 버퍼 바인드 시켜줌
	md3dImmediateContext->OMSetRenderTargets(1, &mRenderTargetView, mDepthStencilView);
	

	// Set the viewport transform.
	//뷰포트 재설정
	mScreenViewport.TopLeftX = 0.0f;
	mScreenViewport.TopLeftY = 0.0f;
	mScreenViewport.Width    = mClientWidth;
	mScreenViewport.Height   = mClientHeight;
	mScreenViewport.MinDepth = 0.0f;
	mScreenViewport.MaxDepth = 1.0f;

	md3dImmediateContext->RSSetViewports(1, &mScreenViewport);
}
 
LRESULT D3DApp::MsgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch( msg )
	{
	// WM_ACTIVATE is sent when the window is activated or deactivated.  
	// We pause the game when the window is deactivated and unpause it 
	// when it becomes active.  
	case WM_ACTIVATE:
		if( LOWORD(wParam) == WA_INACTIVE )
		{
			mAppPaused = true;
			mTimer.Stop();
		}
		else
		{
			mAppPaused = false;
			mTimer.Start();
		}
		return 0;

	// WM_SIZE is sent when the user resizes the window.  
	case WM_SIZE:
		// Save the new client area dimensions.
		mClientWidth  = LOWORD(lParam);
		mClientHeight = HIWORD(lParam);
		if( md3dDevice )
		{
			if( wParam == SIZE_MINIMIZED )
			{
				mAppPaused = true;
				mMinimized = true;
				mMaximized = false;
			}
			else if( wParam == SIZE_MAXIMIZED )
			{
				mAppPaused = false;
				mMinimized = false;
				mMaximized = true;
				OnResize();
			}
			else if( wParam == SIZE_RESTORED )
			{
				
				// Restoring from minimized state?
				if( mMinimized )
				{
					mAppPaused = false;
					mMinimized = false;
					OnResize();
				}

				// Restoring from maximized state?
				else if( mMaximized )
				{
					mAppPaused = false;
					mMaximized = false;
					OnResize();
				}
				else if( mResizing )
				{
					// If user is dragging the resize bars, we do not resize 
					// the buffers here because as the user continuously 
					// drags the resize bars, a stream of WM_SIZE messages are
					// sent to the window, and it would be pointless (and slow)
					// to resize for each WM_SIZE message received from dragging
					// the resize bars.  So instead, we reset after the user is 
					// done resizing the window and releases the resize bars, which 
					// sends a WM_EXITSIZEMOVE message.
				}
				else // API call such as SetWindowPos or mSwapChain->SetFullscreenState.
				{
					OnResize();
				}
			}
		}
		return 0;

	// WM_EXITSIZEMOVE is sent when the user grabs the resize bars.
	case WM_ENTERSIZEMOVE:
		mAppPaused = true;
		mResizing  = true;
		mTimer.Stop();
		return 0;

	// WM_EXITSIZEMOVE is sent when the user releases the resize bars.
	// Here we reset everything based on the new window dimensions.
	case WM_EXITSIZEMOVE:
		mAppPaused = false;
		mResizing  = false;
		mTimer.Start();
		OnResize();
		return 0;
 
	// WM_DESTROY is sent when the window is being destroyed.
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;

	// The WM_MENUCHAR message is sent when a menu is active and the user presses 
	// a key that does not correspond to any mnemonic or accelerator key. 
	case WM_MENUCHAR:
        // Don't beep when we alt-enter.
        return MAKELRESULT(0, MNC_CLOSE);

	// Catch this message so to prevent the window from becoming too small.
	case WM_GETMINMAXINFO:
		((MINMAXINFO*)lParam)->ptMinTrackSize.x = 200;
		((MINMAXINFO*)lParam)->ptMinTrackSize.y = 200; 
		return 0;

	case WM_LBUTTONDOWN:
	case WM_MBUTTONDOWN:
	case WM_RBUTTONDOWN:
		OnMouseDown(wParam, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		return 0;
	case WM_LBUTTONUP:
	case WM_MBUTTONUP:
	case WM_RBUTTONUP:
		OnMouseUp(wParam, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		return 0;
	case WM_MOUSEMOVE:
		OnMouseMove(wParam, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		return 0;
	}

	return DefWindowProc(hwnd, msg, wParam, lParam);
}

void D3DApp::drawDC()
{
	HDC hHdc = GetDC(mhMainWnd);

	//어댑터 갯수 Textout
	std::wostringstream outs;
	outs << L"시스템 어댑터 갯수 : " << mAdpter;// << desc.Description;
	//SetBkMode(hHdc, TRANSPARENT);
	TextOut(hHdc, 0, 0, outs.str().c_str(), outs.str().size());

	//기본(Defualt) 어댑터의 출력대상(Output) 갯수
	std::wostringstream outs_Outputs;
	outs_Outputs << L"기본 어댑터의 아웃풋 갯수 : " << mDefaultOutputCounts;
	TextOut(hHdc, 0, 30, outs_Outputs.str().c_str(), outs_Outputs.str().size());

	ReleaseDC(mhMainWnd, hHdc);
}

bool D3DApp::InitMainWindow()
{
	//윈도우 클래스
	WNDCLASS wc;	
	wc.style         = CS_HREDRAW | CS_VREDRAW;			   //윈도우 수직 수평이 변하면 다시 그려줌 설정
	wc.lpfnWndProc   = MainWndProc;						   //프로시저 함수 설정
	wc.cbClsExtra    = 0;								   //예약 영역 0 설정
	wc.cbWndExtra    = 0;								   //예약 영역 0 설정
	wc.hInstance     = mhAppInst;						   //winmain으로 부터 받은 hInstance 넣어줌
	wc.hIcon         = LoadIcon(0, IDI_APPLICATION);	   //기본 아이콘 불러옴
	wc.hCursor       = LoadCursor(0, IDC_ARROW);		   //기본 마우스 커서 불러옴
	wc.hbrBackground = (HBRUSH)GetStockObject(NULL_BRUSH); //윈도우 창 NULL(흰)값으로 칠해줌
	wc.lpszMenuName  = 0;								   //메뉴 사용 안함 == 0
	wc.lpszClassName = L"D3DWndClassName";				   //윈도우 클래스 이름

	//윈도우 클래스 등록
	if( !RegisterClass(&wc) )
	{
		MessageBox(0, L"RegisterClass Failed.", 0, 0);
		return false;
	}

	// Compute window rectangle dimensions based on requested client area dimensions.
	// 클라이언트 크기 설정
	RECT R = { 0, 0, mClientWidth, mClientHeight };
	//R크기의 클라이언트 설정 이후(윈도우 크기 Out), 메뉴 생성 X
    AdjustWindowRect(&R, WS_OVERLAPPEDWINDOW, false); 
	int width  = R.right - R.left;//윈도우 크기 재계산
	int height = R.bottom - R.top;//윈도우 크기 재계산

	//윈도우 생성
	mhMainWnd = CreateWindow(L"D3DWndClassName", mMainWndCaption.c_str(), 
		WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, width, height, 0, 0, mhAppInst, 0); 
	
	//윈도우 생성 잘됬나 체크
	if( !mhMainWnd )
	{
		MessageBox(0, L"CreateWindow Failed.", 0, 0);
		return false;
	}

	//윈도우 활성화
	ShowWindow(mhMainWnd, SW_SHOW);
	//WM_PAINT 메시지 발생 및 클라이언트 갱신
	UpdateWindow(mhMainWnd);

	return true;
}

bool D3DApp::InitDirect3D()
{
	// Create the device and device context.

	UINT createDeviceFlags = 0; // 디바이스 플레그 설정(Default)
#if defined(DEBUG) || defined(_DEBUG)  
    createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
	//만약 직접 프래임 워크 제작을 할 경우 이 플레그 설정할 것
	//UINT createDeviceFlags = D3D11_CREATE_DEVICE_DEBUG //에러시 디버그 출력해줌
#endif
	//FeatureLevel 선언(최고 수준의 Level 받기위해)
	D3D_FEATURE_LEVEL featureLevel;
	HRESULT hr = D3D11CreateDevice(
			0,                 // default adapter(기본 디스플레이 어댑터 설정)
			md3dDriverType,	   // 3차원 그래픽 가속 적용
			0,                 // no software device
			createDeviceFlags, // 디바이스 플레그 설정
			0, 0,              // default feature level array
			D3D11_SDK_VERSION, // 항상 D3D11_SDK_VERSION 지정
			&md3dDevice,	   // d3d장치 넣어줌 (Out 값)
			&featureLevel,	   // featureLevel 넣어줌[현 사용가능 최고 수준의 Level 나옴] (OUt 값)
			&md3dImmediateContext); // 장치 문맥(Context) 넣어줌 (Out 값)

	//md3dDevice 설정 관료됫나 검사
	if( FAILED(hr) )
	{
		MessageBox(0, L"D3D11CreateDevice Failed.", 0, 0);
		return false;
	}

	//FeatureLevel의 수준 검사
	if( featureLevel != D3D_FEATURE_LEVEL_11_0 )
	{
		MessageBox(0, L"Direct3D Feature Level 11 unsupported.", 0, 0);
		return false;
	}

	// Check 4X MSAA quality support for our back buffer format.
	// All Direct3D 11 capable devices support 4X MSAA for all render 
	// target formats, so we only need to check quality support.
	
	//알맞은 멀티셈플링 퀄리티 세팅
	HR(md3dDevice->CheckMultisampleQualityLevels(
		DXGI_FORMAT_R8G8B8A8_UNORM, 4, &m4xMsaaQuality));
	//올바른 세팅인지 채크
	assert( m4xMsaaQuality > 0 );

	// Fill out a DXGI_SWAP_CHAIN_DESC to describe our swap chain.

	//교환 사슬 설정
	DXGI_SWAP_CHAIN_DESC sd;
	sd.BufferDesc.Width  = mClientWidth;//후면버퍼 너이
	sd.BufferDesc.Height = mClientHeight;//후면버퍼 높이
	sd.BufferDesc.RefreshRate.Numerator = 60;//디스플레이 모드 갱신율(fps)
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;//후면 버퍼 픽셀 형식
	sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;//디스플레이 스캔라인 모드
	sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;//디스플레이 비례 모드

	// Use 4X MSAA? 
	if( mEnable4xMsaa )
	{
		sd.SampleDesc.Count   = 4;
		sd.SampleDesc.Quality = m4xMsaaQuality-1;
	}
	// No MSAA
	else
	{
		sd.SampleDesc.Count   = 1;
		sd.SampleDesc.Quality = 0;
	}

	sd.BufferUsage  = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.BufferCount  = 1;
	sd.OutputWindow = mhMainWnd;
	sd.Windowed     = true;
	sd.SwapEffect   = DXGI_SWAP_EFFECT_DISCARD;
	sd.Flags        = 0;

	// To correctly create the swap chain, we must use the IDXGIFactory that was
	// used to create the device.  If we tried to use a different IDXGIFactory instance
	// (by calling CreateDXGIFactory), we get an error: "IDXGIFactory::CreateSwapChain: 
	// This function is being called with a device from a different IDXGIFactory."

	//교환 사슬 생성(인스턴스 이용)
	IDXGIDevice* dxgiDevice = 0;
	HR(md3dDevice->QueryInterface(__uuidof(IDXGIDevice), (void**)&dxgiDevice));
	
	IDXGIAdapter* dxgiAdapter = 0;
	HR(dxgiDevice->GetParent(__uuidof(IDXGIAdapter), (void**)&dxgiAdapter));
	
	IDXGIFactory* dxgiFactory = 0;
	HR(dxgiAdapter->GetParent(__uuidof(IDXGIFactory), (void**)&dxgiFactory));

	HR(dxgiFactory->CreateSwapChain(md3dDevice, &sd, &mSwapChain));

	//알트 + 앤더 먹지 않게 해줌
	HR(dxgiFactory->MakeWindowAssociation(mhMainWnd, DXGI_MWA_NO_WINDOW_CHANGES));
	
	//시스템 어뎁터 찾아서 넣어줌
	IDXGIAdapter* pTempAdapter;
	while (dxgiFactory->EnumAdapters(mAdpter, &pTempAdapter) != DXGI_ERROR_NOT_FOUND)
	{
		vAdapters.push_back(pTempAdapter);
		++mAdpter;
	}
	//어댑터 갯수 넣어줌
	mAdpter = vAdapters.size();
	//0번째 어댑터 설명 구조체에 넣어줌
	vAdapters[2]->GetDesc(&desc);

	//기본(default) 어댑터의 출력대상 찾아서 넣어주기
	IDXGIOutput* pOutput;
	while (vAdapters[0]->EnumOutputs(mDefaultOutputCounts, &pOutput) != DXGI_ERROR_NOT_FOUND)
	{
		vOutputs.push_back(pOutput);
		++mDefaultOutputCounts;
	}
	//기본 어댑터의 아웃풋 개수 넣어줌
	mDefaultOutputCounts = vOutputs.size();

	//기본 어댑터의 아웃풋에 지원되는 디스플레이 모드 출력해줌
	for (int i = 0; i < vOutputs.size(); i++)
	{
		UINT num = 0;
		//아웃풋의 디스플레이 모드 갯수 받아옴
		HR(vOutputs[i]->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM,
			DXGI_ENUM_MODES_INTERLACED,
			&num,
			0));

		//갯수 이용하여 [디스플레이 모드 DESC] 동적 배열 생성
		DXGI_MODE_DESC* pDescs = new DXGI_MODE_DESC[num];
		//디스플레이 모드 DESC에 넣어줌
		HR(vOutputs[i]->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM,
			DXGI_ENUM_MODES_INTERLACED,
			&num,
			pDescs));

		//DESC 출력해줌
		for (int j = 0; j < num; j++)
		{
			std::wostringstream outs;
			outs << L"***WIDTH = " << pDescs[j].Width
				<< L" HEIGHT = " << pDescs[j].Height
				<< L" REFRESH = " << pDescs[j].RefreshRate.Numerator
				<< L"/" << pDescs[j].RefreshRate.Denominator<<L"\n";
			OutputDebugString(outs.str().c_str());
		}

		//메모리 해제
		delete[] pDescs;
	}
		
	//인터페이스 반납
	//출력 대상(아웃풋) 반납
	for (int i = 0; i < vOutputs.size(); i++)
	{
		ReleaseCOM(vOutputs[i]);
	}
	vOutputs.clear();
	//어댑터 반납
	for (int i = 0; i < vAdapters.size(); i++)
	{
		ReleaseCOM(vAdapters[i]);
	}
	vAdapters.clear();
	//그 외의 것들(인터페이스) 반납
	ReleaseCOM(dxgiDevice);
	ReleaseCOM(dxgiAdapter);
	ReleaseCOM(dxgiFactory);

	// The remaining steps that need to be carried out for d3d creation
	// also need to be executed every time the window is resized.  So
	// just call the OnResize method here to avoid code duplication.
	
	OnResize();

	return true;
}

void D3DApp::CalculateFrameStats()
{
	// Code computes the average frames per second, and also the 
	// average time it takes to render one frame.  These stats 
	// are appended to the window caption bar.

	static int frameCnt = 0;
	static float timeElapsed = 0.0f;

	frameCnt++;

	// Compute averages over one second period.
	if( (mTimer.TotalTime() - timeElapsed) >= 1.0f )
	{
		float fps = (float)frameCnt; // fps = frameCnt / 1
		float mspf = 1000.0f / fps;

		std::wostringstream outs;   
		outs.precision(6);
		outs << mMainWndCaption << L"    "
			 << L"FPS: " << fps << L"    " 
			 << L"Frame Time: " << mspf << L" (ms)";
		SetWindowText(mhMainWnd, outs.str().c_str());
		
		// Reset for next average.
		frameCnt = 0;
		timeElapsed += 1.0f;
	}
}


