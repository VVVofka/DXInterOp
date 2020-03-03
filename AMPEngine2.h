#pragma once
#include <memory>
#include <amp.h>
#include <amp_math.h>
#include <amp_graphics.h>
#include "DXInterOp.h"
//#include <ppl.h>
#include <numeric>
#define THETA 3.1415f/1024  

using namespace concurrency;
using namespace concurrency::fast_math;
using namespace concurrency::direct3d;

class AMPEngine2{
public:
	AMPEngine2(ID3D11Device* d3ddevice) : m_accl_view(create_accelerator_view(d3ddevice)){}

	std::vector<DirectX::XMFLOAT2> v_pos;
	std::vector<int> v_area;
	void Create(int minszX, int minszY, int maxsz, double kFill){
		int szx = minszX;
		int szy = minszY;
		int sz = __max(minszX, minszY);
		while(sz <= maxsz){
			// TODO: fill lays here
			szy *= 2, szx *= 2, sz *= 2;
		}
		size_t szarea = ++szx * ++szy;
		v_area.resize(szarea);
		size_t szpos = int(szarea * kFill + 0.5);
		v_pos.resize(szpos);
		double kfact = double(szy) / double(szx);
		for(auto q : v_pos){
			// TODO: rand
			q.x = szx / 2 * cos()
		}
	} // //////////////////////////////////////////////////////////////////////////////////

	void initialize_data(int num_elements, const Vertex2D* data){
		m_data = std::unique_ptr<array<Vertex2D, 1>>(new array<Vertex2D, 1>(num_elements, data, m_accl_view));
	} // ///////////////////////////////////////////////////////////////////////////////////////////////
	void initialize_data(const std::vector<Vertex2D>& data){
		m_data = std::unique_ptr<array<Vertex2D, 1>>(new array<Vertex2D, 1>(data.size(), data.begin(), m_accl_view));
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

private:
	accelerator_view					          m_accl_view;
	std::unique_ptr<array<Vertex2D, 1>>	          m_data;
	std::unique_ptr<array<DirectX::XMFLOAT2, 1>>  ar_pos;

}; // ******************************************************************************************************


