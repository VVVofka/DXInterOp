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
        return MDX::CreateComputeShader();
    } // /////////////////////////////////////////////////////////////////////////////////////////////
public:
    void Render() {               //  Call from main loop wWinMain()
        g_pAMPComputeEngine->run();
        MDX::Render(sizeof(Vertex3D));
    } // ///////////////////////////////////////////////////////////////////////////////////////////////////
    void CleanupDevice(){     //  Call from wWinMain() twice: onExit & onError
        MDX::CleanupDevice();
        SAFE_DELETE(g_pAMPComputeEngine);
    } // //////////////////////////////////////////////////////////////////////////////////////////
}; // ***********************************************************************


