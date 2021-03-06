#include "stdafx.h"
#include "WinMessages.h"
#include "WinClass.h"
#include "Render.h"
#include "DxError.h"

using namespace std;

UINT WinMessages::width = 0, WinMessages::height = 0;

INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM)
{
	switch (message)
	{
	case WM_INITDIALOG:	return TRUE;
	case WM_COMMAND:	if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
	{
		EndDialog(hDlg, LOWORD(wParam));
		return TRUE;
	}
	}
	return FALSE;
}

void CALLBACK OnTimer(const HWND, UINT, UINT_PTR, const DWORD dwTime)
{
	static const auto start(dwTime);

	if (WinClass::render)
	{
		if (!((dwTime - start) / 200 % 10)) WinClass::render->ReleaseAllKeys();
		WinClass::render->PressKey(rand() % 88);
		WinClass::render->AssignFingerNums(rand() % 88,
			string(static_cast<size_t>(rand() % 3), static_cast<char>('0' + rand() % 6)).c_str(),
			rand() % 2 == 0);
	}
}

inline BOOL WinMessages::OnCreate(const HWND hWnd, LPCREATESTRUCT)
{
	srand(static_cast<unsigned>(time(nullptr)));
	// Potentially throwing function passed to extern C function
	// Undefined behavior may occur if this function throws
#pragma warning(suppress:5039)
	SetTimer(hWnd, 0, 200, OnTimer);

	return true;
}

void WinMessages::OnSize(const HWND hWnd, const UINT, const int cx, const int cy)
{
	width = static_cast<UINT>(cx);
	height = static_cast<UINT>(cy);

	if (WinClass::render) try
	{
		WinClass::render->Resize(width, height);
	}
	catch (const DxError& e)
	{
		MessageBox(hWnd, e.RusWhat(), TEXT("DirectX Error"), MB_OK | MB_ICONHAND);
	}
}
inline void OnCommand(HWND hWnd, int id, HWND, UINT)
{
	switch (id)
	{
		// Potentially throwing function passed to extern C function
		// Undefined behavior may occur if this function throws
#pragma warning(suppress:5039)
	case IDM_ABOUT:	DialogBox(WinClass::hInstance, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About); break;
	case IDM_EXIT:	DestroyWindow(hWnd);
	}
}
inline void OnDestroy(HWND)
{
	PostQuitMessage(0);
}

void OnMouseWheel(HWND hWnd, int, int, int delta, UINT)
{
	if (WinClass::render) try
	{
		for (auto i(0); i < abs(delta) / WHEEL_DELTA * 5; ++i)
		{
			if (!WinClass::render->Zoom(delta > 0)) break;
			WinClass::render->Draw();
		}
	}
	catch (const DxError& e)
	{
		MessageBox(hWnd, e.RusWhat(), TEXT("DirectX Error"), MB_OK | MB_ICONHAND);
	}
}
void OnMButtonDblClick(HWND hWnd, BOOL, int, int, UINT)
{
	if (WinClass::render) try
	{
		while (!WinClass::render->FitToWindow()) WinClass::render->Draw();
	}
	catch (const DxError& e)
	{
		MessageBox(hWnd, e.RusWhat(), TEXT("DirectX Error"), MB_OK | MB_ICONHAND);
	}
}

float Divide(int divisible, UINT divider) { return static_cast<float>(divisible) / static_cast<float>(divider); }
#define UNIT_COORD Divide(x, width), Divide(y, height)

#pragma warning(push)
#pragma warning(disable:5045) // Compiler will insert Spectre mitigation for memory load if /Qspectre switch specified
void WinMessages::OnMButtonDown(const HWND hWnd, const BOOL, const int x, const int y, const UINT)
{
	if (WinClass::render) try
	{
		const auto unitX(Divide(x, width)), unitY(Divide(y, height));
		// it is screen coordinates when right-click --> context menu is shown:
		if (unitX >= 0 && unitX <= 1 && unitY >= 0 && unitY <= 1) WinClass::render->MoveStart(unitX, unitY);
	}
	catch (const DxError& e)
	{
		MessageBox(hWnd, e.RusWhat(), TEXT("DirectX Error"), MB_OK | MB_ICONHAND);
	}
}
void WinMessages::OnLButtonDown(const HWND hWnd, const BOOL, const int x, const int y, const UINT)
{
	if (WinClass::render) try
	{
		WinClass::render->RotateStart(UNIT_COORD);
	}
	catch (const DxError& e)
	{
		MessageBox(hWnd, e.RusWhat(), TEXT("DirectX Error"), MB_OK | MB_ICONHAND);
	}
}
void WinMessages::OnMouseMove(const HWND hWnd, const int x, const int y, const UINT keyFlags)
{
	if (WinClass::render) try
	{
		if (keyFlags & MK_MBUTTON) WinClass::render->MoveEnd(UNIT_COORD);
		if (keyFlags & MK_LBUTTON) WinClass::render->RotateEnd(UNIT_COORD);
	}
	catch (const DxError& e)
	{
		MessageBox(hWnd, e.RusWhat(), TEXT("DirectX Error"), MB_OK | MB_ICONHAND);
	}
}
#pragma warning(pop)

void OnContextMenu(HWND hWnd, HWND, int xPos, int yPos)
{
	if (TrackPopupMenu(GetSubMenu(LoadMenu(WinClass::hInstance,
		MAKEINTRESOURCE(IDR_CONTEXT_MENU)), 0),
		TPM_CENTERALIGN | TPM_VCENTERALIGN | TPM_RETURNCMD,// | TPM_HORPOSANIMATION | TPM_VERPOSANIMATION,
		xPos, yPos, 0, hWnd, nullptr)
		== IDM_DEFAULT_POS && WinClass::render) try
	{
		while (!WinClass::render->Restore3DPosition()) WinClass::render->Draw();
	}
	catch (const DxError& e)
	{
		MessageBox(hWnd, e.RusWhat(), TEXT("DirectX Error"), MB_OK | MB_ICONHAND);
	}
}

LRESULT CALLBACK WinMessages::Main(const HWND hWnd, const UINT message, const WPARAM wParam, const LPARAM lParam)
{
	switch (message)
	{
		HANDLE_MSG(hWnd, WM_CREATE, OnCreate);
		HANDLE_MSG(hWnd, WM_SIZE, OnSize);
		HANDLE_MSG(hWnd, WM_COMMAND, OnCommand);
		HANDLE_MSG(hWnd, WM_DESTROY, OnDestroy);

		HANDLE_MSG(hWnd, WM_MOUSEWHEEL, OnMouseWheel);
		HANDLE_MSG(hWnd, WM_MBUTTONDBLCLK, OnMButtonDblClick);

		HANDLE_MSG(hWnd, WM_MBUTTONDOWN, OnMButtonDown);
		HANDLE_MSG(hWnd, WM_LBUTTONDOWN, OnLButtonDown);
		HANDLE_MSG(hWnd, WM_MOUSEMOVE, OnMouseMove);

		HANDLE_MSG(hWnd, WM_CONTEXTMENU, OnContextMenu);

	default: return DefWindowProc(hWnd, message, wParam, lParam);
	}
}