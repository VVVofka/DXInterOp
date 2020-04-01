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
#include "RunA.h"
#include "runAlast.h"
//#include <ppl.h>
//#define AMPDBG
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
		//setConsole();
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
		m_data = std::unique_ptr<array<Vertex2D, 1>>(new array<Vertex2D, 1>(model.posLast()->size(), model.posLast()->begin(), m_accl_view));
		last_dirs = std::unique_ptr<array<FLT2, 2>>(new array<FLT2, 2>(model.sizeY(), model.sizeX(), model.last_dirs.begin(), m_accl_view));
		amask = std::unique_ptr<array<int, 1>>(new array<int, 1>(16, AMask, m_accl_view));
#endif
	} // ///////////////////////////////////////////////////////////////////////////////////////////////
	HRESULT get_data_d3dbuffer(void** d3dbuffer) const{
		return get_buffer(*m_data)->QueryInterface(__uuidof(ID3D11Buffer), (LPVOID*)d3dbuffer);
	} // ///////////////////////////////////////////////////////////////////////////////////////////////
	void run(){
		//return;
		int nlastlay = model.LaysCnt() - 1;
		//runAlast(*var_areas[nlastlay], *var_areas[nlastlay - 1], *amask);
		RunAlast::run(*var_areas[nlastlay], *var_areas[nlastlay - 1], *amask);
		for(int nlay = nlastlay - 1; nlay > 0; nlay--){
			//runA(*var_areas[nlay], *var_areas[nlay - 1], *amask);
			RunA::run(*var_areas[nlay], *var_areas[nlay - 1], *amask);
		}
		// Back to down
#ifdef AMPDBG
		//dumpA();
#endif
		for(int nlay = 1; nlay < nlastlay; nlay++){
			runD(*var_dirs[nlay - 1], *var_dirs[nlay], *var_areas[nlay]);
#ifdef AMPDBG
			//dumpD(nlay);
#endif
		}
		array<FLT2, 2>& dirs = *last_dirs;
		runDlast(*var_dirs[nlastlay - 1], *m_data, *var_areas[nlastlay], dirs);
        //concurrency::copy(*m_data, vpos.data());
		//for(int n=0; n<(int)vpos.size(); n++) printf("%d\t%f\t%f\n", n, vpos[n].Pos.y, vpos[n].Pos.x);
#ifdef AMPDBG
		//dumpPos();
		//dumpDLast();
		//dumpA();
#endif
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

#ifdef AMPDBG
		setConsole();
		printf("Before move:\n");
		dumpA(-1);
		//dumpDLast();
		dumpPos();
		struct myStruct4{
			int y, x, adry, adrx, newy, newx, aold, anew;
			FLT2 dirold, dirnew;
		};
		std::vector<myStruct4> dbg(dsta.extent.size());
		array_view<myStruct4, 2> av = array_view<myStruct4, 2>(dsta.extent[0], dsta.extent[1], dbg);
#endif
		const int szy = model.sizeY(), szx = model.sizeX();
		const float kwidthy = float(szy-1) / szy, kwidthx = float(szx - 1) / szx;
		const int leny = szy - 1, lenx = szx - 1;
		const float ky = (leny <= 0) ? 0 : 2.f / float(leny);
		const float kx = (lenx <= 0) ? 0 : 2.f / float(lenx);
		for(int nshift = 0; nshift < 4; nshift++){
			const int yshift = nshift / 2;
			const int xshift = nshift % 2;
#ifdef AMPDBG
			printf("\nshift = %d\n", nshift);
#endif
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
#ifdef AMPDBG
					av[y][x].dirold.y = dstd[y][x].y;
					av[y][x].dirold.x = dstd[y][x].x;
					av[y][x].x = x;
					av[y][x].y = y;
					av[y][x].aold = aold;
#endif
					if(aold < 0) continue;
					int signy = sign(int(dstd[y][x].y));
					int signx = sign(int(dstd[y][x].x));
					if((signx == xcell * 2 - 1) || (signy == ycell * 2 - 1)) continue;
					int adry = ycell * 4 + signy + 1; // 0..1 | 0..2
					int adrx = xcell * 4 + signx + 1;
					int newy = y + mask[adry];
					int newx = x + mask[adrx];
					int anew = dsta[newy][newx];
#ifdef AMPDBG
					av[y][x].adry = adry;
					av[y][x].adrx = adrx;
					av[y][x].newy = newy;
					av[y][x].newx = newx;
					av[y][x].anew = anew;
					av[y][x].dirnew.y = dstd[newy][newx].y;
					av[y][x].dirnew.x = dstd[newy][newx].x;
#endif
					if(anew >= 0) continue;

					dsta[y][x] = anew;
					dsta[newy][newx] = aold;

					//dstd[newy][newx].y = dstd[newy][newx].x = 0; // Block next moves by ncell
					dstd[y][x].y = dstd[y][x].x = 0;

					//dstpos[aold].Pos.y = (+(ky * newy - 1.0f)) * kwidthy;
					//dstpos[aold].Pos.x = (kx * newx - 1.0f) * kwidthx;
					dstpos[aold].Pos.y = normal(newy, szy);
					dstpos[aold].Pos.x = normal(newx, szx);
				}
			}); // parallel_for_each(srcd.extent,
#ifdef AMPDBG
			av.synchronize();
			//avpos.synchronize();
			dumpA(-1);
			//dumpDLast();
			dumpPos();
#endif
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
	void dumpPos(){
		array_view<Vertex2D, 1> avpos(m_data->extent[0]);
		m_data.get()->copy_to(avpos);
		for(int n = 0; n < (int)avpos.extent[0]; n++){
			auto p = avpos[n].Pos;
			printf("%d:\t%+.3f %+.3f\n", n, p.y, p.x);
		}
	} // ////////////////////////////////////////////////////////////////////////////////////////
}; // ******************************************************************************************************
#ifdef AMPDBG
#undef AMPDBG
#endif
