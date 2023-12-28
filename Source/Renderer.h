class Renderer
{
public:
	Renderer();
	~Renderer();

	bool Initialize();

private:
	bool InitializeDirectX();
	void SelectAdapter(IDXGIAdapter1* adapter, IDXGIFactory4* factory);

#ifdef _DEBUG
	void InitDebugMode(UINT factoryFlags, ID3D12Device* device);
#endif
};