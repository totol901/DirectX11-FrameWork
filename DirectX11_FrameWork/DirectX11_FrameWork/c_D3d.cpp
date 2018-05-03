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
	// vsync(��������ȭ) ���� ����
	m_vsync_enabled = vsync;

	// DirectX �׷��� �������̽� ���丮�� ����
	IDXGIFactory* pFactory;
	HR(CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&pFactory));

	//���丮 ��ü�� �̿��Ͽ�, ������(�׷��� ī��)�� ���Ϳ� �����
	IDXGIAdapter* pTempAdapter;
	UINT mAdpter = 0;

	while (pFactory->EnumAdapters(mAdpter, &pTempAdapter) != DXGI_ERROR_NOT_FOUND)
	{
		m_vAdapters.push_back(pTempAdapter);
		++mAdpter;
	}
	//����� ���� �־���
	mAdpter = m_vAdapters.size();

	//���� ��� 
	//����
	ofstream fon("VideoInfo.txt", ios::out);

	wostringstream outs;
	
	UINT MaxGrapicCardMemIndex = 0;
	SIZE_T MaxMemSize = 0;
	for (UINT i = 0; i < mAdpter; i++)
	{
		//��� ������ ��� ���� ����ü�� �־���
		DXGI_ADAPTER_DESC desc;
		m_vAdapters[i]->GetDesc(&desc);
		
		//wchar -> char�� ����ȯ
		char* pStr = Convert_WCHAR_To_CHAR(desc.Description);

		//stringstream�� �־���
		outs << "�׷��� ī�� �̸� : " << pStr << "\n"
			<< "���� �޸� : " << desc.DedicatedVideoMemory / 1024 / 1024 << "\n"
			<< "�ý��� �޸� : " << desc.DedicatedSystemMemory / 1024 / 1024 << "\n"
			<< "Shared �ý��� �޸� : " << desc.SharedSystemMemory / 1024 / 1024 << "\n"
			<< "\n";

		//���� ū �޸� ũ���� ����̽��� ã�´�.
		if (MaxMemSize < desc.DedicatedVideoMemory)
		{
			MaxGrapicCardMemIndex = i;
			MaxMemSize = desc.DedicatedVideoMemory;
			//�� ����̽��� �̸��� �޸𸮾� ����
			m_videoCardDescription = pStr;
			m_videoCardMemory = desc.DedicatedVideoMemory / 1024 / 1024;
		}

		//�����Ҵ� ����
		SAFE_DELETE_ARRAY(pStr);
	}

	// ���(�����)�� �׷��� �޸𸮰� ���� ū �ƴ��͸� ��������
	UINT mDefaultOutputCounts = 0;
	IDXGIOutput* pOutput;
	HR(m_vAdapters[MaxGrapicCardMemIndex]->EnumOutputs(mDefaultOutputCounts,
		&pOutput));
	
	//���� ������� �ƿ�ǲ�� �����Ǵ� ���÷��� ��� �������
	UINT numModes = 0;
	//�ƿ�ǲ�� ���÷��� ��� ���� �޾ƿ�
	HR(pOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM,
		DXGI_ENUM_MODES_INTERLACED,
		&numModes,
		0));

	//���� �̿��Ͽ� [���÷��� ��� DESC] ���� �迭 ����
	DXGI_MODE_DESC* displayModeList = new DXGI_MODE_DESC[numModes];
	//���÷��� ��� DESC�� �־���
	HR(pOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM,
		DXGI_ENUM_MODES_INTERLACED,
		&numModes,
		displayModeList));

	UINT numerator = 0;
	UINT denominator = 0;
	//DESC �������
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

	//�޸� ����
	SAFE_DELETE_ARRAY(displayModeList);
	
	//���Ͽ� string ����
	fon << outs.str().c_str();
	//����
	fon.close();

	//��� ��� �Ҵ� ����
	RELEASE_COM(pOutput);

	//��� �Ҵ� ����
	for (UINT i = 0; i < m_vAdapters.size(); i++)
	{
		RELEASE_COM(m_vAdapters[i]);
	}


	UINT createDeviceFlags = 0; // ����̽� �÷��� ����(Default)
