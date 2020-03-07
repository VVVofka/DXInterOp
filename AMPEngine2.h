#pragma once
#include <memory>
#include <amp.h>
#include <amp_math.h>
#include <amp_graphics.h>
#include "DXInterOp.h"
//#include <ppl.h>
#include <numeric>
#include "Model2D.h"
#include <DirectXMath.h>

#define THETA 3.1415f/1024  
extern Model2D model;

using namespace concurrency;
using namespace concurrency::fast_math;
using namespace concurrency::direct3d;

class AMPEngine2{
	accelerator_view					m_accl_view;
	std::unique_ptr<array<int, 2>>      ar_area;
	std::unique_ptr<array<Vertex2D, 1>>	m_data;

	std::vector<std::unique_ptr<array<int, 2>>> var_areas;
	std::vector<std::unique_ptr<array<Vertex2D, 1>>> var_poss;
	std::vector<std::unique_ptr<array<DirectX::XMFLOAT2, 2>>> var_dirs;
public:
	AMPEngine2(ID3D11Device* d3ddevice) : m_accl_view(create_accelerator_view(d3ddevice)){}
	void initialize_data(const std::vector<Vertex2D>& data){
		m_data = std::unique_ptr<array<Vertex2D, 1>>(new array<Vertex2D, 1>(data.size(), data.begin(), m_accl_view));
#ifdef MYAREA
		//ar_area= std::unique_ptr<array<int, 2>>(new array<int, 2>(model.sz.y, model.sz.x, model.lastArea().begin(), m_accl_view));

		for(size_t nlay = 0; nlay < model.v_areas.size(); nlay++){
			int y = model.sizeY(nlay);
			int x = model.sizeX(nlay);

			var_areas.push_back(std::unique_ptr<array<int, 2>>());
			var_areas[nlay] = std::unique_ptr<array<int, 2>>
				(new array<int, 2>(y, x, model.v_areas[nlay].begin(), m_accl_view));

			var_dirs.push_back(std::unique_ptr<array<DirectX::XMFLOAT2, 2>>());
			var_dirs[nlay] = std::unique_ptr<array<DirectX::XMFLOAT2, 2>>
				(new array<DirectX::XMFLOAT2, 2>(y, x, model.v_dirs[nlay].begin(), m_accl_view));

			var_poss.push_back(std::unique_ptr<array<Vertex2D, 1>>());
			var_poss[nlay] = std::unique_ptr<array<Vertex2D, 1>>
				(new array<Vertex2D, 1>(model.v_poss[nlay].size(), model.v_poss[nlay].begin(), m_accl_view));
		}
#endif
	} // ///////////////////////////////////////////////////////////////////////////////////////////////
	HRESULT get_data_d3dbuffer(void** d3dbuffer) const{
		return get_buffer(*m_data)->QueryInterface(__uuidof(ID3D11Buffer), (LPVOID*)d3dbuffer);
	} // ///////////////////////////////////////////////////////////////////////////////////////////////
	void run(){
		array<Vertex2D, 1>& data_ref = *m_data;
		parallel_for_each(m_data->extent, [=, &data_ref](index<1> idx) restrict(amp){
			// Rotate the vertex by angle THETA
			DirectX::XMFLOAT2 pos = data_ref[idx].Pos;
			data_ref[idx].Pos.y = pos.y * cos(THETA) - pos.x * sin(THETA);
			data_ref[idx].Pos.x = pos.y * sin(THETA) + pos.x * cos(THETA);
		});
	} // ///////////////////////////////////////////////////////////////////////////////////////////////
	std::vector<Vertex2D>* return_data(std::vector<Vertex2D>* vreturn){
		concurrency::copy(*m_data, begin(*vreturn));
		return vreturn;
	} // ///////////////////////////////////////////////////////////////////////////////////////////////
}; // ******************************************************************************************************


