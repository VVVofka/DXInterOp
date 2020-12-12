#pragma once
#include <memory>
#include <amp.h>
#include <amp_math.h>
#include <amp_graphics.h>
#include <numeric>
//#include <ppl.h>

//#pragma warning(push)
//#pragma warning(disable:4005 26812) 
#include <D3D11.h>
//#pragma warning(pop)

#include "Vertex.h"
#include "DXInterOp.h"
#define THETA 3.1415f/1024  

using namespace concurrency;
using namespace concurrency::fast_math;
using namespace concurrency::direct3d;

class AMPEngine3{
private:
	accelerator_view					m_accl_view;
	std::unique_ptr<array<Vertex3D, 1>>	m_data; // set in initialize_data() from MDX3.CreateComputeShader()
	std::vector<Vertex3D> vret;
public:
	AMPEngine3(ID3D11Device* d3ddevice) : m_accl_view(create_accelerator_view(d3ddevice)){}

	void initialize_data(const std::vector<Vertex3D>& data){ // Call from MDX3.CreateComputeShader()
		m_data = std::unique_ptr<array<Vertex3D, 1>>(new array<Vertex3D, 1>(data.size(), data.begin(), m_accl_view));
	} // ///////////////////////////////////////////////////////////////////////////////////////////////
	std::vector<Vertex3D>* return_data(){ // std::vector<Vertex3D>& data
		if(vret.size() != m_data->extent.size())
			vret.resize(m_data->extent.size());
		concurrency::copy(*m_data, begin(vret));
		return &vret;
	} // ///////////////////////////////////////////////////////////////////////////////////////////////
	std::vector<Vertex3D>* return_data(std::vector<Vertex3D>* vreturn){ // std::vector<Vertex3D>& data
		concurrency::copy(*m_data, begin(*vreturn));
		return vreturn;
	} // ///////////////////////////////////////////////////////////////////////////////////////////////
	HRESULT get_data_d3dbuffer(void** d3dbuffer) const{
		return get_buffer(*m_data)->QueryInterface(__uuidof(ID3D11Buffer), (LPVOID*)d3dbuffer);
	} // ///////////////////////////////////////////////////////////////////////////////////////////////
	void run(){  //Call from MDX.Render() from "main loop" in wWinMain()
		array<Vertex3D, 1>& data_ref = *m_data; // private std::unique_ptr<array<Vertex3D, 1>>
		parallel_for_each(m_data->extent, [=, &data_ref](index<1> idx) restrict(amp){
			// Rotate the vertex by angle THETA
			DirectX::XMFLOAT3 pos = data_ref[idx].Pos;
			data_ref[idx].Pos.y = pos.y * cos(THETA) - pos.x * sin(THETA);
			data_ref[idx].Pos.x = pos.y * sin(THETA) + pos.x * cos(THETA);
			data_ref[idx].Pos.z = pos.y * cos(THETA) + pos.x * sin(THETA);
		});
	} // ///////////////////////////////////////////////////////////////////////////////////////////////
}; // ******************************************************************************************************


