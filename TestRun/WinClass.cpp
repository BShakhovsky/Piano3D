#include "stdafx.h"
#include "WinClass.h"
#include "WinMessages.h"
#include "Render.h"
#include "DxError.h"
#include "vld.h"

using namespace std;

HINSTANCE WinClass::hInstance = nullptr;
HWND WinClass::hWnd = nullptr;
shared_ptr<Render> WinClass::render = nullptr;

#pragma warning(push)
#pragma warning(disable: 4711) // Automatic inline expansion
ATOM WinClass::MyRegisterClass() const
{
	WNDCLASSEXW wcex{ sizeof wcex };
	wcex.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
	wcex.lpfnWndProc = WinMessages::Main;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_PIANO3D));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = reinterpret_cast<HBRUSH>(COLOR_WINDOW + 1);
	wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_PIANO3D);
	wcex.lpszClassName = szWindowClass_;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));
	return RegisterClassExW(&wcex);
}
#pragma warning(pop)

int WinClass::Main(const int nCmdShow) const
{
	if (!InitInstance(nCmdShow)) return FALSE;

	//////////////////////////////////////////////////////////////////////////////////////////////////
	if (!SystemParametersInfo(SPI_SETMENUANIMATION, 0, nullptr, 0))
		MessageBox(hWnd, TEXT("Could not set menu animation"), TEXT("Info"), MB_OK | MB_ICONASTERISK);
	//////////////////////////////////////////////////////////////////////////////////////////////////

	TCHAR path[MAX_PATH];
	GetCurrentDirectory(ARRAYSIZE(path), path);
	try
	{
		render = make_shared<Render>(hWnd, WinMessages::width, WinMessages::height, path);
	}
	catch (const DxError& e)
	{
		MessageBox(hWnd, e.RusWhat(), TEXT("DirectX Error"), MB_OK | MB_ICONHAND);
	}

	const auto hAccelTable(LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_PIANO3D)));
	MSG msg{ 0 };

#ifdef _DEBUG
	const auto before(VLDGetLeaksCount());
#endif

	while (msg.message != WM_QUIT)
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE) && !TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else try
		{
			render->Draw();
		}
		catch (const DxError& e)
		{
			MessageBox(hWnd, e.RusWhat(), TEXT("DirectX Error"), MB_OK | MB_ICONHAND);
		}

#ifdef _DEBUG
		const auto after(VLDGetLeaksCount());
#endif
	assert("Memory leaks detected" && before == after || before + 3 == after);

	return static_cast<int>(msg.wParam);
}