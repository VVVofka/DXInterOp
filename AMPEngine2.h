#pragma once
#include <memory>
#include <amp.h>
#include <amp_math.h>
#include <amp_graphics.h>
#include "DXInterOp.h"
//#include <ppl.h>
#include <numeric>
#include "Model2D.h"
#include "Masks.h"
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
	std::vector<std::unique_ptr<array<DirItem, 2>>> var_dirs;

public:
	AMPEngine2(ID3D11Device* d3ddevice) : m_accl_view(create_accelerator_view(d3ddevice)){}
	void initialize_data(const std::vector<Vertex2D>& data){
		m_data = std::unique_ptr<array<Vertex2D, 1>>(new array<Vertex2D, 1>(data.size(), data.begin(), m_accl_view));
#ifdef MYAREA
		auto layscnt = model.v_areas.size();
		for(size_t nlay = 0; nlay < layscnt; nlay++){
			int sizey = model.sizeY(nlay);
			int sizex = model.sizeX(nlay);

			var_areas.push_back(std::unique_ptr<array<int, 2>>());
			var_areas[nlay] = std::unique_ptr<array<int, 2>>
				(new array<int, 2>(sizey, sizex, model.v_areas[nlay].begin(), m_accl_view));

			if(nlay < layscnt - 1){
				var_dirs.push_back(std::unique_ptr<array<DirItem, 2>>());
				var_dirs[nlay] = std::unique_ptr<array<DirItem, 2>>
					(new array<DirItem, 2>(sizey, sizex, model.v_dirs[nlay].begin(), m_accl_view));
			}
		}
		auto v = model.v_poss[model.v_poss.size() - 1];
		m_data = std::unique_ptr<array<Vertex2D, 1>>(new array<Vertex2D, 1>(v.size(), v.begin(), m_accl_view));
#endif
	} // ///////////////////////////////////////////////////////////////////////////////////////////////
	HRESULT get_data_d3dbuffer(void** d3dbuffer) const{
		return get_buffer(*m_data)->QueryInterface(__uuidof(ID3D11Buffer), (LPVOID*)d3dbuffer);
	} // ///////////////////////////////////////////////////////////////////////////////////////////////
	void run(){
		int nlastlay = model.LaysCnt() - 1;
		array<int, 2>& src = *var_areas[nlastlay];
		array<int, 2>& dst = *var_areas[nlastlay - 1];
		array<DirItem, 2>& srcd = *var_dirs[nlastlay];
		array<DirItem, 2>& dstd = *var_dirs[nlastlay - 1];
		runAlast(src, dst);
		for(int nlay = nlastlay - 1; nlay > 0; nlay--){
			src = dst;
			dst = *var_areas[nlay - 1];
			runA(src, dst);
		}
		for(int nlay = 1; nlay < nlastlay; nlay++){
			srcd = *var_dirs[nlay - 1];
			dstd = *var_dirs[nlay];
			dst = *var_areas[nlay];
			runD(srcd, dstd, dst, dirXMasks, dirYMasks);
		}
		src = dst;
		dst = *var_areas[nlastlay];
		runDlast(src, dst);

		array<Vertex2D, 1>& data_ref = *m_data;
		parallel_for_each(m_data->extent, [=, &data_ref](index<1> idx) restrict(amp){
			// Rotate the vertex by angle THETA
			DirectX::XMFLOAT2 pos = data_ref[idx].Pos;
			data_ref[idx].Pos.y = pos.y * cos(THETA) - pos.x * sin(THETA);
			data_ref[idx].Pos.x = pos.y * sin(THETA) + pos.x * cos(THETA);
		});
	} // ///////////////////////////////////////////////////////////////////////////////////////////////
	void runAlast(array<int, 2>& src, array<int, 2>& dst){
		parallel_for_each(src.extent, [&dst, &src](index<2> idx) restrict(amp){ // TODO: dst.extent var_areas[lastlay - 1]->extent
			const int mask[16] = {0, 0, 0, 0, 0, 0, 1, 1, 0, 1, 0, 1, 0, 1, 1, 1};
			//00 10 01 11 00 10 01 11 00 10 01 11 00 10 01 11
			//00 00 00 00 10 10 10 10 01 01 01 01 11 11 11 11
			//0  1  2  3  4  5  6  7  8  9  10 11 12 13 14 15
			const int y = idx[0];
			const int y2 = y * 2;
			const int y2t = y2 - 1;
			const int y2b = y2 + 1;

			const int x = idx[1];
			const int x2 = x * 2;
			const int x2l = x2 - 1;
			const int x2r = x2 + 1;

			// yx: c-centre, l-left, r-right
			const int cc = src[y2][x2] & 1;
			const int tc = (src[y2t][x2] & 1) << 1;
			const int cl = (src[y2][x2l] & 1) << 2;
			const int tl = (src[y2t][x2l] & 1) << 3;
			int sum = mask[cc + tc + cl + tl];
			const int cr = (src[y2][x2r] & 1) << 2;
			const int tr = (src[y2t][x2r] & 1) << 3;
			sum = (sum << 1) | mask[cc + tc + cr + tr];
			const int bc = (src[y2b][x2] & 1) << 1;
			const int bl = (src[y2b][x2l] & 1) << 3;
			sum = (sum << 1) | mask[cc + bc + cl + bl];
			const int br = (src[y2b][x2r] & 1) << 3;
			dst[y][x] = (sum << 1) | mask[cc + bc + cr + br];
		});
	} // ///////////////////////////////////////////////////////////////////////////////////////////////
	void runA(array<int, 2>& src, array<int, 2>& dst){
		parallel_for_each(src.extent, [&dst, &src](index<2> idx) restrict(amp){ // TODO: dst.extent var_areas[lastlay - 1]->extent
			const int mask[16] = {0, 0, 0, 0, 0, 0, 1, 1, 0, 1, 0, 1, 0, 1, 1, 1};
			//00 10 01 11 00 10 01 11 00 10 01 11 00 10 01 11
			//00 00 00 00 10 10 10 10 01 01 01 01 11 11 11 11
			//0  1  2  3  4  5  6  7  8  9  10 11 12 13 14 15
			const int y = idx[0];
			const int y2 = y * 2;
			const int x = idx[1];
			const int x2 = x * 2;

			// yx: l-left, r-right
			int tl = src[y2][x2];
			int tr = src[y2][x2 + 1];
			int bl = src[y2 + 1][x2];
			int br = src[y2 + 1][x2 + 1];
			int sum = mask[(tl & 1) + ((tr & 1) << 1) + ((bl & 1) << 2) + ((br & 1) << 3)];
			sum = (sum << 1) | mask[((tl >>= 1) & 1) + (((tr >>= 1) & 1) << 1) + (((bl >>= 1) & 1) << 2) + (((br >>= 1) & 1) << 3)];
			sum = (sum << 1) | mask[((tl >>= 1) & 1) + (((tr >>= 1) & 1) << 1) + (((bl >>= 1) & 1) << 2) + (((br >>= 1) & 1) << 3)];
			dst[y][x] = (sum << 1) | mask[((tl >> 1) & 1) + (((tr >> 1) & 1) << 1) + (((bl >> 1) & 1) << 2) + (((br >> 1) & 1) << 3)];
		});
	} // ///////////////////////////////////////////////////////////////////////////////////////////////
	void runD(array<DirItem, 2>& srcd, 
			  array<DirItem, 2>& dstd, array<int, 2>& dsta,
			  array<float, 1>& dirxmasks, array<float, 1>& dirymasks){
		parallel_for_each(srcd.extent, [&srcd, &dstd, &dsta, &dirxmasks, &dirymasks](index<2> idx) restrict(amp){ // TODO: dst.extent var_areas[lastlay - 1]->extent
			const int y = idx[0];
			const int y2 = y * 2;
			const int x = idx[1];
			const int x2 = x * 2;
			// yx: l-left, r-right
			int tl = dsta[y2][x2];
			int tr = dsta[y2][x2 + 1];
			int bl = dsta[y2 + 1][x2];
			int br = dsta[y2 + 1][x2 + 1];
			int mask0 = 16 * ((tl & 1) + ((tr & 1) << 1) + ((bl & 1) << 2) + ((br & 1) << 3));
			int mask1 = 16 * (((tl >>= 1) & 1) + (((tr >>= 1) & 1) << 1) + (((bl >>= 1) & 1) << 2) + (((br >>= 1) & 1) << 3));
			int mask2 = 16 * (((tl >>= 1) & 1) + (((tr >>= 1) & 1) << 1) + (((bl >>= 1) & 1) << 2) + (((br >>= 1) & 1) << 3));
			int mask3 = 16 * (((tl >> 1) & 1) + (((tr >> 1) & 1) << 1) + (((bl >> 1) & 1) << 2) + (((br >> 1) & 1) << 3));

			int j = mask0;
			float srcx = srcd[y][x].x0, srcy = srcd[y][x].y0;
			DirItem* item = &dstd[y2][x2];
			item->x0 = srcx + dirxmasks[j]; item->y0 = srcy + dirymasks[j++];
			item->x1 = srcx + dirxmasks[j]; item->y1 = srcy + dirymasks[j++];
			item->x2 = srcx + dirxmasks[j]; item->y2 = srcy + dirymasks[j++];
			item->x3 = srcx + dirxmasks[j]; item->y3 = srcy + dirymasks[j++];
			
			srcx = srcd[y][x].x1, srcy = srcd[y][x].y1;
			item = &dstd[y2][x2 + 1];
			item->x0 = srcx + dirxmasks[j]; item->y0 = srcy + dirymasks[j++];
			item->x1 = srcx + dirxmasks[j]; item->y1 = srcy + dirymasks[j++];
			item->x2 = srcx + dirxmasks[j]; item->y2 = srcy + dirymasks[j++];
			item->x3 = srcx + dirxmasks[j]; item->y3 = srcy + dirymasks[j++];

			srcx = srcd[y][x].x2, srcy = srcd[y][x].y2;
			item = &dstd[y2 + 1][x2];
			item->x0 = srcx + dirxmasks[j]; item->y0 = srcy + dirymasks[j++];
			item->x1 = srcx + dirxmasks[j]; item->y1 = srcy + dirymasks[j++];
			item->x2 = srcx + dirxmasks[j]; item->y2 = srcy + dirymasks[j++];
			item->x3 = srcx + dirxmasks[j]; item->y3 = srcy + dirymasks[j++];

			srcx = srcd[y][x].x3, srcy = srcd[y][x].y3;
			item = &dstd[y2 + 1][x2 + 1];
			item->x0 = srcx + dirxmasks[j]; item->y0 = srcy + dirymasks[j++];
			item->x1 = srcx + dirxmasks[j]; item->y1 = srcy + dirymasks[j++];
			item->x2 = srcx + dirxmasks[j]; item->y2 = srcy + dirymasks[j++];
			item->x3 = srcx + dirxmasks[j]; item->y3 = srcy + dirymasks[j];
		});
	} // ///////////////////////////////////////////////////////////////////////////////////////////////
	void runDlast(array<int, 2>& src, array<int, 2>& dst){

	} // ///////////////////////////////////////////////////////////////////////////////////////////////
	void runbak(){
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
	const float vdirsX[16 * 16] = {
-1,-0,-1,-1,-0,+1,+1,+1,-1,-1,-1,-0,+1,+1,-0,+1,
-0,-0,-0,-0,-0,-1,-0,-1,+1,+1,+1,+1,-0,-1,-0,-1,
+1,-0,+1,-0,-0,-0,-0,-0,+1,-0,+1,-0,-1,-1,-1,-1,
-0,-0,-0,-0,-0,-0,-0,-0,-1,-1,-1,-1,+1,+1,+1,+1,
+1,+1,+1,+1,-0,-1,-0,-1,-0,-0,-0,-0,-0,-1,-0,-1,
-0,-0,-0,-0,-1,-1,-0,-1,-0,-0,-0,-0,-0,-1,-1,-1,
+1,+1,-1,+1,-0,-0,-0,-0,-0,-0,-0,-0,-1,+1,-1,-1,
-0,-0,-0,-0,-0,-0,-0,-0,-0,-0,-0,-0,+1,+1,-1,+1,
+1,-0,+1,-0,-1,-1,-1,-1,+1,-0,+1,-0,-0,-0,-0,-0,
-0,-0,-0,-0,-1,-1,-1,+1,-1,+1,+1,+1,-0,-0,-0,-0,
+1,+1,+1,-0,-0,-0,-0,-0,+1,-0,+1,+1,-0,-0,-0,-0,
-0,-0,-0,-0,-0,-0,-0,-0,-1,-1,-1,+1,-0,-0,-0,-0,
-1,-1,-1,-1,+1,+1,+1,+1,-0,-0,-0,-0,-0,-0,-0,-0,
-0,-0,-0,-0,-1,+1,+1,+1,-0,-0,-0,-0,-0,-0,-0,-0,
-1,+1,-1,-1,-0,-0,-0,-0,-0,-0,-0,-0,-0,-0,-0,-0,
-1,-0,-1,-1,-0,+1,+1,+1,-1,-1,-1,-0,+1,+1,-0,+1 };
	const float vdirsY[16 * 16] = {
-1,-1,-0,-1,-1,-1,-1,-0,-0,+1,+1,+1,+1,-0,+1,+1,
-0,-0,-0,-0,+1,+1,+1,+1,-0,-0,-1,-1,-0,-0,-1,-1,
+1,+1,+1,+1,-0,-0,-0,-0,-0,-0,-1,-1,-0,-0,-1,-1,
-0,-0,-0,-0,-0,-0,-0,-0,-1,-0,-1,-1,-0,-1,-1,-1,
+1,+1,-0,-0,+1,+1,-0,-0,-0,-0,-0,-0,-1,-1,-1,-1,
-0,-0,-0,-0,-1,-1,-1,-1,-0,-0,-0,-0,+1,+1,+1,+1,
+1,-1,+1,+1,-0,-0,-0,-0,-0,-0,-0,-0,-1,-1,+1,-1,
-0,-0,-0,-0,-0,-0,-0,-0,-0,-0,-0,-0,+1,-1,+1,+1,
+1,+1,-0,-0,+1,+1,-0,-0,-1,-1,-1,-1,-0,-0,-0,-0,
-0,-0,-0,-0,-1,+1,+1,+1,-1,-1,-1,+1,-0,-0,-0,-0,
-1,-1,-1,-1,-0,-0,-0,-0,+1,+1,+1,+1,-0,-0,-0,-0,
-0,-0,-0,-0,-0,-0,-0,-0,-1,+1,+1,+1,-0,-0,-0,-0,
+1,+1,+1,-0,+1,+1,-0,+1,-0,-0,-0,-0,-0,-0,-0,-0,
-0,-0,-0,-0,-1,-1,-1,+1,-0,-0,-0,-0,-0,-0,-0,-0,
-1,-1,+1,-1,-0,-0,-0,-0,-0,-0,-0,-0,-0,-0,-0,-0,
-1,-1,-0,-1,-1,-1,-1,-0,-0,+1,+1,+1,+1,-0,+1,+1	};
array<float, 1> dirXMasks = array<float, 1>(16*16, vdirsX);
array<float, 1> dirYMasks = array<float, 1>(16*16, vdirsY);

}; // ******************************************************************************************************


