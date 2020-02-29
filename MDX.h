#pragma once
#include <windows.h>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <iterator>
#include <exception>
#include "DXInterOp.h"
#include "ComputeEngine.h"
class MDX {
	HWND                        g_hWnd = NULL;
	D3D_DRIVER_TYPE             g_driverType = D3D_DRIVER_TYPE_NULL;
	D3D_FEATURE_LEVEL           g_featureLevel = D3D_FEATURE_LEVEL_11_0;
	ID3D11Device* g_pd3dDevice = NULL;
	ID3D11DeviceContext* g_pImmediateContext = NULL;
	IDXGISwapChain* g_pSwapChain = NULL;
	ID3D11RenderTargetView* g_pRenderTargetView = NULL;
	ID3D11ComputeShader* g_pComputeShader = NULL;
	ID3D11VertexShader* g_pVertexShader = NULL;
	ID3D11PixelShader* g_pPixelShader = NULL;
	ID3D11InputLayout* g_pVertexLayout = NULL;
	ID3D11Buffer* g_pVertexBuffer = NULL;
	ID3D11Buffer* g_pVertexPosBuffer = NULL;
	ID3D11ShaderResourceView* g_pVertexPosBufferRV = NULL;
	ID3D11UnorderedAccessView* g_pVertexPosBufferUAV = NULL;
	AMP_compute_engine* g_pAMPComputeEngine = NULL;
public:
	static const unsigned int          g_numVertices = 3;
	//--------------------------------------------------------------------------------------
	HRESULT InitDevice(HWND ghWnd) {// Create Direct3D device and shaders. Call from wWinMain()
        g_hWnd = ghWnd;
		HRESULT hr = S_OK;
		RETURN_IF_FAIL(CreateSwapChain());
		RETURN_IF_FAIL(CreateComputeShader());
		RETURN_IF_FAIL(CreateVertexShader());
		RETURN_IF_FAIL(CreatePixelShader());
		return hr;
	} // //////////////////////////////////////////////////////////////////////////////////
private:
    HRESULT CreateSwapChain() {
        HRESULT hr = S_OK;
        RECT rc;
        GetClientRect(g_hWnd, &rc);
        UINT width = rc.right - rc.left;
        UINT height = rc.bottom - rc.top;
        UINT createDeviceFlags = 0;
#ifdef _DEBUG
        createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif
        D3D_DRIVER_TYPE driverTypes[] =
        {   D3D_DRIVER_TYPE_HARDWARE,
            D3D_DRIVER_TYPE_WARP,
            D3D_DRIVER_TYPE_REFERENCE,
        };
        UINT numDriverTypes = ARRAYSIZE(driverTypes);
        D3D_FEATURE_LEVEL featureLevels[] = { D3D_FEATURE_LEVEL_11_0 };
        UINT numFeatureLevels = ARRAYSIZE(featureLevels);

        // Create swap chain
        DXGI_SWAP_CHAIN_DESC sd;
        ZeroMemory(&sd, sizeof(sd));
        sd.BufferCount = 1;
        sd.BufferDesc.Width = width;
        sd.BufferDesc.Height = height;
        sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        sd.BufferDesc.RefreshRate.Numerator = 60;
        sd.BufferDesc.RefreshRate.Denominator = 1;
        sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        sd.OutputWindow = g_hWnd;
        sd.SampleDesc.Count = 1;
        sd.SampleDesc.Quality = 0;
        sd.Windowed = TRUE;

        for (UINT driverTypeIndex = 0; driverTypeIndex < numDriverTypes; driverTypeIndex++) {
            g_driverType = driverTypes[driverTypeIndex];
            hr = D3D11CreateDeviceAndSwapChain(NULL, g_driverType, NULL, createDeviceFlags, featureLevels, numFeatureLevels,
                D3D11_SDK_VERSION, &sd, &g_pSwapChain, &g_pd3dDevice, &g_featureLevel, &g_pImmediateContext);
            if (SUCCEEDED(hr))
                break;
        }
        RETURN_IF_FAIL(hr);

        // Create a render target view
        ID3D11Texture2D* pBackBuffer = NULL;
        RETURN_IF_FAIL(g_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer));

        hr = g_pd3dDevice->CreateRenderTargetView(pBackBuffer, NULL, &g_pRenderTargetView);
        SAFE_RELEASE(pBackBuffer);
        RETURN_IF_FAIL(hr);

        g_pImmediateContext->OMSetRenderTargets(1, &g_pRenderTargetView, NULL);

