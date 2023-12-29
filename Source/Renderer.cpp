#include "PreCompiledHeaders.h"
#include "Renderer.h"
#include "Application.h"

#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")

Renderer::Renderer()
{
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
	CreateFactory();
	SelectAdapter();
	CreateDevice();
	CreateFence();
	CacheDescriptorsSize();
	CheckMsaaQualitySupport();

	CreateCommandQueue();
	CreateCommandAllocator();
	CreateCommandList();

	CreateSwapChain();


//#ifdef _DEBUG
//	InitDebugMode(factoryFlags, device);
//#endif

	return true;
}

void Renderer::CreateFactory()
{
	UINT factoryFlags = 0;
	HRESULT result = CreateDXGIFactory2(factoryFlags, IID_PPV_ARGS(&m_factory));
}

void Renderer::SelectAdapter()
{
	for (UINT adapterIndex = 0;
		DXGI_ERROR_NOT_FOUND != m_factory->EnumAdapters1(adapterIndex, &m_adapter);
		++adapterIndex)
	{
		DXGI_ADAPTER_DESC1 desc;
		m_adapter->GetDesc1(&desc);

		// Don't select the Basic Render Driver adapter.
		if (desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE)
		{
			continue;
		}

		// Check if the adapter supports Direct3D 12, and use that for the rest
		// of the application
		if (SUCCEEDED(D3D12CreateDevice(m_adapter, D3D_FEATURE_LEVEL_12_0,
			_uuidof(ID3D12Device), nullptr)))
		{
			break;
		}

		// Else we won't use this iteration's adapter, so release it
		m_adapter->Release();
	}
}

void Renderer::CreateDevice()
{
	ThrowIfFailed(D3D12CreateDevice(m_adapter, D3D_FEATURE_LEVEL_12_0, IID_PPV_ARGS(&m_device)));
}

void Renderer::CreateCommandQueue()
{
	D3D12_COMMAND_QUEUE_DESC queueDesc = {};
	queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
	queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;

	ThrowIfFailed(m_device->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&m_commandQueue)));
}

void Renderer::CreateCommandAllocator()
{
	ThrowIfFailed(m_device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&m_commandAllocator)));
}

void Renderer::CreateCommandList()
{
	ThrowIfFailed(m_device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT,
		m_commandAllocator, nullptr, IID_PPV_ARGS(&m_commandList)));
	m_commandList->Close();
}

void Renderer::CreateFence()
{
	ThrowIfFailed(m_device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&m_fence)));
}

void Renderer::CreateBarrier()
{
	/*ID3D12Resource* texResource;
	m_barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	m_barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
	m_barrier.Transition.pResource = texResource;
	m_barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_COPY_SOURCE;
	m_barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_UNORDERED_ACCESS;
	m_barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;

	m_commandList->ResourceBarrier(1, &m_barrier);*/
}

void Renderer::CacheDescriptorsSize()
{
	m_rtvDescriptorSize = m_device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
	m_dsvDescriptorSize = m_device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_DSV);
	m_cbvSrvUavDescriptorSize = m_device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
}

void Renderer::CheckMsaaQualitySupport()
{
	D3D12_FEATURE_DATA_MULTISAMPLE_QUALITY_LEVELS msQualityLevels;
	msQualityLevels.Format = m_backBufferFormat;
	msQualityLevels.SampleCount = 4;
	msQualityLevels.Flags = D3D12_MULTISAMPLE_QUALITY_LEVELS_FLAG_NONE;
	msQualityLevels.NumQualityLevels = 0;
	ThrowIfFailed(m_device->CheckFeatureSupport(D3D12_FEATURE_MULTISAMPLE_QUALITY_LEVELS,
		&msQualityLevels, sizeof(msQualityLevels)));
	m_msaaQuality = msQualityLevels.NumQualityLevels;
}

void Renderer::CreateSwapChain()
{
	DXGI_SWAP_CHAIN_DESC sd;
	sd.BufferDesc.Width = App::GetApp()->GetWindowWidth();
	sd.BufferDesc.Height = App::GetApp()->GetWindowHeight();
	sd.BufferDesc.RefreshRate.Numerator = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.BufferDesc.Format = m_backBufferFormat;
	sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	sd.SampleDesc.Count = m_msaaState ? 4 : 1;
	sd.SampleDesc.Quality = m_msaaState ? (m_msaaQuality - 1) : 0;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.BufferCount = s_swapChainBufferCount;
	sd.OutputWindow = App::GetApp()->GetMainWindow();
	sd.Windowed = true;
	sd.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	ThrowIfFailed(m_factory->CreateSwapChain(m_commandQueue, &sd, &m_swapChain));
}


//#ifdef _DEBUG
//
//void Renderer::InitDebugMode(UINT factoryFlags, ID3D12Device4* device)
//{
//	ID3D12Debug1* debugController;
//	ID3D12Debug* dc;
//	ThrowIfFailed(D3D12GetDebugInterface(IID_PPV_ARGS(&dc)));
//	ThrowIfFailed(dc->QueryInterface(IID_PPV_ARGS(&debugController)));
//	debugController->EnableDebugLayer();
//	debugController->SetEnableGPUBasedValidation(true);
//
//	factoryFlags |= DXGI_CREATE_FACTORY_DEBUG;
//
//	dc->Release();
//	dc = nullptr;
//
//	ID3D12DebugDevice* debugDevice;
//	ThrowIfFailed(device->QueryInterface(&debugDevice));
//}
//
//#endif // _DEBUG