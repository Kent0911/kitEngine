#include "stdafx.h"

#include "../Inc/CKitEngine.h"

using namespace kit::Engine;

FrameRate::FrameRate() {
	m_uiNumerator = DEFALUT_NUMERATOR;
	m_uiDenominator = DEFALUT_DENOMINATOR;
}

FrameRate::~FrameRate() {

}

FrameRate FrameRate::operator= (const FrameRate& _frameRate) {
	this->m_uiNumerator = _frameRate.m_uiNumerator;
	this->m_uiDenominator = _frameRate.m_uiDenominator;
	return *this;
}

HRESULT KitEngine::InitDevice() {
	HRESULT hr = S_OK;

	RECT rc;
	GetClientRect(m_hWnd, &rc);
	UINT width = rc.right - rc.left + 16;
	UINT height = rc.bottom - rc.top + 31;

	UINT createDeviceFlags = 0;
#ifdef _DEBUG
	createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	D3D_DRIVER_TYPE driverTypes[] =
	{
		D3D_DRIVER_TYPE_HARDWARE,
		D3D_DRIVER_TYPE_WARP,
		D3D_DRIVER_TYPE_REFERENCE,
	};
	UINT numDriverTypes = ARRAYSIZE(driverTypes);

	D3D_FEATURE_LEVEL featureLevels[] =
	{
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
	};
	UINT numFeatureLevels = ARRAYSIZE(featureLevels);

	DXGI_SWAP_CHAIN_DESC sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.BufferCount = 1;
	sd.BufferDesc.Width = width;
	sd.BufferDesc.Height = height;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.RefreshRate.Numerator = m_cConfig.m_cFrameRate.Numerator();
	sd.BufferDesc.RefreshRate.Denominator = m_cConfig.m_cFrameRate.Denominator();
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.OutputWindow = m_hWnd;
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.Windowed = TRUE;

	for (UINT driverTypeIndex = 0; driverTypeIndex < numDriverTypes; ++driverTypeIndex) {
		m_driverType = driverTypes[driverTypeIndex];
		hr = D3D11CreateDeviceAndSwapChain(nullptr, m_driverType, nullptr, createDeviceFlags, featureLevels, numFeatureLevels,
			D3D11_SDK_VERSION, &sd, &m_pSwapChain, &m_pd3dDevice, &m_featureLevel, &m_pImmediateContext);
		if (SUCCEEDED(hr))
			break;
	}
	if (FAILED(hr)) { return hr; }


	// Create a render target view
	ID3D11Texture2D* pBackBuffer = nullptr;
	hr = m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
	if (FAILED(hr))
		return hr;

	hr = m_pd3dDevice->CreateRenderTargetView(pBackBuffer, nullptr, &m_pRenderTargetView);
	pBackBuffer->Release();
	if (FAILED(hr))
		return hr;

	// Create depth stencil texture
	D3D11_TEXTURE2D_DESC descDepth;
	ZeroMemory(&descDepth, sizeof(descDepth));
	descDepth.Width = width;
	descDepth.Height = height;
	descDepth.MipLevels = 1;
	descDepth.ArraySize = 1;
	descDepth.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	descDepth.SampleDesc.Count = 1;
	descDepth.SampleDesc.Quality = 0;
	descDepth.Usage = D3D11_USAGE_DEFAULT;
	descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	descDepth.CPUAccessFlags = 0;
	descDepth.MiscFlags = 0;
	hr = m_pd3dDevice->CreateTexture2D(&descDepth, nullptr, &m_pDepthStencil);
	if (FAILED(hr))
		return hr;

	return S_OK;
}

HRESULT KitEngine::HCreateWindow(HINSTANCE _hInstance,int _nCmdShow) {
	m_hInst = _hInstance;
	m_hWnd = CreateWindow(m_uptrWindow->GetWcex().lpszClassName, m_cConfig.m_lpstr, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, m_uptrWindow->GetRect().right - m_uptrWindow->GetRect().left, m_uptrWindow->GetRect().bottom - m_uptrWindow->GetRect().top,
		nullptr, nullptr, m_hInst, nullptr);
	if (!m_hWnd) { return E_FAIL; }

	ShowWindow(m_hWnd, _nCmdShow);

	return S_OK;
}

KitEngine::KitEngine(){	}

bool KitEngine::Create(HINSTANCE _hInstance, int _nCmdShow, LPCONFIG _config, std::shared_ptr<Scene> _startScene) {
	m_cConfig = *_config;
	m_uptrWindow.reset( new CWindow ( _hInstance, _nCmdShow, m_cConfig.m_vWindowSize) );
	if ( nullptr == m_uptrWindow.get() ) { return false; }
	AdjustWindowRect(&m_uptrWindow.get()->GetRect(), WS_OVERLAPPEDWINDOW, FALSE);
	HCreateWindow(_hInstance, _nCmdShow);
	InitDevice();
	m_uptrSceneManager.reset( new SceneManager (_startScene) );
	if ( nullptr == m_uptrSceneManager.get() ) { return false; }
	return true;
}

void KitEngine::ChangeScene(std::shared_ptr<Scene> _changeScene) {
	m_uptrSceneManager->ChangeScene(_changeScene);
}

void KitEngine::Update() {
	m_uptrSceneManager->Update();
}

void KitEngine::Render() {
	m_uptrSceneManager->Render();

	m_pSwapChain->Present(0, 0);
}