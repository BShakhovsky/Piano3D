#include "stdafx.h"
#include "WinMessages.h"
#include "WinClass.h"
#include "CanvasGdi.h"
#include "FontGdi.h"
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
inline void OnPaint(HWND hWnd)
{
	using boost::wformat;

	static auto startTime(GetTickCount() - 1), frames(0ul), fps(0ul);
	++frames;

	if (WinClass::render)
	{
		const auto deltaTime(GetTickCount() - startTime);
		if (deltaTime > 1'000)
		{
			fps = frames * 1'000 / deltaTime;
			startTime += 1'000;
			frames = 0;
		}

		CanvasGdi hDC(hWnd);

		const auto text((wformat{ TEXT("FPS = %1%\nX = %2%\nY = %3%\nZ = %4%") }
			% fps % WinClass::render->GetPosition().x % WinClass::render->GetPosition().y
			% WinClass::render->GetPosition().z).str());

		RECT rect;
		GetClientRect(hWnd, &rect);
		rect.top += 5;
		rect.right -= 10;

		LOGFONT logFont{ 32 };
		FontGdi font(hDC, &logFont, fps > 55 ? RGB(0, 0xFF, 0) : RGB(0xFF, 0, 0), TRANSPARENT);
		DrawText(hDC, text.c_str(), static_cast<int>(text.length()), &rect, DT_RIGHT);
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
		HANDLE_MSG(hWnd, WM_PAINT, OnPaint);
		HANDLE_MSG(hWnd, WM_DESTROY, OnDestroy);
	default: return DefWindowProc(hWnd, message, wParam, lParam);
	}
}