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

void WinMessages::OnSize(const HWND hWnd, const UINT, const int cx, const int cy)
{
	width = static_cast<UINT>(cx);
	height = static_cast<UINT>(cy);

	if (WinClass::render) try
	{
		WinClass::render->Resize(width, height);
		WinClass::render->Draw();
	}
	catch (const DxError& e)
	{
		MessageBoxA(hWnd, e.what(), "DirectX Error", MB_OK | MB_ICONHAND);
	}
}
void OnMove(HWND hWnd, int, int)
{
	if (WinClass::render) try
	{
		WinClass::render->Draw();
	}
	catch (const DxError& e)
	{
		MessageBoxA(hWnd, e.what(), "DirectX Error", MB_OK | MB_ICONHAND);
	}
}
inline void OnCommand(HWND hWnd, int id, HWND, UINT)
{
	switch (id)
	{
	case IDM_ABOUT:	DialogBox(WinClass::hInstance, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About); break;
	case IDM_EXIT:	DestroyWindow(hWnd);
	}
}
inline void OnDestroy(HWND)
{
	PostQuitMessage(0);
}

LRESULT CALLBACK WinMessages::Main(const HWND hWnd, const UINT message, const WPARAM wParam, const LPARAM lParam)
{
	switch (message)
	{
		HANDLE_MSG(hWnd, WM_SIZE, OnSize);
		HANDLE_MSG(hWnd, WM_MOVE, OnMove);
		HANDLE_MSG(hWnd, WM_COMMAND, OnCommand);
		HANDLE_MSG(hWnd, WM_DESTROY, OnDestroy);
	default: return DefWindowProc(hWnd, message, wParam, lParam);
	}
}