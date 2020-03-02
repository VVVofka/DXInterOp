#pragma once
#include "MDX.h"
#include "AMPEngine3.h"
class MDX3 : public MDX{
    AMPEngine3* g_pAMPComputeEngine = NULL;
public:
    HRESULT InitDevice(HWND ghWnd, 
                       std::vector<Vertex3D>& vertices,
                       D3D_PRIMITIVE_TOPOLOGY Primitive = D3D_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_POINTLIST
    ) {// Create Direct3D device and shaders. Call from wWinMain()
        HRESULT hr = MDX::InitDevice(ghWnd, Primitive);
        g_numVertices = vertices.size();
        RETURN_IF_FAIL(MDX::CreateSwapChain());
        RETURN_IF_FAIL(CreateComputeShader(vertices));
        RETURN_IF_FAIL(MDX::CreateVertexShader("VS3"));
        RETURN_IF_FAIL(MDX::CreatePixelShader());
        return hr;
    } // //////////////////////////////////////////////////////////////////////////////////
private:
    HRESULT CreateComputeShader(std::vector<Vertex3D>& vertices) {
        g_pAMPComputeEngine = new AMPEngine3(g_pd3dDevice);
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
public:
    void Render() {               //  Call from main loop wWinMain()
        g_pAMPComputeEngine->run();
        UINT stride = sizeof(Vertex3D);
        MDX::Render(stride);
    } // ///////////////////////////////////////////////////////////////////////////////////////////////////
    void CleanupDevice(){     //  Call from wWinMain() twice: onExit & onError
        MDX::CleanupDevice();
        SAFE_DELETE(g_pAMPComputeEngine);
    } // //////////////////////////////////////////////////////////////////////////////////////////
}; // ***********************************************************************


