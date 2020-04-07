//--------------------------------------------------------------------------------------
// File: InterOp.cpp
// https://hackmag.com/coding/computing-with-gpu/
// This application demonstrates the usage of AMP C++ interOp APIs with Direct3D 11. 
// It displays the animation of a triangle rotation. 
//
// Copyright (c) Microsoft Corporation. All rights reserved.
//--------------------------------------------------------------------------------------
//#include <windows.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <iterator>
#include <exception>
//#include "DXUT.h"
#include "DXInterOp.h"
#include "Masks.h"
#ifdef MY2D
#include "MDX2.h"
#include "Model2D.h"
#elif MY3D // MY2D
#include "MDX3.h"
#else // MY2D
#include "MDX2.h"
#include "Model2D.h"
#include "Utils.h"
#endif // MY2D

//--------------------------------------------------------------------------------------
// Global Variables
HINSTANCE                   g_hInst = NULL;
HWND                        g_hWnd = NULL;
// Forward declarations
HRESULT             InitWindow(HINSTANCE hInstance, int nCmdShow);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
#ifdef MY2D
MDX2 mdx;
Model2D model;
std::vector<Vertex2D> vertices(3);
#elif MY3D // MY2D
MDX3 mdx;
std::vector<Vertex3D> vertices(3);
#else // MY2D
MDX2 mdx;
Model2D model = Model2D();
#endif // MY2D
//--------------------------------------------------------------------------------------
// Entry point to the program. Initializes everything and goes into a message processing 
// loop. Idle time is used to render the scene.
#pragma warning(suppress : 28251)
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow){
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);
	RETURN_IF_FAIL(InitWindow(hInstance, nCmdShow));

#ifdef MY2D
	vertices[0].Pos = DirectX::XMFLOAT2(-0.25f, 0.0f);
	vertices[1].Pos = DirectX::XMFLOAT2(0.0f, -0.5f);
	vertices[2].Pos = DirectX::XMFLOAT2(-0.5f, -0.5f);
	if(FAILED(mdx.InitDevice(g_hWnd, vertices))){
		mdx.CleanupDevice();
		return E_FAIL;
	}
#elif MY3D // MY2D
	vertices[0].Pos = DirectX::XMFLOAT3(-0.25f, 0.0f, 0.0f);
	vertices[1].Pos = DirectX::XMFLOAT3(0.0f, -0.5f, -0.3f);
	vertices[2].Pos = DirectX::XMFLOAT3(-0.5f, -0.5f, 0.3f);
	if(FAILED(mdx.InitDevice(g_hWnd, vertices))){
		mdx.CleanupDevice();
		return E_FAIL;
	}
#else 
	INT2 szlay0 = INT2(1, 1);
	//model.Create(szlay0, 1024 * 1, 0.06); //8 - ???
	model.Create(szlay0, 1024 * 2, 0.015, 0.15, 0.15); //8 - ???
	if(FAILED(mdx.InitDevice(g_hWnd, model.lastPoss()))){
		mdx.CleanupDevice();
		return E_FAIL;
	}
#endif // MY2D

	//Blocks2D2 blks;

	//blks.dumpx();	_RPT0(0, "\n Y \n");	blks.dumpy();

	// Main message loop
	MSG msg = {0};
	while(WM_QUIT != msg.message){
		if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)){
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		} else{
			mdx.Render();
		}
	}
	mdx.CleanupDevice();
	//return DXUTGetExitCode();
	return (int)msg.wParam;
} // ////////////////////////////////////////////////////////////////////////////
HRESULT InitWindow(HINSTANCE hInstance, int nCmdShow){  // Register class and create window
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
	if(!RegisterClassEx(&wcex))  return E_FAIL;

	// Create window
	g_hInst = hInstance;
	RECT rc = {0, 0, 900, 900};
	AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);
	g_hWnd = CreateWindow(L"AMPC++WindowClass", L"AMPC++ and Direct3D 11 InterOp Sample",
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, rc.right - rc.left, rc.bottom - rc.top, NULL, NULL, hInstance,
		NULL);
	if(!g_hWnd)
		return E_FAIL;
	ShowWindow(g_hWnd, nCmdShow);
	return S_OK;
} // //////////////////////////////////////////////////////////////////////////////////////////////
// Called every time the application receives a message
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam){
	PAINTSTRUCT ps;
	HDC hdc;
	switch(message){
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
