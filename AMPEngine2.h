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

class AMPEngine2 {
	accelerator_view					m_accl_view;
	std::unique_ptr<array<int, 2>>      ar_area;
	std::unique_ptr<array<Vertex2D, 1>>	m_data;

	std::vector<std::unique_ptr<array<int, 2>>> var_areas;
	std::vector<std::unique_ptr<array<DirectX::XMFLOAT2, 2>>> var_dirs;

public:
	AMPEngine2(ID3D11Device* d3ddevice) : m_accl_view(create_accelerator_view(d3ddevice)) {}
	void initialize_data(const std::vector<Vertex2D>& data) {
		m_data = std::unique_ptr<array<Vertex2D, 1>>(new array<Vertex2D, 1>(data.size(), data.begin(), m_accl_view));
#ifdef MYAREA
		auto layscnt = model.v_areas.size();
		for (size_t nlay = 0; nlay < layscnt; nlay++) {
			int sizey = model.sizeY(nlay);
			int sizex = model.sizeX(nlay);

			var_areas.push_back(std::unique_ptr<array<int, 2>>());
			var_areas[nlay] = std::unique_ptr<array<int, 2>>
				(new array<int, 2>(sizey, sizex, model.v_areas[nlay].begin(), m_accl_view));

			if (nlay < layscnt - 1) {
				var_dirs.push_back(std::unique_ptr<array<DirectX::XMFLOAT2, 2>>());
				var_dirs[nlay] = std::unique_ptr<array<DirectX::XMFLOAT2, 2>>
					(new array<DirectX::XMFLOAT2, 2>(sizey, sizex, model.v_dirs[nlay].begin(), m_accl_view));
			}
		}
		auto v = model.v_poss[model.v_poss.size() - 1];
		m_data = std::unique_ptr<array<Vertex2D, 1>>(new array<Vertex2D, 1>(v.size(), v.begin(), m_accl_view));
#endif
	} // ///////////////////////////////////////////////////////////////////////////////////////////////
	HRESULT get_data_d3dbuffer(void** d3dbuffer) const {
		return get_buffer(*m_data)->QueryInterface(__uuidof(ID3D11Buffer), (LPVOID*)d3dbuffer);
	} // ///////////////////////////////////////////////////////////////////////////////////////////////
	void run() {
		array<Vertex2D, 1>& data_ref = *m_data;
		int lastlay = model.LaysCnt() - 1;
		array<int, 2>& src = *var_areas[lastlay - 1];
		array<int, 2>& dst = *var_areas[lastlay - 2];
		parallel_for_each(var_areas[lastlay - 1]->extent, [&dst, &src](index<2> idx) restrict(amp) { // TODO: dst.extent var_areas[lastlay - 1]->extent
			const int mask[16] = { 0, 0, 0, 0, 0, 0, 1, 1, 0, 1, 0, 1, 0, 1, 1, 1};
			                     //0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15
			int y = idx[0];
			int y2 = y * 2;
			int y2t = y2 - 1;
			int y2b = y2 + 1;
			
			int x = idx[1];
			int x2 = x * 2;
			int x2l = x2 - 1;
			int x2r = x2 + 1;

			int y2x2 = src[y2][x2];

			int y2tx2 = src[y2t][x2];
			int y2bx2 = src[y2b][x2];
			int y2x2l = src[y2][x2l];
			int y2x2r = src[y2][x2r];

			int sum0 = mask[y2x2 + (y2tx2 << 1) + (y2x2l << 2) + (src[y2t][x2l] << 3)];
			int sum1 = mask[y2x2 + (y2tx2 << 1) + (y2x2r << 2) + (src[y2t][x2r] << 3)];
			int sum2 = mask[y2x2 + (y2bx2 << 1) + (y2x2l << 2) + (src[y2b][x2l] << 3)];
			int sum3 = mask[y2x2 + (y2bx2 << 1) + (y2x2r << 2) + (src[y2b][x2r] << 3)];
			dst[y][x] = (((((sum3 << 1) + sum2) << 1) + sum1) << 1) + sum0;
			});
		for (int nlay = lastlay; nlay > 0; nlay--) {
		}
		parallel_for_each(m_data->extent, [=, &data_ref](index<1> idx) restrict(amp) {
			// Rotate the vertex by angle THETA
			DirectX::XMFLOAT2 pos = data_ref[idx].Pos;
			data_ref[idx].Pos.y = pos.y * cos(THETA) - pos.x * sin(THETA);
			data_ref[idx].Pos.x = pos.y * sin(THETA) + pos.x * cos(THETA);
			});
	} // ///////////////////////////////////////////////////////////////////////////////////////////////
	void runbak() {
		array<Vertex2D, 1>& data_ref = *m_data;
		parallel_for_each(m_data->extent, [=, &data_ref](index<1> idx) restrict(amp) {
			// Rotate the vertex by angle THETA
			DirectX::XMFLOAT2 pos = data_ref[idx].Pos;
			data_ref[idx].Pos.y = pos.y * cos(THETA) - pos.x * sin(THETA);
			data_ref[idx].Pos.x = pos.y * sin(THETA) + pos.x * cos(THETA);
			});
	} // ///////////////////////////////////////////////////////////////////////////////////////////////
	std::vector<Vertex2D>* return_data(std::vector<Vertex2D>* vreturn) {
		concurrency::copy(*m_data, begin(*vreturn));
		return vreturn;
	} // ///////////////////////////////////////////////////////////////////////////////////////////////
}; // ******************************************************************************************************


