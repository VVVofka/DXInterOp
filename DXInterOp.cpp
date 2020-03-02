//--------------------------------------------------------------------------------------
// File: InterOp.cpp
// https://hackmag.com/coding/computing-with-gpu/
// This application demonstrates the usage of AMP C++ interOp APIs with Direct3D 11. 
// It displays the animation of a triangle rotation. 
//
// Copyright (c) Microsoft Corporation. All rights reserved.
//--------------------------------------------------------------------------------------
#include <windows.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <iterator>
#include <exception>
//#include "DXUT.h"
#include "DXInterOp.h"
#include "MDX3.h"
#include "MDX.h"
//--------------------------------------------------------------------------------------
// Global Variables
HINSTANCE                   g_hInst = NULL;
HWND                        g_hWnd = NULL;
// Forward declarations
HRESULT             InitWindow(HINSTANCE hInstance, int nCmdShow);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
MDX3 mdx;
//--------------------------------------------------------------------------------------
// Entry point to the program. Initializes everything and goes into a message processing 
// loop. Idle time is used to render the scene.
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow) {
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);
	RETURN_IF_FAIL(InitWindow(hInstance, nCmdShow));

	std::vector<Vertex3D> vertices(3);
	vertices[0].Pos = DirectX::XMFLOAT3(-0.25f, 0.0f, 0.0f);
	vertices[1].Pos = DirectX::XMFLOAT3(0.0f, -0.5f, -0.3f);
	vertices[2].Pos = DirectX::XMFLOAT3(-0.5f, -0.5f, 0.3f);

	if (FAILED(mdx.InitDevice(g_hWnd, vertices))) {
		mdx.CleanupDevice();
		return E_FAIL;
	}
	// Main message loop
	MSG msg = { 0 };
	while (WM_QUIT != msg.message) {
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else {
			mdx.Render();
			
		}
	}
	mdx.CleanupDevice();
	//return DXUTGetExitCode();
	return (int)msg.wParam;
} // ////////////////////////////////////////////////////////////////////////////
HRESULT InitWindow(HINSTANCE hInstance, int nCmdShow) {  // Register class and create window
	// Register class
	WNDCLASSEX wcex;
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = L"AMPC++WindowClass";
	wcex.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	if (!RegisterClassEx(&wcex))  return E_FAIL;

	// Create window
	g_hInst = hInstance;
	RECT rc = { 0, 0, 640, 480 };
	AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);
	g_hWnd = CreateWindow(L"AMPC++WindowClass", L"AMPC++ and Direct3D 11 InterOp Sample",
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, rc.right - rc.left, rc.bottom - rc.top, NULL, NULL, hInstance,
		NULL);
	if (!g_hWnd)  
		return E_FAIL;
	ShowWindow(g_hWnd, nCmdShow);
	return S_OK;
} // //////////////////////////////////////////////////////////////////////////////////////////////
// Called every time the application receives a message
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	PAINTSTRUCT ps;
	HDC hdc;
	switch (message) {
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_KEYDOWN:
	{
		//switch (wParam) {
		//case VK_ESCAPE:
		//{
		//	if (GetDXUTState().GetHandleEscape())
		//		SendMessage(hWnd, WM_CLOSE, 0, 0);
		//	break;
		//}

		//case VK_PAUSE:
		//{
		//	if (GetDXUTState().GetHandlePause()) {
		//		bool bTimePaused = DXUTIsTimePaused();
		//		bTimePaused = !bTimePaused;
		//		if (bTimePaused)
		//			DXUTPause(true, false);
		//		else
		//			DXUTPause(false, false);
		//	}
		//	break;
		//}
		//}
		break;
	}
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
} // ////////////////////////////////////////////////////////////////////////////////////