#if defined(DEBUG) || defined(_DEBUG)  
								//������ ����� �������
	createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	//FeatureLevel ����(�ְ� ������ Level �ޱ�����)
	D3D_FEATURE_LEVEL featureLevel;
	HRESULT hr = D3D11CreateDevice(
		0,                 // default adapter(�⺻ ���÷��� ����� ����)
		D3D_DRIVER_TYPE_HARDWARE,	   // 3���� �׷��� ���� ����
		0,                 // no software device
		createDeviceFlags, // ����̽� �÷��� ����
		0, 0,              // default feature level array
		D3D11_SDK_VERSION, // �׻� D3D11_SDK_VERSION ����
		&m_pDevice,	   // d3d��ġ �־��� (Out ��)
		&featureLevel,	   // featureLevel �־���[�� ��밡�� �ְ� ������ Level ����] (OUt ��)
		&m_pDeviceContext); // ��ġ ����(Context) �־��� (Out ��)

	//md3dDevice ���� �Ϸ�̳� �˻�
	if (FAILED(hr))
	{
		MessageBox(0, L"D3D11CreateDevice ���� ����.", 0, 0);
		return false;
	}

	//FeatureLevel�� ���� �˻�
	if (featureLevel < D3D_FEATURE_LEVEL_11_0)
	{
		MessageBox(0, L"Direct3D Feature Level 11 ���� ����.", 0, 0);
		return false;
	}

	//�˸��� ��Ƽ���ø� ����Ƽ ����
	HR(m_pDevice->CheckMultisampleQualityLevels(
		DXGI_FORMAT_R8G8B8A8_UNORM, 4, &m_4xMsaaQuality));
	//�ùٸ� �������� äũ
	assert(m_4xMsaaQuality > 0);

	//��ȯ �罽 ����
	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	//��ȯ �罽 description�� �ʱ�ȭ�մϴ�.
	ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));

	//�ϳ��� ����۸��� ����ϵ��� �մϴ�.
	swapChainDesc.BufferCount = 1;

	//������� �ʺ�� ���̸� �����մϴ�.
	swapChainDesc.BufferDesc.Width = screenWidth;
	swapChainDesc.BufferDesc.Height = screenHeight;

	//����۷� �Ϲ����� 32bit�� �����̽��� �����մϴ�.
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;//�ĸ� ���� �ȼ� ����

	// ������� ���ΰ�ħ ������ �����մϴ�. 
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
	// ������� �뵵�� �����մϴ�. 
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT; 
	// �������� �̷���� �������� �ڵ��� �����մϴ�. 
	swapChainDesc.OutputWindow = hwnd; 
	// ��Ƽ���ø� ���� Ȯ��
	if (ENABLE_4X_MSAA)
	{
		swapChainDesc.SampleDesc.Count = 4;
		swapChainDesc.SampleDesc.Quality = m_4xMsaaQuality - 1;
	}
	// ��Ƽ���ø� �Ⱦ�
	else
	{
		swapChainDesc.SampleDesc.Count = 1;
		swapChainDesc.SampleDesc.Quality = 0;
	}
	// ������ ��� �Ǵ� Ǯ��ũ�� ��带 �����մϴ�. 
	if(GlobalManager::GetContorlWindow()->GetWindowModeStyle()
		== E_WINDOW_MODE_STYLE::E_FULL_SCREEN)
	{
		swapChainDesc.Windowed = false; 
	} 
	else 
	{
		swapChainDesc.Windowed = true; 
	} 
	// ��ĵ������ ���İ� ��ĵ���̴��� �������� ��������(unspecified) �����մϴ�. 
	swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED; //���÷��� ��ĵ���� ���
	swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED; //���÷��� ��� ���
	// ��µ� ������ ������� ������ �������� �մϴ�. 
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD; 
	// ��üȭ�� ��ȯ�� �ĸ���� ������ �ߺ����ϴ� ���÷��� ��� ������ 
	swapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	//����ü�� ����
	HR(pFactory->CreateSwapChain(m_pDevice, &swapChainDesc, &m_pSwapChain));

	//��Ʈ + �ش� ���� �ʰ� ����
	HR(pFactory->MakeWindowAssociation(hwnd, DXGI_MWA_NO_WINDOW_CHANGES));

	//���丮 �Ҵ� ����
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

	// ������ color�� ������
	m_pDeviceContext->ClearRenderTargetView(m_pRenderTargetView, color);
	// ���̹��۵� ������ 
	m_pDeviceContext->ClearDepthStencilView(m_pDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
}