        // Setup the viewport
        D3D11_VIEWPORT vp;
        vp.Width = (FLOAT)width;
        vp.Height = (FLOAT)height;
        vp.MinDepth = 0.0f;
        vp.MaxDepth = 1.0f;
        vp.TopLeftX = 0;
        vp.TopLeftY = 0;
        g_pImmediateContext->RSSetViewports(1, &vp);
        return hr;
    } // ///////////////////////////////////////////////////////////////////////////////////////////
    HRESULT CreateComputeShader() {
        // A triangle 
        std::vector<Vertex2D> vertices(3);
        vertices[0].Pos = DirectX::XMFLOAT2(-0.25f, 0.0f);
        vertices[1].Pos = DirectX::XMFLOAT2(0.0f, -0.5f);
        vertices[2].Pos = DirectX::XMFLOAT2(-0.5f, -0.5f);
        //Vertex2D vertices[g_numVertices] =
        //{   DirectX::XMFLOAT2(-0.25f, 0.0f),
        //    DirectX::XMFLOAT2(0.0f, -0.5f),
        //    DirectX::XMFLOAT2(-0.5f, -0.5f),
        //};
        g_pAMPComputeEngine = new AMP_compute_engine(g_pd3dDevice);
        g_pAMPComputeEngine->initialize_data(vertices);
        RETURN_IF_FAIL(g_pAMPComputeEngine->get_data_d3dbuffer(reinterpret_cast<void**>(&g_pVertexPosBuffer)));

        // Bind a resource view to the CS buffer
        D3D11_BUFFER_DESC descBuf;
        ZeroMemory(&descBuf, sizeof(descBuf));
        g_pVertexPosBuffer->GetDesc(&descBuf);

        D3D11_SHADER_RESOURCE_VIEW_DESC DescRV;
        ZeroMemory(&DescRV, sizeof(DescRV));
        DescRV.Format = DXGI_FORMAT_R32_TYPELESS;
        DescRV.ViewDimension = D3D11_SRV_DIMENSION_BUFFEREX;
        DescRV.BufferEx.Flags = D3D11_BUFFEREX_SRV_FLAG_RAW;
        DescRV.Buffer.FirstElement = 0;
        DescRV.Buffer.NumElements = descBuf.ByteWidth / sizeof(int);
        RETURN_IF_FAIL(g_pd3dDevice->CreateShaderResourceView(g_pVertexPosBuffer, &DescRV, &g_pVertexPosBufferRV));
        return S_OK;
    } // /////////////////////////////////////////////////////////////////////////////////////////////
    HRESULT CreateVertexShader() {
        HRESULT hr = S_OK;
        ID3DBlob* pVSBlob = NULL;
        LPCSTR pProfile = (g_pd3dDevice->GetFeatureLevel() >= D3D_FEATURE_LEVEL_11_0) ? "vs_5_0" : "vs_4_0";

        hr = CompileShaderFromFile(L"DXInterOpPsVs.hlsl", "VS", pProfile, &pVSBlob);
        if (FAILED(hr)) {
            MessageBox(NULL, L"The vertex shader in DXInterOpPsVs.hlsl cannot be compiled", L"Error", MB_OK);
            return hr;
        }
        // Create the vertex shader
        hr = g_pd3dDevice->CreateVertexShader(pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), NULL, &g_pVertexShader);
        if (FAILED(hr)) {
            pVSBlob->Release();
            return hr;
        }
        // Define the input layout
        D3D11_INPUT_ELEMENT_DESC layout[] =
        {
            { "POSITION", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        };
        UINT numElements = ARRAYSIZE(layout);

        // Create the input layout
        hr = g_pd3dDevice->CreateInputLayout(layout, numElements, pVSBlob->GetBufferPointer(),
            pVSBlob->GetBufferSize(), &g_pVertexLayout);
        pVSBlob->Release();
        RETURN_IF_FAIL(hr);

        // Set the input layout
        g_pImmediateContext->IASetInputLayout(g_pVertexLayout);

        D3D11_BUFFER_DESC bd;
        ZeroMemory(&bd, sizeof(bd));
        bd.Usage = D3D11_USAGE_DEFAULT;
        bd.ByteWidth = sizeof(Vertex2D) * g_numVertices;
        bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
        bd.CPUAccessFlags = 0;
        RETURN_IF_FAIL(g_pd3dDevice->CreateBuffer(&bd, NULL, &g_pVertexBuffer));

        // Set vertex buffer
        UINT stride = sizeof(Vertex2D);
        UINT offset = 0;
        g_pImmediateContext->IASetVertexBuffers(0, 1, &g_pVertexBuffer, &stride, &offset);

        // Set primitive topology
        g_pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
        return hr;
    } // ///////////////////////////////////////////////////////////////////////////////////////////
    HRESULT CompileShaderFromFile(LPCWSTR szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut) {
        HRESULT hr = S_OK;
        DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS | D3DCOMPILE_OPTIMIZATION_LEVEL2;
#if defined( DEBUG ) || defined( _DEBUG )
        // Set the D3DCOMPILE_DEBUG flag to embed debug information in the shaders.
        // Setting this flag improves the shader debugging experience, but still allows 
        // the shaders to be optimized and to run exactly the way they will run in 
        // the release configuration of this program.
        dwShaderFlags |= D3DCOMPILE_DEBUG;
#endif
        // Read shader file to string buffer
        std::ifstream shaderFileStream(szFileName);
        std::stringstream ssbuff;
        ssbuff << shaderFileStream.rdbuf();
        shaderFileStream.close();
        std::string hlslProgram = ssbuff.str();

        ID3DBlob* pErrorBlob;
        hr = D3DCompile(hlslProgram.c_str(), hlslProgram.size(), NULL, NULL, NULL,
            szEntryPoint, szShaderModel, dwShaderFlags, 0, ppBlobOut, &pErrorBlob);
        if (FAILED(hr)) {
            if (pErrorBlob != NULL) {
                OutputDebugStringA((char*)pErrorBlob->GetBufferPointer());
            }
        }
        SAFE_RELEASE(pErrorBlob);
        return hr;
    } // ///////////////////////////////////////////////////////////////////////////////////////////
    HRESULT CreatePixelShader() {
        HRESULT hr = S_OK;
        // Compile the pixel shader
        ID3DBlob* pPSBlob = NULL;
        LPCSTR pProfile = (g_pd3dDevice->GetFeatureLevel() >= D3D_FEATURE_LEVEL_11_0) ? "ps_5_0" : "ps_4_0";

        hr = CompileShaderFromFile(L"DXInterOpPsVs.hlsl", "PS", pProfile, &pPSBlob);
        if (FAILED(hr)) {
            MessageBox(NULL, L"The pixel shader in DXInterOpPsVs.hlsl cannot be compiled.", L"Error", MB_OK);
            return hr;
        }
        // Create the pixel shader
        hr = g_pd3dDevice->CreatePixelShader(pPSBlob->GetBufferPointer(), pPSBlob->GetBufferSize(), NULL, &g_pPixelShader);
        pPSBlob->Release();
        return hr;
    } // ///////////////////////////////////////////////////////////////////////////////////////////////
