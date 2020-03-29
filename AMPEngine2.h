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
#include <iomanip>
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

						// 0  1  2  3  4  5  6  7  8  9  10 11 12 13 14 15
						// 00 10 01 11 00 10 01 11 00 10 01 11 00 10 01 11
						// 00 00 00 00 10 10 10 10 01 01 01 01 11 11 11 11
	const int AMask[16] = {0, 0, 0, 1, 0, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1};
	std::unique_ptr<array<int, 1>> amask;

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
		amask = std::unique_ptr<array<int, 1>>(new array<int, 1>(16, AMask, m_accl_view));

#endif
	} // ///////////////////////////////////////////////////////////////////////////////////////////////
	HRESULT get_data_d3dbuffer(void** d3dbuffer) const{
		return get_buffer(*m_data)->QueryInterface(__uuidof(ID3D11Buffer), (LPVOID*)d3dbuffer);
	} // ///////////////////////////////////////////////////////////////////////////////////////////////
	void run(){
		int nlastlay = model.LaysCnt() - 1;
		runAlast(*var_areas[nlastlay], *var_areas[nlastlay - 1], *amask);
		for(int nlay = nlastlay - 1; nlay > 0; nlay--){
			runA(*var_areas[nlay], *var_areas[nlay - 1], *amask);
		}
		// Back to down
		dumpA();
		for(int nlay = 1; nlay < nlastlay; nlay++){
			runD(*var_dirs[nlay - 1], *var_dirs[nlay], *var_areas[nlay]);
			//dumpD(nlay);
		}
		array<FLT2, 2>& dirs = *last_dirs;
		runDlast(*var_dirs[nlastlay - 1], *m_data, *var_areas[nlastlay], dirs);
		dumpDLast();
		dumpA();
	} // ///////////////////////////////////////////////////////////////////////////////////////////////
	void runAlast(const array<int, 2> & src, array<int, 2> & dst, const array<int, 1> & mask){
		parallel_for_each(dst.extent, [&dst, &src, &mask](index<2> idx) restrict(amp){
			// yx: c-centre, l-left, r-right
			const int y = idx[0];
			const int y2t = y * 2;
			const int y2c = y2t + 1;
			const int y2b = y2c + 1;

			const int x = idx[1];
			const int x2l = x * 2;
			const int x2c = x2l + 1;
			const int x2r = x2c + 1;

			const int cc = src[y2c][x2c] < 0 ? 0 : 1;  // << 0
			const int tc = src[y2t][x2c] < 0 ? 0 : 2;  // << 1
			const int cl = src[y2c][x2l] < 0 ? 0 : 4;  // << 2
			const int tl = src[y2t][x2l] < 0 ? 0 : 8;  // << 3

			const int br = src[y2b][x2r] < 0 ? 0 : 8;
			const int cr = src[y2c][x2r] < 0 ? 0 : 4;
			const int bc = src[y2b][x2c] < 0 ? 0 : 2;
			int sum = mask[cc + bc + cr + br];
			const int bl = src[y2b][x2l] < 0 ? 0 : 8;
			sum = (sum << 1) | mask[cc + bc + cl + bl];
			const int tr = src[y2t][x2r] < 0 ? 0 : 8;
			sum = (sum << 1) | mask[cc + tc + cr + tr];
			dst[y][x] = (sum << 1) | mask[cc + tc + cl + tl];
		});
	} // ///////////////////////////////////////////////////////////////////////////////////////////////
	void runA(const array<int, 2> & src, array<int, 2> & dst, const array<int, 1> & mask){
		parallel_for_each(dst.extent, [&dst, &src, &mask](index<2> idx) restrict(amp){
			const int y = idx[0];
			const int y2 = y * 2;
			const int x = idx[1];
			const int x2 = x * 2;

			// yx: l-left, r-right
			int tl = src[y2][x2];
			int tr = src[y2][x2 + 1];
			int bl = src[y2 + 1][x2];
			int br = src[y2 + 1][x2 + 1];

			int sum0 = mask[(tl & 1) + ((tr & 1) << 1) + ((bl & 1) << 2) + ((br & 1) << 3)];
			int sum1 = mask[((tl >>= 1) & 1) + (((tr >>= 1) & 1) << 1) + (((bl >>= 1) & 1) << 2) + (((br >>= 1) & 1) << 3)];
			int sum2 = mask[((tl >>= 1) & 1) + (((tr >>= 1) & 1) << 1) + (((bl >>= 1) & 1) << 2) + (((br >>= 1) & 1) << 3)];
			int sum3 = mask[(tl >> 1) + ((tr >> 1) << 1) + ((bl >> 1) << 2) + ((br >> 1) << 3)];
			dst[y][x] = (sum3 << 3) | (sum2 << 2) | (sum1 << 1) | sum0;
		});
	} // ///////////////////////////////////////////////////////////////////////////////////////////////
	void runD(const array<DrShiftQuadro, 2> & srcd, array<DrShiftQuadro, 2> & dstd, const array<int, 2> & dsta){
		parallel_for_each(srcd.extent, [&srcd, &dstd, &dsta](index<2> idx) restrict(amp){
			const float vdirsX[16 * 16] = {
		-1,-0,-1,-1, -0,+1,+1,+1,  -1,-1,-1,-0, +1,+1,-0,+1, //0000
		-0,-0,-0,-0, -0,-1,-0,-1,  +1,+1,+1,+1, -0,-1,-0,-1, //0001
		+1,-0,+1,-0, -0,-0,-0,-0,  +1,-0,+1,-0, -1,-1,-1,-1, //0010
		-0,-0,-0,-0, -0,-0,-0,-0,  -1,-1,-1,-1, +1,+1,+1,+1, //0011
		+1,+1,+1,+1, -0,-1,-0,-1,  -0,-0,-0,-0, -0,-1,-0,-1, //0100
		-0,-0,-0,-0, -1,-1,-0,-1,  -0,-0,-0,-0, -0,-1,-1,-1, //0101
		+1,+1,-1,+1, -0,-0,-0,-0,  -0,-0,-0,-0, -1,+1,-1,-1, //0110
		-0,-0,-0,-0, -0,-0,-0,-0,  -0,-0,-0,-0, +1,+1,-1,+1, //0111
		+1,-0,+1,-0, -1,-1,-1,-1,  +1,-0,+1,-0, -0,-0,-0,-0, //1000
		-0,-0,-0,-0, -1,-1,-1,+1,  -1,+1,+1,+1, -0,-0,-0,-0, //1001
		+1,+1,+1,-0, -0,-0,-0,-0,  +1,-0,+1,+1, -0,-0,-0,-0, //1010
		-0,-0,-0,-0, -0,-0,-0,-0,  -1,-1,-1,+1, -0,-0,-0,-0, //1011
		-1,-1,-1,-1, +1,+1,+1,+1,  -0,-0,-0,-0, -0,-0,-0,-0, //1100
		-0,-0,-0,-0, -1,+1,+1,+1,  -0,-0,-0,-0, -0,-0,-0,-0, //1101
		-1,+1,-1,-1, -0,-0,-0,-0,  -0,-0,-0,-0, -0,-0,-0,-0, //1110
		-1,-0,-1,-1, -0,+1,+1,+1,  -1,-1,-1,-0, +1,+1,-0,+1};//1111
			const float vdirsY[16 * 16] = {
		-1,-1,-0,-1, -1,-1,-1,-0,  -0,+1,+1,+1, +1,-0,+1,+1, //0000
		-0,-0,-0,-0, +1,+1,+1,+1,  -0,-0,-1,-1, -0,-0,-1,-1, //0001
		+1,+1,+1,+1, -0,-0,-0,-0,  -0,-0,-1,-1, -0,-0,-1,-1, //0010
		-0,-0,-0,-0, -0,-0,-0,-0,  -1,-0,-1,-1, -0,-1,-1,-1, //0011
		+1,+1,-0,-0, +1,+1,-0,-0,  -0,-0,-0,-0, -1,-1,-1,-1, //0100
		-0,-0,-0,-0, -1,-1,-1,-1,  -0,-0,-0,-0, +1,+1,+1,+1, //0101
		+1,-1,+1,+1, -0,-0,-0,-0,  -0,-0,-0,-0, -1,-1,+1,-1, //0110
		-0,-0,-0,-0, -0,-0,-0,-0,  -0,-0,-0,-0, +1,-1,+1,+1, //0111
		+1,+1,-0,-0, +1,+1,-0,-0,  -1,-1,-1,-1, -0,-0,-0,-0, //1000
		-0,-0,-0,-0, -1,+1,+1,+1,  -1,-1,-1,+1, -0,-0,-0,-0, //1001
		-1,-1,-1,-1, -0,-0,-0,-0,  +1,+1,+1,+1, -0,-0,-0,-0, //1010
		-0,-0,-0,-0, -0,-0,-0,-0,  -1,+1,+1,+1, -0,-0,-0,-0, //1011
		+1,+1,+1,-0, +1,+1,-0,+1,  -0,-0,-0,-0, -0,-0,-0,-0, //1100
		-0,-0,-0,-0, -1,-1,-1,+1,  -0,-0,-0,-0, -0,-0,-0,-0, //1101
		-1,-1,+1,-1, -0,-0,-0,-0,  -0,-0,-0,-0, -0,-0,-0,-0, //1110
		-1,-1,-0,-1, -1,-1,-1,-0,  -0,+1,+1,+1, +1,-0,+1,+1};//1111

			const int y = idx[0];
			const int x = idx[1];
			const int y2 = y * 2;
			const int x2 = x * 2;

			int tl = dsta[y2][x2];
			int tr = dsta[y2][x2 + 1];
			int bl = dsta[y2 + 1][x2];
			int br = dsta[y2 + 1][x2 + 1];
			int mask[4]; // shift
			mask[0] = ((tl & 1) + ((tr & 1) << 1) + ((bl & 1) << 2) + ((br & 1) << 3));
			mask[1] = (((tl >>= 1) & 1) + (((tr >>= 1) & 1) << 1) + (((bl >>= 1) & 1) << 2) + (((br >>= 1) & 1) << 3));
			mask[2] = (((tl >>= 1) & 1) + (((tr >>= 1) & 1) << 1) + (((bl >>= 1) & 1) << 2) + (((br >>= 1) & 1) << 3));
			mask[3] = ((tl >> 1) + ((tr >> 1) << 1) + ((bl >> 1) << 2) + ((br >> 1) << 3));

			auto srcdcell = &srcd[y][x];
			for(int shift = 0; shift < 4; shift++){
				int nmask = 16 * mask[shift];
				auto srcsh = &srcdcell->shifts[shift];
				for(int ncell = 0; ncell < 4; ncell++){
					FLT2 src = srcsh->items[ncell];
					auto item = &dstd[y2 + ncell / 2][x2 + ncell % 2].shifts[shift];
					{FLT2* dst = &item->items[0]; dst->x = src.x + vdirsX[nmask]; dst->y = src.y + vdirsY[nmask++]; }
					{FLT2* dst = &item->items[1]; dst->x = src.x + vdirsX[nmask]; dst->y = src.y + vdirsY[nmask++]; }
					{FLT2* dst = &item->items[2]; dst->x = src.x + vdirsX[nmask]; dst->y = src.y + vdirsY[nmask++]; }
					{FLT2* dst = &item->items[3]; dst->x = src.x + vdirsX[nmask]; dst->y = src.y + vdirsY[nmask++]; }
				}
			}
		});
	} // ///////////////////////////////////////////////////////////////////////////////////////////////
	void runDlast(const array<DrShiftQuadro, 2> & srcd,
				  array<Vertex2D, 1> & dstpos,
				  array<int, 2> & dsta,
				  array<FLT2, 2> & dstd){
		for(int nshift = 0; nshift < 4; nshift++){
			parallel_for_each(srcd.extent, [=, &srcd, &dstd](index<2> idx) restrict(amp){
				const int y = idx[0];
				const int x = idx[1];
				auto q = srcd[y][x].shifts[nshift].items;
				const int y2 = y * 2 + nshift / 2; // y*2+yshift
				const int x2 = x * 2 + nshift % 2;

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

		parallel_for_each(dstd.extent, [&dstd](index<2> idx) restrict(amp){
			int y = idx[0];
			int x = idx[1];
			float diry = dstd[y][x].y;
			float absdiry = fabsf(diry);
			float dirx = dstd[y][x].x;
			float absdirx = fabsf(dirx);
			if(absdirx >= 2 * absdiry)
				diry = 0;
			else if(absdiry >= 2 * absdirx)
				dirx = 0;
			dstd[y][x].y = diry;
			dstd[y][x].x = dirx;
		}); // parallel_for_each(srcd.extent,
		dumpA(-1);
		dumpDLast();

		struct myStruct4{
			int y, x, newy, newx, aold, anew;
			FLT2 dirold, dirnew;
		};
		std::vector<myStruct4> dbg4(dsta.extent.size());
		array_view<myStruct4, 2> av = array_view<myStruct4, 2>(dsta.extent[0], dsta.extent[1], dbg4);

		const int szy = model.sizeY(), szx = model.sizeX();
		float ky = 2.f / szy;
		float kx = 2.f / szx;
		for(int nshift = 0; nshift < 4; nshift++){
			int yshift = nshift / 2;
			int xshift = nshift % 2;
			printf("\nshift = %d\n", nshift);
			parallel_for_each(srcd.extent, [=, &dsta, &dstd, &dstpos](index<2> idx) restrict(amp){
				const int mask[7] = {0,0,1, 9, -1,0,0};
				int y0 = idx[0] * 2 + yshift;
				int x0 = idx[1] * 2 + xshift;
				for(int ncell = 0; ncell < 4; ncell++){
					int ycell = ncell / 2;
					int xcell = ncell % 2;
					int y = y0 + ycell;
					int x = x0 + xcell;
					int aold = dsta[y][x];

					av[y][x].dirold.y = dstd[y][x].y;
					av[y][x].dirold.x = dstd[y][x].x;
					av[y][x].x = x;
					av[y][x].y = y;
					av[y][x].aold = aold;
					if(aold < 0) continue;
					int adry = ycell * 4 + sign(int(dstd[y][x].y)) + 1; // 0..1 | 0..2
					int adrx = xcell * 4 + sign(int(dstd[y][x].x)) + 1;
					int newy = y + mask[adry];
					int newx = x + mask[adrx];
					int anew = dsta[newy][newx];
					av[y][x].newy = newy;
					av[y][x].newx = newx;
					av[y][x].anew = anew;
					av[y][x].dirnew.y = dstd[newy][newx].y;
					av[y][x].dirnew.x = dstd[newy][newx].x;
					if(anew >= 0) continue;

					dsta[y][x] = anew;
					dsta[newy][newx] = aold;

					//dstd[newy][newx].y = dstd[newy][newx].x = -1;
					dstd[y][x].y = dstd[y][x].x = -1;

					dstpos[aold].Pos.y = ky * newy - 1.0f;
					dstpos[aold].Pos.x = kx * newx - 1.0f;
				}
			}); // parallel_for_each(srcd.extent,
			av.synchronize();
			dumpA(-1);
			dumpDLast();
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
	void dumpA(int nlay){
		const char separ[] = " ";
		if(nlay < 0) nlay = model.LaysCnt() - 1;
		setConsole();
		array<int, 2> av(*var_areas[nlay].get());
		std::cout << "A[" << nlay << "] y*x: " << av.extent[0] << "*" << av.extent[1] << std::endl;
		if(nlay == int(var_areas.size() - 1)){
			for(int y = 0; y < av.extent[0]; y++){
				for(int x = 0; x < av.extent[1]; x++){
					int q = av[y][x];
					if(q < 0)
						std::cout << " . ";
					else
						printf("%2d ", q);
				}
				std::cout << std::endl;
			}
		} else{
			for(int y = 0; y < av.extent[0]; y++){
				for(int x = 0; x < av.extent[1]; x++){
					int q = av[y][x];
					if(q < 0)
						std::cout << ". ";
					else
						std::cout << (q >> 3) << (1 & (q >> 2)) << (1 & (q >> 1)) << (1 & q) << separ;
				}
				std::cout << std::endl;
			}
		}
	} // ////////////////////////////////////////////////////////////////
	void dumpA(){
		setConsole();
		for(int nlay = 0; nlay < model.LaysCnt(); nlay++){
			dumpA(nlay);
			std::cout << std::endl;
		}
	} // ////////////////////////////////////////////////////////////////////////////////////////
	void dumpD(int nlay){
		if(nlay < 0) nlay = model.LaysCnt() - 1;
		setConsole();
		array<DrShiftQuadro, 2> av(*var_dirs[nlay].get());
		std::cout << "Dirs[" << nlay << "] y*x: " << av.extent[0] << "*" << av.extent[1] << std::endl;
		for(int y = 0; y < av.extent[0]; y++){
			for(int x = 0; x < av.extent[1]; x++){
				//if(av[y][x].not0()){
				std::cout << " Y=" << y << " X=" << x << std::endl;
				av[y][x].dump();
				std::cout << std::endl;
			//}
			}
		}
		std::cout << std::endl;
	} // ////////////////////////////////////////////////////////////////
	void dumpD(){
		for(int nlay = 0; nlay < model.LaysCnt() - 1; nlay++)
			dumpD(nlay);
		dumpDLast();
	} // ////////////////////////////////////////////////////////////////////////////////////////
	void dumpDLast(){
		int szy = last_dirs->extent[0],
			szx = last_dirs->extent[1];
		if(szy <= 0 || szx <= 0) return;
		array<FLT2, 2> av(*last_dirs);
		std::cout << "DirsLast[" << model.LaysCnt() - 1 << "] y*x: " << szy << "*" << szx;
		for(int y = 0; y < szy; y++){
			printf("\n%+.2f", av[0][0].y);
			for(int x = 1; x < szx; x++)
				printf("  %+.2f", av[y][x].y);

			printf("\n%+.2f", av[0][0].x);
			for(int x = 1; x < szx; x++)
				printf("  %+.2f", av[y][x].x);
			printf("\n");
		}
	} // ///////////////////////////////////////////////////////////////////////////////////////////////
}; // ******************************************************************************************************
