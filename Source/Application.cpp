#include "PreCompiledHeaders.h"
#include "Application.h"

inline void ThrowIfFailed(HRESULT hr)
{
	if (FAILED(hr))
	{
		throw std::exception();
	}
}

LRESULT CALLBACK WindowProcess(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
App* App::m_app = nullptr;

App::App()
{
	assert(m_app == nullptr);
	m_app = this;
}

App::~App()
{
}

bool App::Initialize()
{
	if (!InitWindow()) return false;
	if (!InitDX()) return false;
	return true;
}

int App::Run()
{
	MSG msg = { 0 };

	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			Update();
			Draw();
		}
	}

	return (int)msg.wParam;
}

LRESULT App::ProcessMessage(HWND Hwnd, UINT message, WPARAM wparam, LPARAM lparam)
{
	switch (message)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(Hwnd, message, wparam, lparam);
}

#pragma region Private

bool App::InitWindow()
{
	InitializeVariables();
	if (!CreateWindowClass()) return false;
	if (!InitializeWindow()) return false;
	return true;
}

void App::InitializeVariables()
{
	LoadString(HInstance(), IDS_PERGAMENAME, m_windowTitle, MAX_NAME_STRING);
	LoadString(HInstance(), IDS_WINDOWCLASS, m_windowClass, MAX_NAME_STRING);
	m_hIcon = LoadIcon(HInstance(), MAKEINTRESOURCE(IDI_MAINICON));
}

bool App::CreateWindowClass()
{
	WNDCLASSEX wcex;
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WindowProcess;
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = HInstance();
	wcex.hIcon = m_hIcon;
	wcex.hIconSm = m_hIcon;
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)GetStockObject(NULL_BRUSH);
	wcex.lpszMenuName = nullptr;
	wcex.lpszClassName = m_windowClass;
	return RegisterClassEx(&wcex);
}

bool App::InitializeWindow()
{
	m_mainWindow = CreateWindow(m_windowClass, m_windowTitle, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, m_windowWidth, m_windowHeight, nullptr, nullptr, HInstance(), nullptr);

	if (!m_mainWindow)
	{
		MessageBox(0, L"Failed to create Window !", 0, 0);
		return false;
	}

	ShowWindow(m_mainWindow, SW_SHOW);
	UpdateWindow(m_mainWindow);
	return true;
}

bool App::InitDX()
{
	IDXGIFactory7* factory;
	UINT factoryFlags = 0;
	HRESULT result = CreateDXGIFactory2(factoryFlags, IID_PPV_ARGS(&factory));

	IDXGIAdapter1* adapter;
	for (UINT adapterIndex = 0;
		DXGI_ERROR_NOT_FOUND != factory->EnumAdapters1(adapterIndex, &adapter);
		++adapterIndex)
	{
		DXGI_ADAPTER_DESC1 desc;
		adapter->GetDesc1(&desc);

		// Don't select the Basic Render Driver adapter.
		if (desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE)
		{
			continue;
		}

		// Check if the adapter supports Direct3D 12, and use that for the rest
		// of the application
		if (SUCCEEDED(D3D12CreateDevice(adapter, D3D_FEATURE_LEVEL_12_0,
			_uuidof(ID3D12Device), nullptr)))
		{
			break;
		}

		// Else we won't use this iteration's adapter, so release it
		adapter->Release();
	}
	
	// Declare Handles
	ID3D12Device* device;

	// Create Device
	ThrowIfFailed(D3D12CreateDevice(adapter, D3D_FEATURE_LEVEL_12_0, IID_PPV_ARGS(&device)));

	return true;
}

void App::Update()
{

}

void App::Draw()
{

}

#pragma endregion Private