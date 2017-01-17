#pragma once

class WinMessages abstract
{
public:
	static UINT width, height;
	static LRESULT CALLBACK Main(HWND, UINT, WPARAM, LPARAM);
private:
	static BOOL OnCreate(HWND, LPCREATESTRUCT);
	static void OnSize(HWND, UINT, int, int);

	static void OnMButtonDown(HWND, BOOL, int, int, UINT);
	static void OnLButtonDown(HWND, BOOL, int, int, UINT);
	static void OnMouseMove(HWND, int, int, UINT);
};