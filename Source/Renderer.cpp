#include "PreCompiledHeaders.h"
#include "Renderer.h"

Renderer::Renderer()
{
	InitializeDirectX();
}

Renderer::~Renderer()
{
}

bool Renderer::Initialize()
{
	if (!InitializeDirectX()) return false;
	return true;
}

bool Renderer::InitializeDirectX()
{
	IDXGIFactory4* factory;
	UINT factoryFlags = 0;
	HRESULT result = CreateDXGIFactory2(factoryFlags, IID_PPV_ARGS(&factory));

	IDXGIAdapter1* adapter = nullptr;
	SelectAdapter(adapter, factory);

	ID3D12Device* device;
	ThrowIfFailed(D3D12CreateDevice(adapter, D3D_FEATURE_LEVEL_12_0, IID_PPV_ARGS(&device)));

#ifdef _DEBUG
	InitDebugMode(factoryFlags, device);
#endif

	return true;
}

void Renderer::SelectAdapter(IDXGIAdapter1* adapter, IDXGIFactory4* factory)
{
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
}

#ifdef _DEBUG

void Renderer::InitDebugMode(UINT factoryFlags, ID3D12Device* device)
{
	ID3D12Debug1* debugController;
	ID3D12Debug* dc;
	ThrowIfFailed(D3D12GetDebugInterface(IID_PPV_ARGS(&dc)));
	ThrowIfFailed(dc->QueryInterface(IID_PPV_ARGS(&debugController)));
	debugController->EnableDebugLayer();
	debugController->SetEnableGPUBasedValidation(true);

	factoryFlags |= DXGI_CREATE_FACTORY_DEBUG;

	dc->Release();
	dc = nullptr;

	ID3D12DebugDevice* debugDevice;
	ThrowIfFailed(device->QueryInterface(&debugDevice));
}

#endif // _DEBUG