public:
    void ComputeRenderData() {     //  Call from main loop wWinMain()
        g_pAMPComputeEngine->run();    // TODO: merge with Render()?
    } // ///////////////////////////////////////////////////////////////////////////////////////////////
    void Render() {               //  Call from main loop wWinMain()
        g_pAMPComputeEngine->run();    //!!! TODO: merge with Render()?
        // Bind the vertex shader data though the compute shader result buffer view
        UINT stride = sizeof(Vertex2D);
        UINT offset = 0;
        g_pImmediateContext->IASetVertexBuffers(0, 1, &g_pVertexBuffer, &stride, &offset);
        g_pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

        ID3D11ShaderResourceView* aRViews[1] = { g_pVertexPosBufferRV };
        g_pImmediateContext->VSSetShaderResources(0, 1, aRViews);

        // Clear the back buffer 
        float ClearColor[4] = { 0.0f, 0.125f, 0.3f, 1.0f }; // red,green,blue,alpha
        g_pImmediateContext->ClearRenderTargetView(g_pRenderTargetView, ClearColor);

        // Render the triangle
        g_pImmediateContext->VSSetShader(g_pVertexShader, NULL, 0);
        g_pImmediateContext->PSSetShader(g_pPixelShader, NULL, 0);
        g_pImmediateContext->Draw(g_numVertices, 0);

        ID3D11ShaderResourceView* ppSRVNULL[1] = { NULL };
        g_pImmediateContext->VSSetShaderResources(0, 1, ppSRVNULL);

        // Present the information rendered to the back buffer to the front buffer (the screen)
        g_pSwapChain->Present(0, 0);
    } // ///////////////////////////////////////////////////////////////////////////////////////////////////
    void CleanupDevice() {     //  Call from wWinMain() twice: onExit & onError
        if (g_pImmediateContext) g_pImmediateContext->ClearState();
        SAFE_RELEASE(g_pVertexPosBufferUAV);
        SAFE_RELEASE(g_pVertexPosBufferRV);
        SAFE_RELEASE(g_pVertexPosBuffer);
        SAFE_RELEASE(g_pVertexBuffer);
        SAFE_RELEASE(g_pVertexLayout);
        SAFE_RELEASE(g_pVertexShader);
        SAFE_RELEASE(g_pPixelShader);
        SAFE_DELETE(g_pAMPComputeEngine);
        SAFE_RELEASE(g_pRenderTargetView);
        SAFE_RELEASE(g_pSwapChain);
        SAFE_RELEASE(g_pImmediateContext);
        SAFE_RELEASE(g_pd3dDevice);
    } // //////////////////////////////////////////////////////////////////////////////////////////
}; // ***********************************************************************

