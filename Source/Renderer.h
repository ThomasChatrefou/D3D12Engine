class Renderer
{
public:
	Renderer();
	~Renderer();

	bool Initialize();

private:
	bool InitializeDirectX();

#pragma region Initialization Methods

	// A Factory defines entry point to the DX API
	void CreateFactory();

	// An Adapter provides information on the physical properties of a given DirectX device
	void SelectAdapter();

	// A Device is your primary entry point to the DirectX 12 API
	void CreateDevice();

	// A Command Queue allows you to submit groups of draw calls, known as command lists
	void CreateCommandQueue();

	// A Command Allocator allows you to create command lists where you can define the functions you want the GPU to execute.
	void CreateCommandAllocator();

	void CreateCommandList();

	// A Fence lets your program know when certain tasks have been executed by the GPU
	void CreateFence();

	// A Barrier lets the driver know how a resource should be used in upcoming commands
	void CreateBarrier();

	void CacheDescriptorsSize();
	void CheckMsaaQualitySupport();
	void CreateSwapChain();


#pragma endregion Initialization Methods

	ID3D12Device4* m_device;
	ID3D12CommandQueue* m_commandQueue;
	ID3D12CommandAllocator* m_commandAllocator;
	ID3D12GraphicsCommandList* m_commandList;
	ID3D12Fence* m_fence;

	D3D12_RESOURCE_BARRIER m_barrier;
	
	IDXGIFactory4* m_factory;
	IDXGIAdapter1* m_adapter;
	IDXGISwapChain* m_swapChain;
	
	static const int s_swapChainBufferCount = 2;
	ID3D12Resource* m_swapChainBuffer[s_swapChainBufferCount];
	ID3D12Resource* m_depthStencilBuffer;
	int m_currentBackBuffer;

	UINT m_rtvDescriptorSize;
	UINT m_dsvDescriptorSize;
	UINT m_cbvSrvUavDescriptorSize;

	// Sync
	UINT m_FrameIndex;
	HANDLE m_FenceEvent;
	ID3D12Fence* m_Fence;
	UINT64 m_FenceValue;

	DXGI_FORMAT m_backBufferFormat = DXGI_FORMAT_R8G8B8A8_UNORM;
	UINT m_msaaQuality = 0;
	bool m_msaaState = false;

//#ifdef _DEBUG
//	void InitDebugMode(UINT factoryFlags, ID3D12Device4* device);
//#endif
};