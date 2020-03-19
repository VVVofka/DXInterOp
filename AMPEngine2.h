#pragma once
#include <memory>
#include <amp.h>
#include <amp_math.h>
#include <amp_graphics.h>
#include "DXInterOp.h"
#include <numeric>
#include "Model2D.h"
#include "Masks.h"
#include <DirectXMath.h>
//#include <ppl.h>

#define THETA 3.1415f/1024  
extern Model2D model;

using namespace concurrency;
using namespace concurrency::fast_math;
using namespace concurrency::direct3d;

class AMPEngine2{
	accelerator_view					m_accl_view;
	//std::unique_ptr<array<int, 2>>      ar_area;
	std::unique_ptr<array<Vertex2D, 1>>	m_data;
	std::unique_ptr<array<FLT2, 2>> last_dirs;

	std::vector<std::unique_ptr<array<int, 2>>> var_areas;
	std::vector<std::unique_ptr<array<DrShiftQuadro, 2>>> var_dirs;

public:
	AMPEngine2(ID3D11Device* d3ddevice) : m_accl_view(create_accelerator_view(d3ddevice)){}
#ifndef MYAREA
	void initialize_data(const std::vector<Vertex2D>& data){
		m_data = std::unique_ptr<array<Vertex2D, 1>>(new array<Vertex2D, 1>(data.size(), data.begin(), m_accl_view));
#else
	void initialize_data(){
		auto layscnt = model.v_areas.size();
		for(size_t nlay = 0; nlay < layscnt; nlay++){
			int sizey = model.sizeY(nlay);
			int sizex = model.sizeX(nlay);

			var_areas.push_back(std::unique_ptr<array<int, 2>>());
			var_areas[nlay] = std::unique_ptr<array<int, 2>>
				(new array<int, 2>(sizey, sizex, model.v_areas[nlay].begin(), m_accl_view));

			if(nlay < layscnt - 1){
				var_dirs.push_back(std::unique_ptr<array<DrShiftQuadro, 2>>());
				var_dirs[nlay] = std::unique_ptr<array<DrShiftQuadro, 2>>
					(new array<DrShiftQuadro, 2>(sizey, sizex, model.v_dirs[nlay].begin(), m_accl_view));
			}
		}
		auto v = model.v_poss[model.v_poss.size() - 1];
		m_data = std::unique_ptr<array<Vertex2D, 1>>(new array<Vertex2D, 1>(v.size(), v.begin(), m_accl_view));
		last_dirs = std::unique_ptr<array<FLT2, 2>>(new array<FLT2, 2>(model.sizeY(), model.sizeX(), model.last_dirs.begin(), m_accl_view));
#endif
	} // ///////////////////////////////////////////////////////////////////////////////////////////////
	HRESULT get_data_d3dbuffer(void** d3dbuffer) const{
		return get_buffer(*m_data)->QueryInterface(__uuidof(ID3D11Buffer), (LPVOID*)d3dbuffer);
	} // ///////////////////////////////////////////////////////////////////////////////////////////////
	void run(){
		int nlastlay = model.LaysCnt() - 1;
		runAlast(*var_areas[nlastlay], *var_areas[nlastlay - 1]);
		for(int nlay = nlastlay - 1; nlay > 0; nlay--){
			runA(*var_areas[nlay], *var_areas[nlay - 1]);
		}
		for(int nlay = 1; nlay < nlastlay; nlay++){
			runD(*var_dirs[nlay - 1], *var_dirs[nlay], *var_areas[nlay]);
		}
		array<FLT2, 2>& dirs = *last_dirs;
		runDlast(*var_dirs[nlastlay - 1], *m_data, *var_areas[nlastlay], dirs);
	} // ///////////////////////////////////////////////////////////////////////////////////////////////
	void runAlast(array<int, 2> & src, array<int, 2> & dst){
		parallel_for_each(dst.extent, [&dst, &src](index<2> idx) restrict(amp){ // TODO: dst.extent var_areas[lastlay - 1]->extent
			const int mask[16] = {0, 0, 0, 0, 0, 0, 1, 1, 0, 1, 0, 1, 0, 1, 1, 1};
			//00 10 01 11 00 10 01 11 00 10 01 11 00 10 01 11
			//00 00 00 00 10 10 10 10 01 01 01 01 11 11 11 11
			//0  1  2  3  4  5  6  7  8  9  10 11 12 13 14 15
			// yx: c-centre, l-left, r-right
			const int y = idx[0];
			const int y2t = y * 2;
			const int y2c = y2t + 1;
			const int y2b = y2c + 1;

			const int x = idx[1];
			const int x2l = x * 2;
			const int x2c = x2l + 1;
			const int x2r = x2c + 1;

			const int cc = src[y2c][x2c] & 1;
			const int tc = (src[y2t][x2c] & 1) << 1;
			const int cl = (src[y2c][x2l] & 1) << 2;
			const int tl = (src[y2t][x2l] & 1) << 3;
			int sum = mask[cc + tc + cl + tl];
			const int cr = (src[y2c][x2r] & 1) << 2;
			const int tr = (src[y2t][x2r] & 1) << 3;
			sum = (sum << 1) | mask[cc + tc + cr + tr];
			const int bc = (src[y2b][x2c] & 1) << 1;
			const int bl = (src[y2b][x2l] & 1) << 3;
			sum = (sum << 1) | mask[cc + bc + cl + bl];
			const int br = (src[y2b][x2r] & 1) << 3;
			dst[y][x] = (sum << 1) | mask[cc + bc + cr + br];
		});
	} // ///////////////////////////////////////////////////////////////////////////////////////////////
	void runA(array<int, 2> & src, array<int, 2> & dst){
		parallel_for_each(dst.extent, [&dst, &src](index<2> idx) restrict(amp){ // TODO: dst.extent var_areas[lastlay - 1]->extent
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
	void runD(array<DrShiftQuadro, 2> & srcd,
			  array<DrShiftQuadro, 2> & dstd, array<int, 2> & dsta
			  //array<float, 1> & dirxmasks, array<float, 1> & dirymasks
			  //float* dirmx
			  ){
		//parallel_for_each(srcd.extent, [&srcd, &dstd, &dsta, &dirxmasks, &dirymasks](index<2> idx) restrict(amp){ // TODO: dst.extent var_areas[lastlay - 1]->extent
		parallel_for_each(srcd.extent, [&srcd, &dstd, &dsta](index<2> idx) restrict(amp){ // TODO: dst.extent var_areas[lastlay - 1]->extent
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
		-1,-0,-1,-1,-0,+1,+1,+1,-1,-1,-1,-0,+1,+1,-0,+1};
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
		-1,-1,-0,-1,-1,-1,-1,-0,-0,+1,+1,+1,+1,-0,+1,+1};

			int mask[4]; // shift
			const int y = idx[0];
			const int x = idx[1];
			const int y2 = y * 2;
			const int x2 = x * 2;
			//getMaskDir(y2, x2, dsta, mask);
			{
				int tl = dsta[y2][x2];
				int tr = dsta[y2][x2 + 1];
				int bl = dsta[y2 + 1][x2];
				int br = dsta[y2 + 1][x2 + 1];
				mask[0] = 16 * ((tl & 1) + ((tr & 1) << 1) + ((bl & 1) << 2) + ((br & 1) << 3));
				mask[1] = 16 * (((tl >>= 1) & 1) + (((tr >>= 1) & 1) << 1) + (((bl >>= 1) & 1) << 2) + (((br >>= 1) & 1) << 3));
				mask[2] = 16 * (((tl >>= 1) & 1) + (((tr >>= 1) & 1) << 1) + (((bl >>= 1) & 1) << 2) + (((br >>= 1) & 1) << 3));
				mask[3] = 16 * (((tl >> 1) & 1) + (((tr >> 1) & 1) << 1) + (((bl >> 1) & 1) << 2) + (((br >> 1) & 1) << 3));
			}
			for(int shift = 0; shift < 4; shift++){
				int nmask = mask[shift];
				auto srcsh = &srcd[y][x].shifts[shift];
				for(int qSrc = 0; qSrc < 4; qSrc++){
					auto src = srcsh->items[qSrc];
					int yitem = y2 + qSrc / 2;
					int xitem = x2 + qSrc % 2;
					auto item = &dstd[yitem][xitem].shifts[shift];
					for(int qDst = 0; qDst < 4; qDst++){
						auto dst = &item->items[qDst];
						dst->x = src.x + vdirsX[nmask];
						dst->y = src.y + vdirsY[nmask++];
					}
				}
			}
		});
	} // ///////////////////////////////////////////////////////////////////////////////////////////////
	void runDlast(array<DrShiftQuadro, 2> & srcd,
				  array<Vertex2D, 1> & dstpos,
				  array<int, 2> & dsta,
				  array<FLT2, 2> & dstd){
		int szy = model.sizeY(), szx = model.sizeX();
		for(int nshift = 0; nshift < 4; nshift++){
			int yshift = nshift / 2;
			int xshift = nshift % 2;
			parallel_for_each(srcd.extent, [=, &srcd, &dstd](index<2> idx) restrict(amp){
				auto q = srcd[idx[0]][idx[1]].shifts[nshift].items;
				int y2 = mad(idx[0], 2, yshift); // y*2+yshift
				int x2 = mad(idx[1], 2, xshift);

				dstd[y2][x2].y += q->y;
				dstd[y2][x2].x += q->x;

				dstd[y2][x2 + 1].y += (++q)->y;
				dstd[y2][x2 + 1].x += q->x;

				dstd[y2 + 1][x2].y += (++q)->y;
				dstd[y2 + 1][x2].x += q->x;

				dstd[y2 + 1][x2 + 1].y += (++q)->y;
				dstd[y2 + 1][x2 + 1].x += q->x;
			}); // parallel_for_each(srcd.extent,
		} // for(nshift
		for(int nshift = 0; nshift < 4; nshift++){
			int yshift = nshift / 2;
			int xshift = nshift % 2;
			parallel_for_each(srcd.extent, [=, &dsta, &dstd, &dstpos](index<2> idx) restrict(amp){
				int y2 = idx[0] * 2 + yshift;
				int x2 = idx[1] * 2 + xshift;
				float ky = 2.f / szy;
				float kx = 2.f / szx;

				for(int nitem = 0; nitem < 4; nitem++){
					const int mask[7] = {0,0,1, 9, 0,1,1};
					int yitem = nitem / 2;
					int xitem = nitem % 2;
					int y = y2 + yitem;
					int x = x2 + xitem;
					float diry = dstd[y][x].y;
					float absdiry = fabsf(diry);
					float dirx = dstd[y][x].x;
					float absdirx = fabsf(dirx);
					if(absdirx > 2 * absdiry)
						diry = 0;
					else if(absdiry > 2 * absdirx)
						dirx = 0;
					int adry = (yitem << 2) + sign((int)diry) + 1; // 0..1 | 0..2
					int adrx = (xitem << 2) + sign((int)dirx) + 1;

					int newy = y2 + mask[adry];
					int newx = x2 + mask[adrx];

					int tmp1 = dsta[newy][newx];
					dsta[y2][x2] = tmp1;

					int tmp2 = dsta[y2][x2];
					dsta[newy][newx] = tmp2;

					if(tmp1 >= 0){
						dstpos[tmp1].Pos.y = ky * y2 - 1.0f;
						dstpos[tmp1].Pos.x = kx * x2 - 1.0f;
					}
					if(tmp2 >= 0){
						dstpos[tmp2].Pos.y = ky * newy - 1.0f;
						dstpos[tmp2].Pos.x = kx * newx - 1.0f;
					}

					dstd[y2][x2].y = dstd[y2][x2].x = 0;
					dstd[newy][newx].y = dstd[newy][newx].x = 0;
				}
			}); // parallel_for_each(srcd.extent,
		} // for(nshift
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
	std::vector<Vertex2D>* return_data(std::vector<Vertex2D> * vreturn){
		concurrency::copy(*m_data, begin(*vreturn));
		return vreturn;
	} // ///////////////////////////////////////////////////////////////////////////////////////////////
private:
}; // ******************************************************************************************************

//void moveQuad(
//	const int y1, const int x1,
//	const int y, const int x,
//	array<int, 2>& dsta,
//	array<DrShiftQuadro, 2>& srcd,
//	int shift
//) restrict(amp)
//{
//		if(dsta[y][x] != 0){ // 0
//	//		//DrShiftQuadro::DrQuadro::DrItem* dr = &dir.items[0];
//			auto dr = &srcd[y1][x1].shifts[shift].items[0];
//			if(dsta[y][x + 1] == 0 && dr->y == 0 && dr->x > 0){
//				dr->x = 0;
//				dsta[y][x] = 0;
//				dsta[y][x + 1] = 1;
//			} else if(dsta[y + 1][x] == 0 && dr->y > 0 && dr->x == 0){
//				dr->y = 0;
//				dsta[y][x] = 0;
//				dsta[y + 1][x] = 1;
//			} else if(dsta[y + 1][x + 1] == 0 && dr->y > 0 && dr->x > 0){
//				dr->y = 0;
//				dr->x = 0;
//				dsta[y][x] = 0;
//				dsta[y + 1][x + 1] = 1;
//			}
//		}
//		if(dsta[y][x + 1] != 0){ // 1
//			//DrShiftQuadro::DrQuadro::DrItem* dr = &dir.items[1];
//			auto dr = &srcd[y1][x1].shifts[shift].items[1];
//			if(dsta[y][x] == 0 && dr->y == 0 && dr->x < 0){
//				dr->x = 0;
//				dsta[y][x + 1] = 0;
//				dsta[y][x] = 1;
//			} else if(dsta[y + 1][x] == 0 && dr->y > 0 && dr->x < 0){
//				dr->y = 0;
//				dr->x = 0;
//				dsta[y][x + 1] = 0;
//				dsta[y + 1][x] = 1;
//			} else if(dsta[y + 1][x + 1] == 0 && dr->y > 0 && dr->x == 0){
//				dr->y = 0;
//				dsta[y][x + 1] = 0;
//				dsta[y + 1][x + 1] = 1;
//			}
//		}
//		if(dsta[y + 1][x] != 0){ // 2
//	//		DrShiftQuadro::DrQuadro::DrItem* dr = &dir.items[2];
//			auto dr = &srcd[y1][x1].shifts[shift].items[2];
//			if(dsta[y][x] == 0 && dr->y < 0 && dr->x == 0){
//				dr->y = 0;
//				dsta[y + 1][x] = 0;
//				dsta[y][x] = 1;
//			} else if(dsta[y][x + 1] == 0 && dr->y < 0 && dr->x > 0){
//				dr->y = 0;
//				dr->x = 0;
//				dsta[y + 1][x] = 0;
//				dsta[y][x + 1] = 1;
//			} else if(dsta[y + 1][x + 1] == 0 && dr->y == 0 && dr->x > 0){
//				dr->x = 0;
//				dsta[y + 1][x] = 0;
//				dsta[y + 1][x + 1] = 1;
//			}
//		}
//		if(dsta[y + 1][x + 1] != 0){ // 3
//			//DrShiftQuadro::DrQuadro::DrItem* dr = &dir.items[3];
//			auto dr = &srcd[y1][x1].shifts[shift].items[3];
//			if(dsta[y][x] == 0 && dr->y < 0 && dr->x < 0){
//				dr->y = 0;
//				dr->x = 0;
//				dsta[y + 1][x + 1] = 0;
//				dsta[y][x] = 1;
//			} else if(dsta[y][x + 1] == 0 && dr->y < 0 && dr->x == 0){
//				dr->y = 0;
//				dsta[y + 1][x + 1] = 0;
//				dsta[y][x + 1] = 1;
//			} else if(dsta[y + 1][x] == 0 && dr->y == 0 && dr->x < 0){
//				dr->x = 0;
//				dsta[y + 1][x + 1] = 0;
//				dsta[y + 1][x] = 1;
//			}
//		}
//} // //////////////////////////////////////////////////////////////////////////////////////////////
//
