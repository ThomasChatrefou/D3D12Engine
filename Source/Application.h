class Renderer;

class App
{
public:
	static App* GetApp() { return m_app; }

	App();
	~App();

	bool Initialize();
	int Run();

	LRESULT ProcessMessage(HWND Hwnd, UINT message, WPARAM wparam, LPARAM lparam);

	HWND GetMainWindow() { return m_mainWindow; }
	int GetWindowWidth() { return m_windowWidth; }
	int GetWindowHeight() { return m_windowHeight; }

private:
	static App* m_app;

	bool InitWindow();
	void InitializeVariables();
	bool CreateWindowClass();
	bool InitializeWindow();

	void Update();
	void Draw();

	HWND m_mainWindow;

	WCHAR m_windowClass[MAX_NAME_STRING];
	WCHAR m_windowTitle[MAX_NAME_STRING];

	int m_windowWidth = 1366;
	int m_windowHeight = 768;

	HICON m_hIcon;

	Renderer* m_renderer;
};