void c_D3d::EndScene()
{ 
	// �����ۿ� �ø� �׸� ȭ������ ���� 
	if (m_vsync_enabled)
	{
		// ��������� �׷���
		HR(m_pSwapChain->Present(1, 0));
	}
	else
	{
		// ������ ���� �׷���
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
	//�� �Լ��� ���� ������ äũ
	assert(m_pDeviceContext);
	assert(m_pDevice);
	assert(m_pSwapChain);

	//����� COM��ü�� ������
	RELEASE_COM(m_pRenderTargetView);
	RELEASE_COM(m_pDepthStencilView);
	RELEASE_COM(m_pDepthStencilBuffer);

	//���� ũ�� �缳��
	HR(m_pSwapChain->ResizeBuffers(1, screenWidth, screenHeight, DXGI_FORMAT_R8G8B8A8_UNORM, 0));
	ID3D11Texture2D* backBuffer;
	HR(m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&backBuffer)));
	//���� Ÿ�� �缳��
	HR(m_pDevice->CreateRenderTargetView(backBuffer, 0, &m_pRenderTargetView));
	RELEASE_COM(backBuffer);

	//���� ���� ����
	D3D11_TEXTURE2D_DESC depthBufferDesc;
	ZeroMemory(&depthBufferDesc, sizeof(D3D11_TEXTURE2D_DESC));

	depthBufferDesc.Width = screenWidth;
	depthBufferDesc.Height = screenHeight;
	depthBufferDesc.MipLevels = 1;
	depthBufferDesc.ArraySize = 1;
	depthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;

	// ��Ƽ���ø� ����
	if (ENABLE_4X_MSAA)
	{
		depthBufferDesc.SampleDesc.Count = 4;
		depthBufferDesc.SampleDesc.Quality = m_4xMsaaQuality - 1;
	}
	// ��Ƽ���ø� ����
	else
	{
		depthBufferDesc.SampleDesc.Count = 1;
		depthBufferDesc.SampleDesc.Quality = 0;
	}

	depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthBufferDesc.CPUAccessFlags = 0;
	depthBufferDesc.MiscFlags = 0;

	//���� ���� �ؽ�ó ����
	HR(m_pDevice->CreateTexture2D(&depthBufferDesc, 0, &m_pDepthStencilBuffer));
	
	// ���ٽ� ������ description�� �ʱ�ȭ�մϴ�.
	D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
	ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));
	// ���ٽ� ������ description�� �ۼ��մϴ�.
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

	// ����-���ٽ� ���¸� ����
	HR(m_pDevice->CreateDepthStencilState(&depthStencilDesc, &m_pDepthStencilState));

	// ����-���ٽ� ���¸� ����
	m_pDeviceContext->OMSetDepthStencilState(m_pDepthStencilState, 1);

	// ����-���ٽ� ���� description�� �ʱ�ȭ
	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
	ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));

	// ����-���ٽ� ���� description�� �ۼ��մϴ�.
	depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0;

	// ����-���ٽ� �並 ����
	HR(m_pDevice->CreateDepthStencilView(m_pDepthStencilBuffer, &depthStencilViewDesc, &m_pDepthStencilView));

	//����Ÿ���� ���� ���� ���ε� ������
	m_pDeviceContext->OMSetRenderTargets(1, &m_pRenderTargetView, m_pDepthStencilView);

	//�����Ͷ����� Desc ����
	D3D11_RASTERIZER_DESC rasterDesc;
	ZeroMemory(&rasterDesc, sizeof(D3D11_RASTERIZER_DESC));
	//�ʱ�ȭ
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

	// �ۼ��� description���κ��� ������ȭ�� ���¸� ����
	HR(m_pDevice->CreateRasterizerState(&rasterDesc, &m_pRasterState));

	// ������ȭ�� ���¸� ����
	m_pDeviceContext->RSSetState(m_pRasterState);

	//����Ʈ �缳��
	m_ScreenViewport.TopLeftX = 0.0f;
	m_ScreenViewport.TopLeftY = 0.0f;
	m_ScreenViewport.Width = static_cast<float>(screenWidth);
	m_ScreenViewport.Height = static_cast<float>(screenHeight);
	m_ScreenViewport.MinDepth = 0.0f;
	m_ScreenViewport.MaxDepth = 1.0f;

	m_pDeviceContext->RSSetViewports(1, &m_ScreenViewport);
}
